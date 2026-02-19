#pragma once
#include <vector>
#include <cstdint>

#pragma once
#include <vector>
#include <cstdint>
#include "Renderer/MathTypes.h"

namespace TONGUE {
    struct Vertex {
        Vector3 position;
        Vector3 normal;
        Vector2 uv;
    };

    class Mesh {
    public:
        void Upload(const std::vector<Vertex>& verts,
                    const std::vector<uint32_t>& indices);

        void Draw() const;
    };
}
