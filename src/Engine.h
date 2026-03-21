#ifndef VULCRAFT_ENGINE_H
#define VULCRAFT_ENGINE_H

#if _WIN32
#define VK_USE_PLATFORM_WIN32_KHR
#endif //#IF WIN32

#include <vulkan/vulkan.h>
#include "Managers/WindowManager/WindowManager.h"

#include <vector>
#include <iostream>

class Engine {

    public:
        void Run();

    private:
        VkInstance m_instance = nullptr;
        VkSurfaceKHR m_surface = nullptr;
        VkPhysicalDevice m_physicalDevice = nullptr;
        VkDevice m_device = nullptr;
        VkQueue m_graphicsQueue = nullptr;

        void MainLoop(WindowManager* windowManager);
        void Cleanup(WindowManager* windowManager);

        void InitGLFW();

        void InitVulkan(WindowManager* windowManager);
        void VulkanInstantiate();

        void CreateSurface(WindowManager* windowManager);
        void PickPhysicalDevice();
        void CreateLogicalDevice();

};


#endif //VULCRAFT_ENGINE_H