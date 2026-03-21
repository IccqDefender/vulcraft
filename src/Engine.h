#ifndef VULCRAFT_ENGINE_H
#define VULCRAFT_ENGINE_H

#if _WIN32
#define VK_USE_PLATFORM_WIN32_KHR
#endif //#IF WIN32

#include <vulkan/vulkan.h>
#include "Managers/WindowManager/WindowManager.h"

class Engine {

    public:
        void Run();

    private:
        void MainLoop(WindowManager* windowManager);
        void Cleanup(WindowManager* windowManager);

        void InitGLFW();

        void InitVulkan();
        void VulkanInstantiate();

        void CreateSurface(WindowManager* windowManager);

        VkInstance m_instance;
        VkSurfaceKHR m_surface;

};


#endif //VULCRAFT_ENGINE_H