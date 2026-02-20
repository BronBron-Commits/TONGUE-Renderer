#include "Renderer.h"
#include "Renderer/Color.h"
#include "Renderer/RenderSubmission.h"
#include "Renderer/Mesh.h"
#include "Renderer/Texture.h"
#include "Renderer/Shader.h"
#include <memory>
#include <vector>
 #include <d3d12.h>
#include <dxgi1_6.h>
#include <d3dcompiler.h>
#include <wrl/client.h>
#include "../include/d3dx12.h" // DirectX 12 helper classes
using Microsoft::WRL::ComPtr;

namespace TONGUE {

struct Renderer::Impl {
	HWND hwnd = nullptr;
	ComPtr<ID3D12Device> device;
	ComPtr<IDXGISwapChain3> swapChain;
	ComPtr<ID3D12CommandQueue> commandQueue;
	ComPtr<ID3D12CommandAllocator> commandAllocator;
	ComPtr<ID3D12GraphicsCommandList> commandList;
	ComPtr<ID3D12DescriptorHeap> rtvHeap;
	UINT rtvDescriptorSize = 0;
	std::vector<ComPtr<ID3D12Resource>> renderTargets;
	UINT frameIndex = 0;
	HANDLE fenceEvent = nullptr;
	ComPtr<ID3D12Fence> fence;
	UINT64 fenceValue = 0;
	// Triangle resources
	ComPtr<ID3D12Resource> vertexBuffer;
	D3D12_VERTEX_BUFFER_VIEW vertexBufferView = {};
	ComPtr<ID3D12RootSignature> rootSignature;
	ComPtr<ID3D12PipelineState> pipelineState;
};

Renderer::Renderer() : impl(std::make_unique<Impl>()) {}
Renderer::~Renderer() = default;

void Renderer::SetHWND(void* hwnd) {
	impl->hwnd = static_cast<HWND>(hwnd);
}

bool Renderer::Init(int width, int height, const char* title) {
	HRESULT hr;
	UINT dxgiFactoryFlags = 0;
	ComPtr<IDXGIFactory4> factory;
	hr = CreateDXGIFactory2(dxgiFactoryFlags, IID_PPV_ARGS(&factory));
	if (FAILED(hr)) return false;

	hr = D3D12CreateDevice(nullptr, D3D_FEATURE_LEVEL_11_0, IID_PPV_ARGS(&impl->device));
	if (FAILED(hr)) return false;

	D3D12_COMMAND_QUEUE_DESC queueDesc = {};
	queueDesc.Flags = D3D12_COMMAND_QUEUE_FLAG_NONE;
	queueDesc.Type = D3D12_COMMAND_LIST_TYPE_DIRECT;
	hr = impl->device->CreateCommandQueue(&queueDesc, IID_PPV_ARGS(&impl->commandQueue));
	if (FAILED(hr)) return false;

	// Swap chain
	DXGI_SWAP_CHAIN_DESC1 scDesc = {};
	scDesc.BufferCount = 2;
	scDesc.Width = width;
	scDesc.Height = height;
	scDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	scDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	scDesc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;
	scDesc.SampleDesc.Count = 1;
	ComPtr<IDXGISwapChain1> swapChain1;
	hr = factory->CreateSwapChainForHwnd(impl->commandQueue.Get(), impl->hwnd, &scDesc, nullptr, nullptr, &swapChain1);
	if (FAILED(hr)) return false;
	hr = swapChain1.As(&impl->swapChain);
	if (FAILED(hr)) return false;
	impl->frameIndex = impl->swapChain->GetCurrentBackBufferIndex();

	// Descriptor heap for render target views
	D3D12_DESCRIPTOR_HEAP_DESC rtvHeapDesc = {};
	rtvHeapDesc.NumDescriptors = 2;
	rtvHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_RTV;
	rtvHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_NONE;
	hr = impl->device->CreateDescriptorHeap(&rtvHeapDesc, IID_PPV_ARGS(&impl->rtvHeap));
	if (FAILED(hr)) return false;
	impl->rtvDescriptorSize = impl->device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV);

	// Create frame resources
	CD3DX12_CPU_DESCRIPTOR_HANDLE rtvHandle(impl->rtvHeap->GetCPUDescriptorHandleForHeapStart());
	impl->renderTargets.resize(2);
	for (UINT i = 0; i < 2; ++i) {
		hr = impl->swapChain->GetBuffer(i, IID_PPV_ARGS(&impl->renderTargets[i]));
		if (FAILED(hr)) return false;
		impl->device->CreateRenderTargetView(impl->renderTargets[i].Get(), nullptr, rtvHandle);
		rtvHandle.Offset(1, impl->rtvDescriptorSize);
	}

