//*********************************************************
//
// Copyright (c) Microsoft Corporation.
// Licensed under the MIT License (MIT).
//
//*********************************************************

#ifndef __D3DX12_H__
#define __D3DX12_H__

#include "d3d12.h"

// Minimal single-header version for DirectX 12 helper classes
// Only the most common helpers are included for triangle rendering

struct CD3DX12_CPU_DESCRIPTOR_HANDLE : public D3D12_CPU_DESCRIPTOR_HANDLE {
    CD3DX12_CPU_DESCRIPTOR_HANDLE() = default;
    explicit CD3DX12_CPU_DESCRIPTOR_HANDLE(const D3D12_CPU_DESCRIPTOR_HANDLE &o) : D3D12_CPU_DESCRIPTOR_HANDLE(o) {}
    CD3DX12_CPU_DESCRIPTOR_HANDLE(CD3DX12_CPU_DESCRIPTOR_HANDLE const &o) = default;
    CD3DX12_CPU_DESCRIPTOR_HANDLE(D3D12_CPU_DESCRIPTOR_HANDLE other, INT offsetScaledByIncrementSize) {
        ptr = other.ptr + offsetScaledByIncrementSize;
    }
    CD3DX12_CPU_DESCRIPTOR_HANDLE(D3D12_CPU_DESCRIPTOR_HANDLE other, INT offsetInDescriptors, UINT descriptorIncrementSize) {
        ptr = other.ptr + INT64(offsetInDescriptors) * INT64(descriptorIncrementSize);
    }
    inline void Offset(INT offsetInDescriptors, UINT descriptorIncrementSize) {
        ptr = ptr + INT64(offsetInDescriptors) * INT64(descriptorIncrementSize);
    }
};

struct CD3DX12_RESOURCE_BARRIER : public D3D12_RESOURCE_BARRIER {
    static D3D12_RESOURCE_BARRIER Transition(
        ID3D12Resource *pResource,
        D3D12_RESOURCE_STATES stateBefore,
        D3D12_RESOURCE_STATES stateAfter,
        UINT subresource = D3D12_RESOURCE_BARRIER_ALL_SUBRESOURCES,
        D3D12_RESOURCE_BARRIER_FLAGS flags = D3D12_RESOURCE_BARRIER_FLAG_NONE) {
        D3D12_RESOURCE_BARRIER result = {};
        result.Type = D3D12_RESOURCE_BARRIER_TYPE_TRANSITION;
        result.Flags = flags;
        result.Transition.pResource = pResource;
        result.Transition.StateBefore = stateBefore;
        result.Transition.StateAfter = stateAfter;
        result.Transition.Subresource = subresource;
        return *(CD3DX12_RESOURCE_BARRIER*)&result;
    }
};

struct CD3DX12_HEAP_PROPERTIES : public D3D12_HEAP_PROPERTIES {
    CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE type) {
        Type = type;
        CPUPageProperty = D3D12_CPU_PAGE_PROPERTY_UNKNOWN;
        MemoryPoolPreference = D3D12_MEMORY_POOL_UNKNOWN;
        CreationNodeMask = 1;
        VisibleNodeMask = 1;
    }
};

struct CD3DX12_RESOURCE_DESC : public D3D12_RESOURCE_DESC {
    static CD3DX12_RESOURCE_DESC Buffer(UINT64 width) {
        CD3DX12_RESOURCE_DESC desc = {};
        desc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
        desc.Alignment = 0;
        desc.Width = width;
        desc.Height = 1;
        desc.DepthOrArraySize = 1;
        desc.MipLevels = 1;
        desc.Format = DXGI_FORMAT_UNKNOWN;
        desc.SampleDesc.Count = 1;
        desc.SampleDesc.Quality = 0;
        desc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;
        desc.Flags = D3D12_RESOURCE_FLAG_NONE;
        return desc;
    }
};

