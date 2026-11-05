# AetherEngine

A modern, from-scratch game engine targeting Vulkan rendering, data-oriented ECS,
PBR, glTF 2.0, Jolt Physics, and large open-world maps. Built in C++23.

## Phase 0 - Project setup & core

This phase delivers:
- Professional CMake build (Debug/Release, multi-config)
- spdlog-based logging
- JSON settings
- GLFW window + input
- Main engine loop
- Time / DeltaTime system

## Build (Windows, Visual Studio 2022/2026)

```powershell
cd AetherEngine
cmake -S . -B build -G "Visual Studio 17 2022" -A x64
cmake --build build --config Debug
.\build\sandbox\Debug\aether_sandbox.exe
```

## Build (Linux / GCC)

```bash
cd AetherEngine
cmake -S . -B build -DCMAKE_BUILD_TYPE=Debug
cmake --build build -j
./build/sandbox/aether_sandbox
```
