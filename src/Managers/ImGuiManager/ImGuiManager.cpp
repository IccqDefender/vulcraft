
#include "ImGuiManager.h"

ImGuiManager::~ImGuiManager() {
    CloseImGui();
}

void ImGuiManager::InitImGui(WindowManager *windowManager) {
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();

    ImGuiIO& io = ImGui::GetIO();
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;

    ImGui::StyleColorsDark();

    ImGui_ImplGlfw_InitForVulkan(windowManager->window, true);

    std::cout << "[IMGUI]: Initialized!" << std::endl;
}

void ImGuiManager::RenderImGui() {
    ImGui_ImplVulkan_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();

    ImGui::ShowDemoWindow();

    {
        ImGui::Begin("Colors");

        ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);
        ImGui::ColorEdit3("Background colors", (float*)&clear_color);

        ImVec4 color_picker = ImVec4(0.2f, 0.5f, 0.8f, 1.0f);
        ImGui::ColorPicker4("Color pick", (float*)&color_picker);

        ImGui::End();
    }
}

void ImGuiManager::CloseImGui() {
    ImGui_ImplVulkan_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
    std::cout << "[IMGUI]: Closed!" << std::endl;
}
