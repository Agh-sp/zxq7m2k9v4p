#pragma once

#include <GLFW/glfw3.h>
#include <cstdint>
#include <string>
#include <functional>

namespace aether {

enum class ClientApi {
    None,   // No graphics API (Vulkan will provide its own)
    OpenGL  // OpenGL 3.3 Core (used by ImGui launcher in Phase 1)
};

struct WindowProps {
    std::string title = "AetherEngine";
    uint32_t    width = 1280;
    uint32_t    height = 720;
    bool        vsync = true;
    bool        fullscreen = false;
    ClientApi   clientApi = ClientApi::None;
};

class Window {
public:
    explicit Window(const WindowProps& props = {});
    ~Window();

    Window(const Window&) = delete;
    Window& operator=(const Window&) = delete;

    void PollEvents();
    void SwapBuffers();
    bool ShouldClose() const;
    void Close();

    uint32_t GetWidth() const  { return m_Data.width; }
    uint32_t GetHeight() const { return m_Data.height; }
    float    GetAspectRatio() const {
        return static_cast<float>(m_Data.width) / static_cast<float>(m_Data.height);
    }
    GLFWwindow* GetNativeWindow() const { return m_Window; }
    bool HasGLContext() const { return m_HasGLContext; }

    void SetVSync(bool enabled);
    bool IsVSync() const { return m_Data.vsync; }
    void SetTitle(const std::string& title);
    void SetSize(uint32_t width, uint32_t height);
    void SetFullscreen(bool enabled);

    using ResizeCallback = std::function<void(uint32_t, uint32_t)>;
    void SetResizeCallback(ResizeCallback cb) { m_Data.resizeCallback = std::move(cb); }

private:
    void Init(const WindowProps& props);
    void Shutdown();

    GLFWwindow* m_Window = nullptr;
    bool        m_HasGLContext = false;

    struct WindowData {
        std::string     title;
        uint32_t        width = 0;
        uint32_t        height = 0;
        bool            vsync = true;
        bool            fullscreen = false;
        ClientApi       clientApi = ClientApi::None;
        ResizeCallback  resizeCallback;
    } m_Data{};
};

} // namespace aether