	// Command allocator and list
	hr = impl->device->CreateCommandAllocator(D3D12_COMMAND_LIST_TYPE_DIRECT, IID_PPV_ARGS(&impl->commandAllocator));
	if (FAILED(hr)) return false;
	hr = impl->device->CreateCommandList(0, D3D12_COMMAND_LIST_TYPE_DIRECT, impl->commandAllocator.Get(), nullptr, IID_PPV_ARGS(&impl->commandList));
	if (FAILED(hr)) return false;
	impl->commandList->Close();

	// Fence
	hr = impl->device->CreateFence(0, D3D12_FENCE_FLAG_NONE, IID_PPV_ARGS(&impl->fence));
	if (FAILED(hr)) return false;
	impl->fenceValue = 1;
	impl->fenceEvent = CreateEvent(nullptr, FALSE, FALSE, nullptr);
	if (!impl->fenceEvent) return false;

	// Root signature
	D3D12_ROOT_SIGNATURE_DESC rootSignatureDesc = {};
	rootSignatureDesc.Flags = D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT;
	ComPtr<ID3DBlob> signature;
	ComPtr<ID3DBlob> error;
	hr = D3D12SerializeRootSignature(&rootSignatureDesc, D3D_ROOT_SIGNATURE_VERSION_1, &signature, &error);
	if (FAILED(hr)) return false;
	hr = impl->device->CreateRootSignature(0, signature->GetBufferPointer(), signature->GetBufferSize(), IID_PPV_ARGS(&impl->rootSignature));
	if (FAILED(hr)) return false;

	// Shaders (hardcoded HLSL)
	const char* vsCode = R"(
	struct VSInput { float3 pos : POSITION; float4 color : COLOR; };
	struct PSInput { float4 pos : SV_POSITION; float4 color : COLOR; };
	PSInput main(VSInput input) {
		PSInput output;
		output.pos = float4(input.pos, 1.0);
		output.color = input.color;
		return output;
	})";
	const char* psCode = R"(
	struct PSInput { float4 pos : SV_POSITION; float4 color : COLOR; };
	float4 main(PSInput input) : SV_TARGET { return input.color; })";
	ComPtr<ID3DBlob> vsBlob, psBlob;
	hr = D3DCompile(vsCode, strlen(vsCode), nullptr, nullptr, nullptr, "main", "vs_5_0", 0, 0, &vsBlob, nullptr);
	if (FAILED(hr)) return false;
	hr = D3DCompile(psCode, strlen(psCode), nullptr, nullptr, nullptr, "main", "ps_5_0", 0, 0, &psBlob, nullptr);
	if (FAILED(hr)) return false;

	// Input layout
	D3D12_INPUT_ELEMENT_DESC inputLayout[] = {
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
		{ "COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 12, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
	};

	// Pipeline state
	D3D12_GRAPHICS_PIPELINE_STATE_DESC psoDesc = {};
	psoDesc.InputLayout = { inputLayout, _countof(inputLayout) };
	psoDesc.pRootSignature = impl->rootSignature.Get();
	psoDesc.VS = { vsBlob->GetBufferPointer(), vsBlob->GetBufferSize() };
	psoDesc.PS = { psBlob->GetBufferPointer(), psBlob->GetBufferSize() };
	psoDesc.RasterizerState = CD3DX12_RASTERIZER_DESC();
	psoDesc.BlendState = CD3DX12_BLEND_DESC();
	psoDesc.DepthStencilState.DepthEnable = FALSE;
	psoDesc.DepthStencilState.StencilEnable = FALSE;
	psoDesc.SampleMask = UINT_MAX;
	psoDesc.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;
	psoDesc.NumRenderTargets = 1;
	psoDesc.RTVFormats[0] = DXGI_FORMAT_R8G8B8A8_UNORM;
	psoDesc.SampleDesc.Count = 1;
	hr = impl->device->CreateGraphicsPipelineState(&psoDesc, IID_PPV_ARGS(&impl->pipelineState));
	if (FAILED(hr)) return false;

	// Vertex buffer (triangle)
	struct Vertex { float pos[3]; float color[4]; };
	Vertex triangleVerts[] = {
		{ { 0.0f, 0.5f, 0.0f }, { 1, 0, 0, 1 } },
		{ { 0.5f, -0.5f, 0.0f }, { 0, 1, 0, 1 } },
		{ { -0.5f, -0.5f, 0.0f }, { 0, 0, 1, 1 } },
	};
	const UINT vbSize = sizeof(triangleVerts);
	CD3DX12_HEAP_PROPERTIES heapProps(D3D12_HEAP_TYPE_UPLOAD);
	CD3DX12_RESOURCE_DESC vbDesc = CD3DX12_RESOURCE_DESC::Buffer(vbSize);
	hr = impl->device->CreateCommittedResource(
		&heapProps, D3D12_HEAP_FLAG_NONE, &vbDesc,
		D3D12_RESOURCE_STATE_GENERIC_READ, nullptr, IID_PPV_ARGS(&impl->vertexBuffer));
	if (FAILED(hr)) return false;
	void* pData;
	hr = impl->vertexBuffer->Map(0, nullptr, &pData);
	if (FAILED(hr)) return false;
	memcpy(pData, triangleVerts, vbSize);
	impl->vertexBuffer->Unmap(0, nullptr);
	impl->vertexBufferView.BufferLocation = impl->vertexBuffer->GetGPUVirtualAddress();
	impl->vertexBufferView.StrideInBytes = sizeof(Vertex);
	impl->vertexBufferView.SizeInBytes = vbSize;

	return true;
}

