cmake_minimum_required(VERSION 3.20)

project(AetherEngine
    VERSION 0.1.0
    DESCRIPTION "AetherEngine - Modern Vulkan game engine (Phase 0)"
    LANGUAGES CXX
)

# ---- Global C++ settings ----
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

if(MSVC)
    add_compile_options(/W4 /permissive- /Zc:__cplusplus /utf-8 /EHsc)
    add_compile_definitions(_CRT_SECURE_NO_WARNINGS)
else()
    add_compile_options(-Wall -Wextra -Wpedantic -Wno-unused-parameter)
endif()

# ---- Options ----
option(AETHER_BUILD_LAUNCHER "Build the launcher application" ON)
option(AETHER_BUILD_SANDBOX  "Build the sandbox/test application" ON)
option(AETHER_BUILD_TESTS   "Build unit tests" OFF)

# ---- Third-party dependencies (local) ----
set(THIRD_PARTY_DIR ${CMAKE_CURRENT_SOURCE_DIR}/third_party)

# GLFW
set(GLFW_BUILD_DOCS     OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS    OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
set(GLFW_INSTALL        OFF CACHE BOOL "" FORCE)
add_subdirectory(${THIRD_PARTY_DIR}/glfw ${CMAKE_BINARY_DIR}/_deps/glfw-build EXCLUDE_FROM_ALL)

# GLM (header-only)
add_subdirectory(${THIRD_PARTY_DIR}/glm ${CMAKE_BINARY_DIR}/_deps/glm-build EXCLUDE_FROM_ALL)

# spdlog
set(SPDLOG_BUILD_SHARED OFF CACHE BOOL "" FORCE)
set(SPDLOG_BUILD_EXAMPLE OFF CACHE BOOL "" FORCE)
set(SPDLOG_BUILD_TESTS OFF CACHE BOOL "" FORCE)
add_subdirectory(${THIRD_PARTY_DIR}/spdlog ${CMAKE_BINARY_DIR}/_deps/spdlog-build EXCLUDE_FROM_ALL)

# nlohmann/json (header-only)
set(JSON_BuildTests OFF CACHE BOOL "" FORCE)
add_subdirectory(${THIRD_PARTY_DIR}/nlohmann_json ${CMAKE_BINARY_DIR}/_deps/nlohmann_json-build EXCLUDE_FROM_ALL)

# ---- Subdirectories ----
add_subdirectory(engine)

if(AETHER_BUILD_LAUNCHER)
    add_subdirectory(launcher)
endif()

if(AETHER_BUILD_SANDBOX)
    add_subdirectory(sandbox)
endif()
