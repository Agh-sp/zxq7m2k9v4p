# Blender Asset Support Roadmap (AssaultCube Overhaul)

Goal: Accept Blender-exported assets (maps, characters, guns, animations) and overhaul UI.

## Phase 0: Asset format decision
- Use **glTF 2.0** (.gltf/.glb) as runtime format (exported from Blender).
- Keep legacy MD2/MD3 for compatibility.

## Phase 1: Static model support (guns/props)
- Integrate cgltf (done: source/include/cgltf.h).
- Implement glTF loader in source/src/gltfmodel.cpp.
- Register new model type in rendermodel.cpp (loadmodel dispatch).
- Load textures from glTF.
- Render static meshes.

## Phase 2: Skeletal animation (characters/guns)
- Parse skins/joints/animations.
- Add skinning to render pipeline.
- Map animation clips to engine anim states.

## Phase 3: Map support
- Option A: Convert glTF scene to AssaultCube octree map offline.
- Option B: Render glTF scene directly as a "model map" (no octree collision).
- Collision/physics integration.

## Phase 4: Menu/UI overhaul
- Rewrite config/menus*.cfg.
- New HUD, new game modes (Comp).

## Phase 5: Build & test
- Update Makefile/vcxproj.
- Validate on Windows.

## Current status
- cgltf integrated.
- Maps backed up, single map (ac_dust2 placeholder).
- Gamemode menu simplified to Comp Mode.
PY