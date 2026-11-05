#include "aether/ui/ImGuiLayer.h"

#include "imgui.h"
#include "backends/imgui_impl_glfw.h"
#include "backends/imgui_impl_opengl3.h"

#include "aether/core/Log.h"

namespace aether::ui {

void ImGuiLayer::Init(GLFWwindow* window) {
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();

    ImGuiIO& io = ImGui::GetIO();
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;

    ImGui::StyleColorsDark();

    ImGuiStyle& style = ImGui::GetStyle();
    style.WindowRounding = 8.0f;
    style.FrameRounding  = 4.0f;
    style.GrabRounding   = 4.0f;

    if (!ImGui_ImplGlfw_InitForOpenGL(window, true)) {
        AE_CORE_ERROR("ImGui GLFW backend init failed");
        return;
    }
    if (!ImGui_ImplOpenGL3_Init("#version 330")) {
        AE_CORE_ERROR("ImGui OpenGL3 backend init failed");
        return;
    }

    AE_CORE_INFO("ImGui layer initialized");
}

void ImGuiLayer::Shutdown() {
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
    AE_CORE_INFO("ImGui layer shutdown");
}

void ImGuiLayer::BeginFrame() {
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();
}

void ImGuiLayer::EndFrame() {
    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

bool ImGuiLayer::WantCaptureMouse() {
    return ImGui::GetIO().WantCaptureMouse;
}

bool ImGuiLayer::WantCaptureKeyboard() {
    return ImGui::GetIO().WantCaptureKeyboard;
}

} // namespace aether::ui
