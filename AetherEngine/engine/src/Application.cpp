#include "aether/core/Application.h"
#include "aether/core/Log.h"

namespace aether {

Application* Application::s_Instance = nullptr;

Application::Application(const std::string& name, uint32_t width, uint32_t height, ClientApi clientApi) {
    AE_CORE_ASSERT(s_Instance == nullptr, "Application already exists");
    s_Instance = this;

    WindowProps props;
    props.title = name;
    props.width = width;
    props.height = height;
    props.clientApi = clientApi;
    m_Window = std::make_unique<Window>(props);
    AE_CORE_ASSERT(m_Window != nullptr && m_Window->GetNativeWindow() != nullptr,
                   "Failed to create window");

    Input::Init(m_Window->GetNativeWindow());
    Time::Init();

    AE_CORE_INFO("Application '{}' created ({}x{})", name, width, height);
}

Application::~Application() {
    OnShutdown();
    Input::Shutdown();
    m_Window.reset();
    s_Instance = nullptr;
    AE_CORE_INFO("Application destroyed");
}

void Application::Run() {
    OnInit();
    AE_CORE_INFO("Entering main loop");

    while (m_Running && m_Window && !m_Window->ShouldClose()) {
        Time::Update();

        float dt = Time::GetDeltaTime();
        if (dt > 0.1f) dt = 0.1f; // clamp to avoid spiral of death

        m_Window->PollEvents();
        Input::Update();

        OnUpdate(dt);
        OnRender();

        m_Window->SwapBuffers();
    }

    AE_CORE_INFO("Exiting main loop");
}

} // namespace aether
