
#ifndef VULCRAFT_IMGUIMANAGER_H
#define VULCRAFT_IMGUIMANAGER_H


#include <iostream>
#include <ostream>

#include "../WindowManager/WindowManager.h"

#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_vulkan.h>

class ImGuiManager {
public:
    ~ImGuiManager();

    void InitImGui(WindowManager* windowManager);

    void RenderImGui();
    void CloseImGui();
};


#endif //VULCRAFT_IMGUIMANAGER_H