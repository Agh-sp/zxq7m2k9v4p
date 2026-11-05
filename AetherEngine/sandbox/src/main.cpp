#include "aether/core/Application.h"
#include "aether/core/Log.h"
#include "aether/core/Input.h"
#include "aether/core/Time.h"
#include "aether/core/Settings.h"

#include <GLFW/glfw3.h>

#include <chrono>
#include <thread>

class SandboxApp : public aether::Application {
public:
    SandboxApp()
        : aether::Application("AetherEngine - Sandbox (Phase 0)", 1280, 720) {}

    void OnInit() override {
        AE_INFO("Sandbox initialized");

        aether::Settings::Load("settings.json");
        AE_INFO("Settings: {}", aether::Settings::Get().dump());

        // Hide cursor for FPS-style testing (optional)
        // aether::Input::SetCursorMode(GLFW_CURSOR_DISABLED);
    }

    void OnUpdate(float dt) override {
        // Demo input: ESC to close
        if (aether::Input::IsKeyPressed(GLFW_KEY_ESCAPE)) {
            Close();
        }

        // Demo: print FPS every ~1s
        static float accum = 0.0f;
        accum += dt;
        if (accum >= 1.0f) {
            AE_INFO("FPS: {:.1f} | dt: {:.4f}s | time: {:.2f}s",
                    aether::Time::GetFPS(), dt, aether::Time::GetTime());
            accum = 0.0f;
        }

        // Demo: WASD movement input logging (just to verify input polling)
        if (aether::Input::IsKeyPressed(GLFW_KEY_W)) AE_TRACE("W pressed");
        if (aether::Input::IsKeyPressed(GLFW_KEY_A)) AE_TRACE("A pressed");
        if (aether::Input::IsKeyPressed(GLFW_KEY_S)) AE_TRACE("S pressed");
        if (aether::Input::IsKeyPressed(GLFW_KEY_D)) AE_TRACE("D pressed");
    }

    void OnRender() override {
        // Phase 0: no rendering yet (Vulkan comes in Phase 2).
    }

    void OnShutdown() override {
        aether::Settings::Save("settings.json");
        AE_INFO("Sandbox shutdown");
    }
};

aether::Application* aether::CreateApplication() {
    return new SandboxApp();
}

int main(int /*argc*/, char** /*argv*/) {
    aether::Log::Init();

    aether::Application* app = aether::CreateApplication();
    if (!app) {
        aether::Log::Shutdown();
        return -1;
    }

    app->Run();

    delete app;
    aether::Log::Shutdown();
    return 0;
}
