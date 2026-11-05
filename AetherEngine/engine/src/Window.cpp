#include "aether/core/Window.h"
#include "aether/core/Log.h"

namespace aether {

static int s_GLFWRefCount = 0;

Window::Window(const WindowProps& props) {
    Init(props);
}

Window::~Window() {
    Shutdown();
}

void Window::Init(const WindowProps& props) {
    m_Data.title      = props.title;
    m_Data.width      = props.width;
    m_Data.height     = props.height;
    m_Data.vsync      = props.vsync;
    m_Data.fullscreen = props.fullscreen;
    m_Data.clientApi  = props.clientApi;

    if (s_GLFWRefCount == 0) {
        if (!glfwInit()) {
            AE_CORE_CRITICAL("Failed to initialize GLFW");
            return;
        }
        AE_CORE_INFO("GLFW initialized");
    }
    ++s_GLFWRefCount;

    if (props.clientApi == ClientApi::OpenGL) {
        glfwWindowHint(GLFW_CLIENT_API, GLFW_OPENGL_API);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
        glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GLFW_TRUE);
        m_HasGLContext = true;
    } else {
        glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
        m_HasGLContext = false;
    }
    glfwWindowHint(GLFW_RESIZABLE, GLFW_TRUE);

    m_Window = glfwCreateWindow(
        static_cast<int>(m_Data.width),
        static_cast<int>(m_Data.height),
        m_Data.title.c_str(),
        props.fullscreen ? glfwGetPrimaryMonitor() : nullptr,
        nullptr);

    if (!m_Window) {
        AE_CORE_CRITICAL("Failed to create GLFW window");
        return;
    }

    if (m_HasGLContext) {
        glfwMakeContextCurrent(m_Window);
        glfwSwapInterval(props.vsync ? 1 : 0);
    }

    glfwSetWindowUserPointer(m_Window, &m_Data);

    glfwSetWindowSizeCallback(m_Window, [](GLFWwindow* w, int width, int height) {
        auto* data = static_cast<WindowData*>(glfwGetWindowUserPointer(w));
        if (!data) return;
        data->width  = static_cast<uint32_t>(width);
        data->height = static_cast<uint32_t>(height);
        if (data->resizeCallback) {
            data->resizeCallback(data->width, data->height);
        }
    });

    glfwSetWindowCloseCallback(m_Window, [](GLFWwindow* /*w*/) {
        AE_CORE_INFO("Window close requested");
    });

    AE_CORE_INFO("Window created: {}x{} '{}' (GL={})", m_Data.width, m_Data.height, m_Data.title, m_HasGLContext);
}

void Window::Shutdown() {
    if (m_Window) {
        glfwDestroyWindow(m_Window);
        m_Window = nullptr;
    }
    --s_GLFWRefCount;
    if (s_GLFWRefCount <= 0) {
        s_GLFWRefCount = 0;
        glfwTerminate();
        AE_CORE_INFO("GLFW terminated");
    }
}

void Window::PollEvents() { glfwPollEvents(); }

void Window::SwapBuffers() {
    if (m_Window && m_HasGLContext) {
        glfwSwapBuffers(m_Window);
    }
}

bool Window::ShouldClose() const { return m_Window && glfwWindowShouldClose(m_Window); }
void Window::Close() { if (m_Window) glfwSetWindowShouldClose(m_Window, GLFW_TRUE); }

void Window::SetVSync(bool enabled) {
    m_Data.vsync = enabled;
    if (m_Window && m_HasGLContext) {
        glfwSwapInterval(enabled ? 1 : 0);
    }
}

void Window::SetTitle(const std::string& title) {
    m_Data.title = title;
    if (m_Window) glfwSetWindowTitle(m_Window, title.c_str());
}

void Window::SetSize(uint32_t width, uint32_t height) {
    m_Data.width = width;
    m_Data.height = height;
    if (m_Window) glfwSetWindowSize(m_Window, static_cast<int>(width), static_cast<int>(height));
}

void Window::SetFullscreen(bool enabled) {
    m_Data.fullscreen = enabled;
    if (!m_Window) return;
    if (enabled) {
        GLFWmonitor* monitor = glfwGetPrimaryMonitor();
        const GLFWvidmode* mode = glfwGetVideoMode(monitor);
        glfwSetWindowMonitor(m_Window, monitor, 0, 0, mode->width, mode->height, mode->refreshRate);
        m_Data.width = static_cast<uint32_t>(mode->width);
        m_Data.height = static_cast<uint32_t>(mode->height);
    } else {
        glfwSetWindowMonitor(m_Window, nullptr, 100, 100,
                             static_cast<int>(m_Data.width), static_cast<int>(m_Data.height), 0);
    }
}

} // namespace aether
