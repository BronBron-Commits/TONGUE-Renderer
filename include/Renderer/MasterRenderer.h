#pragma once

#include "Renderer/Window.h"
#include "Renderer/Mesh.h"
#include "Renderer/MeshLoader.h"
#include "Renderer/Shader.h"
#include "Renderer/Texture.h"

namespace Renderer {
    class MasterRenderer {
    public:
        MasterRenderer();
        ~MasterRenderer();

        bool initialize();
        void render();
        void cleanup();

    private:
        TONGUE::Window window;
        std::shared_ptr<TONGUE::Mesh> mesh;
        std::shared_ptr<TONGUE::Mesh> loadedMesh;
        TONGUE::Shader shader;
        TONGUE::Texture texture;
        // Add other module members as needed
    };
}
