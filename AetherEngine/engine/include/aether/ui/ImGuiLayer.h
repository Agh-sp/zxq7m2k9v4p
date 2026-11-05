#pragma once

#include <GLFW/glfw3.h>

namespace aether::ui {

class ImGuiLayer {
public:
    static void Init(GLFWwindow* window);
    static void Shutdown();

    static void BeginFrame();
    static void EndFrame();

    static bool WantCaptureMouse();
    static bool WantCaptureKeyboard();
};

} // namespace aether::ui
