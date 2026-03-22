#ifndef VULCRAFT_ENGINE_H
#define VULCRAFT_ENGINE_H

#if _WIN32
#define VK_USE_PLATFORM_WIN32_KHR
#endif //#IF WIN32

#include "Managers/WindowManager/WindowManager.h"
#include "Managers/VulkanManager/VulkanManager.h"
#include "Managers/ImGuiManager/ImGuiManager.h"

#include "glm/gtc/matrix_transform.hpp"

class Engine {

    public:
        void Run();

    private:

        /* ENGINE FUNCTIONS */
        void MainLoop(WindowManager* windowManager, VulkanManager* vulkanManager, ImGuiManager* imGuiManager);
        void Cleanup(WindowManager* windowManager, VulkanManager* vulkanManager, ImGuiManager* imGuiManager);

        /* GLFW FUNCTIONS */
        void InitGLFW();

};


#endif //VULCRAFT_ENGINE_H