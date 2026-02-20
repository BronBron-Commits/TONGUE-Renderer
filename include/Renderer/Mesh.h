#pragma once
#include <vector>
#include <cstdint>
#include "Renderer/MathTypes.h"
using TONGUE::Vector2;
using TONGUE::Vector3;

namespace TONGUE {
    struct Vertex {
        Vector3 position;
        Vector3 normal;
        Vector2 uv;
    };

    class Mesh {
    public:
        Mesh() : vertexCount(0), indexCount(0) {}
        void Upload(const std::vector<Vertex>& verts,
                    const std::vector<uint32_t>& indices);
        void Draw() const;
    private:
        // DirectX 12 resource handles will go here
        // Example: ComPtr<ID3D12Resource> vertexBuffer;
        uint32_t vertexCount, indexCount;
    };
}