struct CD3DX12_VIEWPORT : public D3D12_VIEWPORT {
    CD3DX12_VIEWPORT(float topLeftX, float topLeftY, float width, float height, float minDepth = 0.0f, float maxDepth = 1.0f) {
        TopLeftX = topLeftX;
        TopLeftY = topLeftY;
        Width = width;
        Height = height;
        MinDepth = minDepth;
        MaxDepth = maxDepth;
    }
};

struct CD3DX12_RECT : public D3D12_RECT {
    CD3DX12_RECT(LONG l, LONG t, LONG r, LONG b) {
        left = l;
        top = t;
        right = r;
        bottom = b;
    }
};


struct CD3DX12_RASTERIZER_DESC : public D3D12_RASTERIZER_DESC {
    CD3DX12_RASTERIZER_DESC(const D3D12_RASTERIZER_DESC& o) : D3D12_RASTERIZER_DESC(o) {}
    CD3DX12_RASTERIZER_DESC(D3D12_FILL_MODE fillMode = D3D12_FILL_MODE_SOLID, D3D12_CULL_MODE cullMode = D3D12_CULL_MODE_BACK, BOOL frontCounterClockwise = FALSE, INT depthBias = D3D12_DEFAULT_DEPTH_BIAS, FLOAT depthBiasClamp = D3D12_DEFAULT_DEPTH_BIAS_CLAMP, FLOAT slopeScaledDepthBias = D3D12_DEFAULT_SLOPE_SCALED_DEPTH_BIAS, BOOL depthClipEnable = TRUE, BOOL multisampleEnable = FALSE, BOOL antialiasedLineEnable = FALSE, UINT forcedSampleCount = 0, D3D12_CONSERVATIVE_RASTERIZATION_MODE conservativeRaster = D3D12_CONSERVATIVE_RASTERIZATION_MODE_OFF) {
        FillMode = fillMode;
        CullMode = cullMode;
        FrontCounterClockwise = frontCounterClockwise;
        DepthBias = depthBias;
        DepthBiasClamp = depthBiasClamp;
        SlopeScaledDepthBias = slopeScaledDepthBias;
        DepthClipEnable = depthClipEnable;
        MultisampleEnable = multisampleEnable;
        AntialiasedLineEnable = antialiasedLineEnable;
        ForcedSampleCount = forcedSampleCount;
        ConservativeRaster = conservativeRaster;
    }
};

struct CD3DX12_BLEND_DESC : public D3D12_BLEND_DESC {
    CD3DX12_BLEND_DESC() {
        AlphaToCoverageEnable = FALSE;
        IndependentBlendEnable = FALSE;
        for (UINT i = 0; i < D3D12_SIMULTANEOUS_RENDER_TARGET_COUNT; ++i) {
            RenderTarget[i].BlendEnable = FALSE;
            RenderTarget[i].LogicOpEnable = FALSE;
            RenderTarget[i].SrcBlend = D3D12_BLEND_ONE;
            RenderTarget[i].DestBlend = D3D12_BLEND_ZERO;
            RenderTarget[i].BlendOp = D3D12_BLEND_OP_ADD;
            RenderTarget[i].SrcBlendAlpha = D3D12_BLEND_ONE;
            RenderTarget[i].DestBlendAlpha = D3D12_BLEND_ZERO;
            RenderTarget[i].BlendOpAlpha = D3D12_BLEND_OP_ADD;
            RenderTarget[i].LogicOp = D3D12_LOGIC_OP_NOOP;
            RenderTarget[i].RenderTargetWriteMask = D3D12_COLOR_WRITE_ENABLE_ALL;
        }
    }
    CD3DX12_BLEND_DESC(const D3D12_BLEND_DESC& o) : D3D12_BLEND_DESC(o) {}
    CD3DX12_BLEND_DESC(const D3D12_BLEND_DESC* o) : D3D12_BLEND_DESC(*o) {}
};

#endif // __D3DX12_H__