void Renderer::Shutdown() {
	// DirectX 12 cleanup will go here
}

void Renderer::BeginFrame() {
	// Record commands to draw triangle
	HRESULT hr = impl->commandAllocator->Reset();
	if (FAILED(hr)) return;
	hr = impl->commandList->Reset(impl->commandAllocator.Get(), impl->pipelineState.Get());
	if (FAILED(hr)) return;

	D3D12_RESOURCE_BARRIER barrier = CD3DX12_RESOURCE_BARRIER::Transition(
		impl->renderTargets[impl->frameIndex].Get(),
		D3D12_RESOURCE_STATE_PRESENT, D3D12_RESOURCE_STATE_RENDER_TARGET);
	impl->commandList->ResourceBarrier(1, &barrier);

	CD3DX12_CPU_DESCRIPTOR_HANDLE rtvHandle(
		impl->rtvHeap->GetCPUDescriptorHandleForHeapStart(),
		impl->frameIndex, impl->rtvDescriptorSize);
	impl->commandList->OMSetRenderTargets(1, &rtvHandle, FALSE, nullptr);

	const float clearColor[] = { 0.1f, 0.1f, 0.2f, 1.0f };
	impl->commandList->ClearRenderTargetView(rtvHandle, clearColor, 0, nullptr);

	impl->commandList->SetGraphicsRootSignature(impl->rootSignature.Get());
	CD3DX12_VIEWPORT viewport(0.0f, 0.0f, 800.0f, 600.0f);
	impl->commandList->RSSetViewports(1, &viewport);
	CD3DX12_RECT scissorRect(0, 0, 800, 600);
	impl->commandList->RSSetScissorRects(1, &scissorRect);
	impl->commandList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	impl->commandList->IASetVertexBuffers(0, 1, &impl->vertexBufferView);
	impl->commandList->DrawInstanced(3, 1, 0, 0);
}

void Renderer::SetViewport(int x, int y, int width, int height) {
	// Set viewport
}

void Renderer::Clear(const Color& color) {
	// Clear render target
}

void Renderer::Submit(const RenderSubmission& submission) {
	// Queue mesh/texture/material for drawing
}

void Renderer::EndFrame() {
	// Transition back to present
	D3D12_RESOURCE_BARRIER barrier = CD3DX12_RESOURCE_BARRIER::Transition(
		impl->renderTargets[impl->frameIndex].Get(),
		D3D12_RESOURCE_STATE_RENDER_TARGET, D3D12_RESOURCE_STATE_PRESENT);
	impl->commandList->ResourceBarrier(1, &barrier);
	impl->commandList->Close();
}

void Renderer::Present() {
	// Execute and present
	ID3D12CommandList* ppCommandLists[] = { impl->commandList.Get() };
	impl->commandQueue->ExecuteCommandLists(1, ppCommandLists);
	impl->swapChain->Present(1, 0);
	// Wait for frame
	const UINT64 fenceToWaitFor = impl->fenceValue;
	impl->commandQueue->Signal(impl->fence.Get(), fenceToWaitFor);
	impl->fenceValue++;
	if (impl->fence->GetCompletedValue() < fenceToWaitFor) {
		impl->fence->SetEventOnCompletion(fenceToWaitFor, impl->fenceEvent);
		WaitForSingleObject(impl->fenceEvent, INFINITE);
	}
	impl->frameIndex = impl->swapChain->GetCurrentBackBufferIndex();
}

bool Renderer::WindowShouldClose() const {
	return false;
}

std::shared_ptr<Mesh> Renderer::CreateMesh(/* params */) {
	return nullptr;
}

std::shared_ptr<Texture> Renderer::CreateTexture(/* params */) {
	return nullptr;
}

std::shared_ptr<Shader> Renderer::CreateShader(/* params */) {
	return nullptr;
}

} // namespace TONGUE
