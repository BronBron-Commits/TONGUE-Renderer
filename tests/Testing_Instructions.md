# TONGUE Renderer Bring-Up & Testing Instructions

## 1. Renderer Bring-Up Test: `TriangleStressTest.cpp`

**Goal:**
Prove the renderer can open a window, initialize the GPU, render a triangle, apply shader effects, sustain high FPS, and allow visual tuning before scene logic.

---

## 2. Test File

Create:
`tests/TriangleStressTest.cpp`

**Dependencies:** Only Renderer, Mesh, Shader, (optional: Texture). No gameplay or entity code.

---

## 3. Triangle Test Requirements

**Stage 1:**
- Render 1 triangle
- Vertex color interpolation
- Clear color background

**Stage 2:**
- Smooth vertex color gradient (proves interpolation)
- Gamma correction (prevents washed-out colors)
- Time-based animation (`uTime` uniform: pulse brightness, rotate colors, wobble UVs)
- Basic lighting simulation (fake normal + light: `float light = dot(normal, lightDir); color *= light * 0.5 + 0.5;`)
- Subpixel edge smoothing (`float edge = smoothstep(0.0, 0.01, barycentricCoord);` for anti-aliased edges)

---

## 4. Stress Tests

**Mode 1: Triangle Flood**
- Draw 10, 100, 1,000, 10,000, 100,000 triangles (use instancing if available)
- Track frame time, draw call count, GPU/CPU bottleneck

**Mode 2: Shader Complexity Ramp**
- Runtime keys:
  1 = flat shader
  2 = gradient shader
  3 = animated shader
  4 = lighting
  5 = heavy math shader
  6 = fake PBR triangle
- Prove shader hot-swap and pipeline resilience

**Mode 3: Resolution Scaling**
- Test at: 800×600, 1280×720, 1920×1080, 2560×1440, 4K
- Renderer must not break or distort

---

## 5. Triangle Mesh

Hardcode this mesh:
```
(-0.6, -0.4, 0)
( 0.6, -0.4, 0)
( 0.0,  0.6, 0)
```
Each vertex: position, color, normal, uv (even if unused).

---

## 6. Shader Uniforms

Minimum:
`uProjection`, `uView`, `uModel`, `uTime`, `uResolution`
(Even for a triangle.)

---

## 7. Performance Metrics

Print every second:
- FPS
- Frame time
- Draw calls
- Triangles rendered
- GPU frame time (if possible)

---

## 8. Exit Criteria

Do NOT build scenes until:
- Triangle renders correctly
- Shader hot-reload works
- Renderer survives 100k triangles
- FPS telemetry works
- Window resizing works
- VSync toggle works
- Gamma correct output verified
- No memory leaks after 10 min run

---

**Summary:**
This test ensures the renderer is robust, future-proof, and ready for real content. No scene logic or gameplay until all bring-up criteria are met.
