#pragma once
#include <memory>

#pragma once
#include <memory>

namespace TONGUE {
    class Window;
    class Mesh;
    class Texture;
    class Character;
    struct Vector3;

    class Renderer {
    public:
        bool Init(int width, int height);
        void Shutdown();

        void BeginFrame();
        void EndFrame();
        void Present();

        void DrawMesh(const Mesh& mesh);
        void DrawTexture(const Texture& texture, const Vector3& pos);
        void DrawCharacter(const Character& character);

    private:
        std::unique_ptr<Window> window;
    };
}
