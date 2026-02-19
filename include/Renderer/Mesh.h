#pragma once

// Mesh & model representation for TONGUE Renderer

namespace TONGUE {

class Mesh {
public:
    bool Load(const char* path);
    void Draw() const;
};

} // namespace TONGUE
