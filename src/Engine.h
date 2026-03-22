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

        /* VULKAN PROPERTIES */
        VkInstance m_instance;

        VkSurfaceKHR m_windowSurface;

        VkPhysicalDevice m_physicalDevice;
        VkPhysicalDeviceMemoryProperties m_deviceMemoryProperties;
        VkDevice m_device;

        VkQueue m_graphicsQueue;
        VkQueue m_presentQueue;
        uint32_t m_graphicsQueueFamily;
        uint32_t m_presentQueueFamily;

        VkBuffer m_vertexBuffer;
        VkBuffer m_indexBuffer;

        VkSemaphore m_imageAvailableSemaphore;
        VkSemaphore m_renderingFinishedSemaphore;


        /* VULKAN FUNCTIONS*/
        void InitVulkan(WindowManager* windowManager);

        void VulkanCreateInstance();

        void CreateWindowSurface(WindowManager* windowManager);

        void FindPhysicalDevice();

        void CheckSwapChainSupport();

        void FindQueueFamilies();

        void CreateLogicalDevice();

        void CreateSemaphores();

        void CreateCommandPool();

        void CreateVertexBuffer();

        void CreateUniformBuffer();

        void CreateSwapChain();

        void CreateRenderPass();

        void CreateImageViews();

        void CreateFramebuffers();

        void CreateGraphicsPipeline();

        void CreateDescriptorPool();

        void CreateDescriptorSet();

        void CreateCommandBuffers();

        /* ENGINE FUNCTIONS */
        void MainLoop(WindowManager* windowManager);
        void Cleanup(WindowManager* windowManager);

        /* GLFW FUNCTIONS */
        void InitGLFW();

};


#endif //VULCRAFT_ENGINE_H