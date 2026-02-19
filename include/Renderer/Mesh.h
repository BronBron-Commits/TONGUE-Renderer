#pragma once
#include <vector>
#include <cstdint>
#include "Renderer/MathTypes.h"
#include <GL/glew.h>
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
        Mesh() : VAO(0), VBO(0), EBO(0), vertexCount(0), indexCount(0) {}
        void Upload(const std::vector<Vertex>& verts,
                    const std::vector<uint32_t>& indices);
        void Draw() const;
    private:
        GLuint VAO, VBO, EBO;
        GLsizei vertexCount, indexCount;
    };
}
