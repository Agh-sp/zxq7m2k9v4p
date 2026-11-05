#include "aether/core/Input.h"
#include "aether/core/Log.h"

namespace aether {

GLFWwindow* Input::s_Window     = nullptr;
double      Input::s_LastX      = 0.0;
double      Input::s_LastY      = 0.0;
double      Input::s_DeltaX     = 0.0;
double      Input::s_DeltaY     = 0.0;
bool        Input::s_FirstMouse = true;

void Input::Init(GLFWwindow* window) {
    s_Window = window;
    s_FirstMouse = true;
    s_DeltaX = s_DeltaY = 0.0;

    glfwSetCursorPosCallback(window, [](GLFWwindow* /*w*/, double xpos, double ypos) {
        if (s_FirstMouse) {
            s_LastX = xpos;
            s_LastY = ypos;
            s_FirstMouse = false;
        }
        s_DeltaX += xpos - s_LastX;
        s_DeltaY += ypos - s_LastY;
        s_LastX = xpos;
        s_LastY = ypos;
    });

    AE_CORE_INFO("Input system initialized");
}

void Input::Update() {
    // Deltas accumulate via callback; nothing to do here per frame.
}

void Input::Shutdown() {
    s_Window = nullptr;
    s_FirstMouse = true;
    s_DeltaX = s_DeltaY = 0.0;
    AE_CORE_INFO("Input system shutdown");
}

bool Input::IsKeyPressed(int key) {
    if (!s_Window) return false;
    return glfwGetKey(s_Window, key) == GLFW_PRESS;
}

bool Input::IsKeyReleased(int key) {
    if (!s_Window) return false;
    return glfwGetKey(s_Window, key) == GLFW_RELEASE;
}

bool Input::IsMouseButtonPressed(int button) {
    if (!s_Window) return false;
    return glfwGetMouseButton(s_Window, button) == GLFW_PRESS;
}

std::pair<double, double> Input::GetMousePosition() {
    double x = 0.0, y = 0.0;
    if (s_Window) glfwGetCursorPos(s_Window, &x, &y);
    return { x, y };
}

std::pair<double, double> Input::GetMouseDelta() {
    auto delta = std::make_pair(s_DeltaX, s_DeltaY);
    s_DeltaX = 0.0;
    s_DeltaY = 0.0;
    return delta;
}

void Input::SetCursorMode(int mode) {
    if (s_Window) glfwSetInputMode(s_Window, GLFW_CURSOR, mode);
}

} // namespace aether
