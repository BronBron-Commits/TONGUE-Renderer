#pragma once

// Texture abstraction for TONGUE Renderer

namespace TONGUE {

class Texture {
public:
    bool Load(const char* path);
    void Bind(unsigned slot = 0) const;
    void Unbind() const;
};

} // namespace TONGUE
