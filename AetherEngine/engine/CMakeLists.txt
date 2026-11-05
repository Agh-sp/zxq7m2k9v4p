add_library(aether_engine STATIC
    src/Log.cpp
    src/Window.cpp
    src/Input.cpp
    src/Time.cpp
    src/Application.cpp
    src/Settings.cpp
    src/ui/ImGuiLayer.cpp

    # ImGui core
    ${CMAKE_CURRENT_SOURCE_DIR}/../third_party/imgui/imgui.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/../third_party/imgui/imgui_draw.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/../third_party/imgui/imgui_tables.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/../third_party/imgui/imgui_widgets.cpp

    # ImGui backends
    ${CMAKE_CURRENT_SOURCE_DIR}/../third_party/imgui/backends/imgui_impl_glfw.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/../third_party/imgui/backends/imgui_impl_opengl3.cpp
)

add_library(Aether::Engine ALIAS aether_engine)

target_include_directories(aether_engine
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/include
        ${CMAKE_CURRENT_SOURCE_DIR}/../third_party/imgui
        ${CMAKE_CURRENT_SOURCE_DIR}/../third_party/imgui/backends
)

target_link_libraries(aether_engine
    PUBLIC
        glfw
        glm::glm
        spdlog::spdlog
        nlohmann_json::nlohmann_json
        opengl32
)

if(UNIX AND NOT APPLE)
    target_link_libraries(aether_engine PUBLIC pthread dl GL)
endif()

set_target_properties(aether_engine PROPERTIES
    CXX_STANDARD 23
    CXX_STANDARD_REQUIRED ON
    CXX_EXTENSIONS OFF
)
