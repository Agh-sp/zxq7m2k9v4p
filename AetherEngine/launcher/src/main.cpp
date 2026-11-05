#include "aether/core/Application.h"
#include "aether/core/Log.h"
#include "aether/core/Window.h"
#include "aether/core/Input.h"
#include "aether/core/Time.h"
#include "aether/core/Settings.h"
#include "aether/ui/ImGuiLayer.h"

#include <GLFW/glfw3.h>
#include <GL/gl.h>
#include <imgui.h>

#include <string>
#include <vector>

namespace {
enum class LauncherState { Splash, MainMenu, Settings, InGame };
LauncherState g_State = LauncherState::Splash;
float g_SplashTimer = 0.0f;
const float SPLASH_DURATION = 2.0f;

int g_ResolutionIndex = 0;
bool g_Fullscreen = false;
bool g_VSync = true;
float g_MouseSens = 1.0f;

const std::vector<std::pair<int,int>> g_Resolutions = {
    {1280, 720}, {1920, 1080}, {2560, 1440}
};

void LoadSettingsIntoUI() {
    auto& s = aether::Settings::Get();
    auto w = s.value("window", nlohmann::json::object());
    auto i = s.value("input", nlohmann::json::object());
    g_Fullscreen = w.value("fullscreen", false);
    g_VSync = w.value("vsync", true);
    int width = w.value("width", 1280);
    int height = w.value("height", 720);
    g_ResolutionIndex = 0;
    for (size_t k = 0; k < g_Resolutions.size(); ++k) {
        if (g_Resolutions[k].first == width && g_Resolutions[k].second == height) {
            g_ResolutionIndex = (int)k; break;
        }
    }
    g_MouseSens = (float)i.value("mouseSensitivity", 1.0);
}

void ApplySettings(aether::Window& window) {
    auto [w, h] = g_Resolutions[g_ResolutionIndex];
    aether::Settings::Set("window", nlohmann::json{{"width",w},{"height",h},{"fullscreen",g_Fullscreen},{"vsync",g_VSync}});
    aether::Settings::Set("input", nlohmann::json{{"mouseSensitivity", g_MouseSens}});
    window.SetVSync(g_VSync);
    window.SetFullscreen(g_Fullscreen);
    if (!g_Fullscreen) window.SetSize((uint32_t)w, (uint32_t)h);
    aether::Settings::Save("settings.json");
    AE_INFO("Settings applied: {}x{} fs={} vsync={} sens={}", w, h, g_Fullscreen, g_VSync, g_MouseSens);
}
}

class LauncherApp : public aether::Application {
public:
    LauncherApp() : aether::Application("AetherEngine Launcher", 1280, 720, aether::ClientApi::OpenGL) {}

    void OnInit() override {
        AE_INFO("Launcher initialized");
        aether::Settings::Load("settings.json");
        LoadSettingsIntoUI();
        auto& win = GetWindow();
        auto& s = aether::Settings::Get();
        auto w = s.value("window", nlohmann::json::object());
        int width = w.value("width", 1280);
        int height = w.value("height", 720);
        bool fs = w.value("fullscreen", false);
        bool vs = w.value("vsync", true);
        win.SetVSync(vs);
        if (fs) win.SetFullscreen(true); else win.SetSize((uint32_t)width, (uint32_t)height);
        aether::ui::ImGuiLayer::Init(win.GetNativeWindow());
    }

    void OnUpdate(float dt) override {
        if (aether::Input::IsKeyPressed(GLFW_KEY_ESCAPE)) {
            if (g_State == LauncherState::InGame) g_State = LauncherState::MainMenu;
            else if (g_State == LauncherState::Settings) g_State = LauncherState::MainMenu;
            else if (g_State == LauncherState::MainMenu) Close();
        }
        if (g_State == LauncherState::Splash) {
            g_SplashTimer += dt;
            if (g_SplashTimer >= SPLASH_DURATION) g_State = LauncherState::MainMenu;
        }
    }

    void OnRender() override {
        auto& win = GetWindow();
        if (win.HasGLContext()) {
            glClearColor(0.08f, 0.09f, 0.12f, 1.0f);
            glClear(GL_COLOR_BUFFER_BIT);
        }
        aether::ui::ImGuiLayer::BeginFrame();
        switch (g_State) {
            case LauncherState::Splash:  RenderSplash(); break;
            case LauncherState::MainMenu: RenderMainMenu(); break;
            case LauncherState::Settings: RenderSettings(); break;
            case LauncherState::InGame:  RenderInGame(); break;
        }
        aether::ui::ImGuiLayer::EndFrame();
    }

    void OnShutdown() override {
        aether::ui::ImGuiLayer::Shutdown();
        aether::Settings::Save("settings.json");
        AE_INFO("Launcher shutdown");
    }


private:
    void RenderSplash() {
        ImGuiIO& io = ImGui::GetIO();
        ImGui::SetNextWindowPos(ImVec2(0,0));
        ImGui::SetNextWindowSize(io.DisplaySize);
        ImGui::Begin("##splash", nullptr, ImGuiWindowFlags_NoTitleBar|ImGuiWindowFlags_NoResize|ImGuiWindowFlags_NoMove|ImGuiWindowFlags_NoScrollbar|ImGuiWindowFlags_NoInputs|ImGuiWindowFlags_NoBackground);
        const char* title = "AETHER ENGINE";
        ImVec2 ts = ImGui::CalcTextSize(title);
        ImGui::SetCursorPos(ImVec2((io.DisplaySize.x-ts.x)*0.5f, (io.DisplaySize.y-ts.y)*0.5f - 20.0f));
        ImGui::TextColored(ImVec4(0.4f,0.7f,1.0f,1.0f), "%s", title);
        const char* sub = "Phase 1 - Launcher";
        ImVec2 ss = ImGui::CalcTextSize(sub);
        ImGui::SetCursorPos(ImVec2((io.DisplaySize.x-ss.x)*0.5f, (io.DisplaySize.y-ss.y)*0.5f + 20.0f));
        ImGui::TextColored(ImVec4(0.6f,0.6f,0.6f,1.0f), "%s", sub);
        float progress = g_SplashTimer / SPLASH_DURATION;
        ImGui::SetCursorPos(ImVec2(io.DisplaySize.x*0.35f, io.DisplaySize.y*0.7f));
        ImGui::ProgressBar(progress, ImVec2(io.DisplaySize.x*0.3f, 4.0f), "");
        ImGui::End();
    }

