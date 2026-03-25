#ifndef VULCRAFT_RENDERER_H
#define VULCRAFT_RENDERER_H

#include <vulkan/vulkan.h>
#include "../Core/Device.h"
#include "../Core/Swapchain.h"

class Renderer {
public:
    void Init(Device& device, Swapchain& m_swapchain);
    void Cleanup();

    void DrawFrame();

private:
    Device* m_device = nullptr;
    Swapchain* m_swapchain = nullptr;

    VkRenderPass m_renderPass = VK_NULL_HANDLE;
    VkPipeline m_pipeline = VK_NULL_HANDLE;
    VkPipelineLayout m_pipelineLayout = VK_NULL_HANDLE;

    VkCommandPool m_commandPool = VK_NULL_HANDLE;
    std::vector<VkCommandBuffer> m_commandBuffers;

    std::vector<VkSemaphore> m_imageAvailable;
    std::vector<VkSemaphore> m_renderFinished;
    std::vector<VkFence> m_inFlight;

    uint32_t m_currentFrame = 0;

    void CreateRenderPass();
    void CreatePipeline();
    void CreateCommandBuffers();
    void CreateSyncObjects();
};

#endif //VULCRAFT_RENDERER_H