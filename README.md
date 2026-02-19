T.O.N.G.U.E Renderer

Graphics module of the T.O.N.G.U.E Engine

The TONGUE-Renderer repository handles all graphics-related functionality for the T.O.N.G.U.E system. It depends on TONGUE-Core for shared data types, math utilities, and entity abstractions but remains isolated from server or input logic.

Responsibilities

TONGUE-Renderer is designed to provide:

Rendering Pipeline – Abstractions for rendering 2D and 3D content, frame management, and GPU interfacing.

Asset Management – Loading and managing textures, models, shaders, and materials.

Scene & Entity Rendering – High-level functions to draw characters, environment objects, and UI elements.

Integration with Core – Uses Core types like Vector3, Color, Entity, and Character to ensure consistency.

Project Layout
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

Usage
#include <Renderer.h>
#include <Tongue.h>  // Core types

Renderer renderer;
renderer.Init(1280, 720);

Character player;
renderer.DrawCharacter(player);
renderer.Present();


All rendering should go through Renderer to isolate graphics logic from the rest of the engine.

Guidelines

Use Core types – Avoid duplicating vectors, colors, or entity definitions.

Keep it modular – Rendering features should not leak Core or Server logic.

Abstract the API – Prepare for swapping graphics APIs (OpenGL, Vulkan, DirectX).

Document & Test – Provide clear documentation for drawing functions, and unit tests for critical rendering components.

Contribution

Only implement graphics, scene management, and asset handling.

Do not include input handling, network code, or server logic.

Follow the naming and modularization conventions to maintain a clean separation from Core and other modules.