    void RenderMainMenu() {
        ImGuiIO& io = ImGui::GetIO();
        ImGui::SetNextWindowPos(ImVec2(0,0));
        ImGui::SetNextWindowSize(io.DisplaySize);
        ImGui::Begin("##mainmenu", nullptr, ImGuiWindowFlags_NoTitleBar|ImGuiWindowFlags_NoResize|ImGuiWindowFlags_NoMove|ImGuiWindowFlags_NoScrollbar|ImGuiWindowFlags_NoCollapse|ImGuiWindowFlags_NoBackground);
        const char* title = "AETHER ENGINE";
        ImVec2 ts = ImGui::CalcTextSize(title);
        ImGui::SetCursorPos(ImVec2((io.DisplaySize.x-ts.x)*0.5f, 80.0f));
        ImGui::TextColored(ImVec4(0.4f,0.7f,1.0f,1.0f), "%s", title);
        const char* sub = "Main Menu";
        ImVec2 ss = ImGui::CalcTextSize(sub);
        ImGui::SetCursorPos(ImVec2((io.DisplaySize.x-ss.x)*0.5f, 120.0f));
        ImGui::TextColored(ImVec4(0.6f,0.6f,0.6f,1.0f), "%s", sub);

        float btnW = 260.0f, btnH = 44.0f;
        float x = (io.DisplaySize.x - btnW)*0.5f;
        float y = 220.0f, sp = 16.0f;
        ImGui::SetCursorPos(ImVec2(x, y));
        if (ImGui::Button("Start Game", ImVec2(btnW, btnH))) { g_State = LauncherState::InGame; AE_INFO("Start Game pressed"); }
        ImGui::SetCursorPos(ImVec2(x, y + (btnH+sp)));
        if (ImGui::Button("Settings", ImVec2(btnW, btnH))) { LoadSettingsIntoUI(); g_State = LauncherState::Settings; }
        ImGui::SetCursorPos(ImVec2(x, y + (btnH+sp)*2));
        if (ImGui::Button("Exit", ImVec2(btnW, btnH))) Close();

        const char* footer = "AetherEngine v0.1 - Phase 1";
        ImVec2 fs = ImGui::CalcTextSize(footer);
        ImGui::SetCursorPos(ImVec2((io.DisplaySize.x-fs.x)*0.5f, io.DisplaySize.y - 30.0f));
        ImGui::TextColored(ImVec4(0.4f,0.4f,0.4f,1.0f), "%s", footer);
        ImGui::End();
    }

    void RenderSettings() {
        ImGuiIO& io = ImGui::GetIO();
        ImGui::SetNextWindowPos(ImVec2(io.DisplaySize.x*0.5f, io.DisplaySize.y*0.5f), ImGuiCond_Always, ImVec2(0.5f,0.5f));
        ImGui::SetNextWindowSize(ImVec2(460, 420));
        ImGui::Begin("Settings", nullptr, ImGuiWindowFlags_NoCollapse);
        ImGui::Text("Display"); ImGui::Separator();
        const char* resItems[] = { "1280 x 720", "1920 x 1080", "2560 x 1440" };
        ImGui::Combo("Resolution", &g_ResolutionIndex, resItems, IM_ARRAYSIZE(resItems));
        ImGui::Checkbox("Fullscreen", &g_Fullscreen);
        ImGui::Checkbox("VSync", &g_VSync);
        ImGui::Spacing();
        ImGui::Text("Input"); ImGui::Separator();
        ImGui::SliderFloat("Mouse Sensitivity", &g_MouseSens, 0.1f, 5.0f, "%.2f");
        ImGui::Spacing(); ImGui::Separator();
        if (ImGui::Button("Apply", ImVec2(120, 32))) ApplySettings(GetWindow());
        ImGui::SameLine();
        if (ImGui::Button("Back", ImVec2(120, 32))) g_State = LauncherState::MainMenu;
        ImGui::End();
    }

    void RenderInGame() {
        ImGui::SetNextWindowPos(ImVec2(20,20));
        ImGui::Begin("Game", nullptr, ImGuiWindowFlags_AlwaysAutoResize);
        ImGui::TextColored(ImVec4(0.4f,1.0f,0.4f,1.0f), "Game Running (placeholder)");
        ImGui::Text("Press ESC to return to menu");
        ImGui::Text("FPS: %.1f", aether::Time::GetFPS());
        ImGui::End();
    }
};

aether::Application* aether::CreateApplication() { return new LauncherApp(); }

int main(int /*argc*/, char** /*argv*/) {
    aether::Log::Init();
    aether::Application* app = aether::CreateApplication();
    if (!app) { aether::Log::Shutdown(); return -1; }
    app->Run();
    delete app;
    aether::Log::Shutdown();
    return 0;
}

