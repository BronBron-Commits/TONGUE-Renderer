
# TONGUE-Renderer

TONGUE-Renderer is the graphics backend of the T.O.N.G.U.E engine.  
It is responsible for GPU interaction, rendering pipelines, and visual asset handling while remaining fully decoupled from gameplay, networking, and input logic.

## Architectural Rules

- Renderer depends only on TONGUE-Core and external graphics libraries
- Renderer must not depend on Client, Server, or gameplay logic
- Renderer accepts Core data structures but never mutates them
- Rendering state must remain internal to the module


## Responsibilities

TONGUE-Renderer is designed to provide:

- Rendering Pipeline – Frame lifecycle, draw submission, batching, and GPU resource management.
- Asset Management – Loading and managing textures, models, shaders, and materials.
- Scene & Entity Rendering – Translation layer that converts Core entities into renderable data.
- Resource Lifetime Management – Handles creation, reuse, and destruction of GPU resources to prevent leaks and ensure performance stability.
- Integration with Core – Uses Core types like Vector3, Color, Entity, and Character to ensure consistency.


## Project Layout
TONGUE-Renderer/
├─ include/
│  ├─ Renderer.h        # Main interface for rendering
│  ├─ Renderer/         # Rendering subsystem headers
│  │  ├─ Window.h       # Window and context management
│  │  ├─ Texture.h      # Texture abstraction
│  │  ├─ Mesh.h         # Mesh & model representation
│  │  └─ Shader.h       # Shader management
├─ src/                 # Implementation files
├─ assets/              # Sample textures, shaders, and models
├─ tests/               # Unit tests and rendering validation
└─ CMakeLists.txt       # Build configuration


## Usage
```cpp
#include <Renderer.h>
#include <Tongue.h>

Renderer renderer;
renderer.Init(1280, 720);

Character player;
renderer.Submit(player);   // enqueue renderable data
renderer.RenderFrame();    // execute draw calls
```

All rendering should go through Renderer to isolate graphics logic from the rest of the engine.


## Build Expectations

TONGUE-Renderer builds as a static library and does not produce an executable.

All dependencies (OpenGL, GLEW, etc.) are managed internally by the module.

Frontend applications (Sandbox, PlayerViewer, Client) should only link against TongueRenderer and never directly against graphics libraries.

## Guidelines

- Use Core types – Avoid duplicating vectors, colors, or entity definitions.
- Keep it modular – Rendering features should not leak Core or Server logic.
- Abstract the API – All graphics API calls must be wrapped so that higher-level code remains API-agnostic.
- Document & Test – Provide clear documentation for drawing functions, and unit tests for critical rendering components.


## Contribution

Only implement graphics, scene management, and asset handling.
Do not include input handling, network code, or server logic.
Follow the naming and modularization conventions to maintain a clean separation from Core and other modules.

---

TONGUE-Renderer prioritizes stability, determinism, and clear ownership of GPU resources over convenience abstractions.