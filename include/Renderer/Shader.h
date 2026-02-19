#pragma once

// Shader management for TONGUE Renderer

namespace TONGUE {

class Shader {
public:
    bool Load(const char* vertexPath, const char* fragmentPath);
    void Bind() const;
    void Unbind() const;
};

} // namespace TONGUE
