#pragma once

#include <GLFW/glfw3.h>
#include <utility>

namespace aether {

class Input {
public:
    static void Init(GLFWwindow* window);
    static void Update();
    static void Shutdown();

    static bool IsKeyPressed(int key);
    static bool IsKeyReleased(int key);
    static bool IsMouseButtonPressed(int button);

    static std::pair<double, double> GetMousePosition();
    static std::pair<double, double> GetMouseDelta();

    static void SetCursorMode(int mode);

private:
    static GLFWwindow* s_Window;
    static double      s_LastX;
    static double      s_LastY;
    static double      s_DeltaX;
    static double      s_DeltaY;
    static bool        s_FirstMouse;
};

} // namespace aether
