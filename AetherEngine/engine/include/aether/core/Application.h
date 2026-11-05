#pragma once

#include "aether/core/Window.h"
#include "aether/core/Input.h"
#include "aether/core/Time.h"

#include <cstdint>
#include <memory>
#include <string>

namespace aether {

class Application {
public:
    explicit Application(const std::string& name = "AetherEngine",
                        uint32_t width  = 1280,
                        uint32_t height = 720,
                        ClientApi clientApi = ClientApi::None);
    virtual ~Application();

    Application(const Application&) = delete;
    Application& operator=(const Application&) = delete;

    void Run();
    void Close() { m_Running = false; }

    Window& GetWindow() { return *m_Window; }

    static Application& Get() { return *s_Instance; }
    static bool IsRunning() { return s_Instance != nullptr && s_Instance->m_Running; }

protected:
    virtual void OnInit()     {}
    virtual void OnUpdate(float dt) { (void)dt; }
    virtual void OnRender()   {}
    virtual void OnShutdown() {}

private:
    static Application* s_Instance;

    std::unique_ptr<Window> m_Window;
    bool m_Running = true;
};

// Implemented by the client (sandbox / game).
Application* CreateApplication();

} // namespace aether
