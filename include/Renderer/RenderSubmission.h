// RenderSubmission.h
#pragma once
#include <memory>
namespace TONGUE {
class Mesh;
class Texture;
struct RenderSubmission {
    std::shared_ptr<Mesh> mesh;
    std::shared_ptr<Texture> texture;
    // Add transform, material, etc. as needed
};
}