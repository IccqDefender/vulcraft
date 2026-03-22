
#ifndef VULCRAFT_VULKANMANAGER_H
#define VULCRAFT_VULKANMANAGER_H

#include <vulkan/vulkan.h>
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"

#include "../WindowManager/WindowManager.h"

class VulkanManager {
public:
    void InitVulkan(WindowManager* windowManager);

private:
    /* VULKAN FUNCTIONS */
    void InitializeVulkan(); // VULKAN INIT FUNCTION

    void CreateSurface(WindowManager* windowManager); // VULKAN CREATE SURFACE

    void PickPhysicalDevice(); // VULKAN PICK PHYSICAL DEVICE

    void CreateLogicalDevice(); // VULKAN CREATE LOGICAL DEVICE

    void CreateSwapChain(); // VULKAN CREATE SWAP CHAIN

    void CreateImageViews(); // VULKAN CREATE IMAGE VIEWS

    void CreateRenderPass(); // VULKAN CREATE RENDER PASS

    void CreateGraphicsPipeline(); // VULKAN CREATE GRAPHICS PIPELINE

    void CreateFramebuffers(); // VULKAN CREATE FRAME BUFFERS

    void CreateCommandPool(); // VULKAN CREATE COMMAND POOL

    void CreateVertexBuffer(); // VULKAN CREATE VERTEX BUFFER

    void CreateCommandBuffer(); // VULKAN CREATE COMMAND BUFFER

    void CreateSyncObjects(); // VULKAN CREATE SYNC OBJECTS

    /* VULKAN INSTANCE */
    VkInstance m_instance; // VULKAN INSTANCE

    /* VULKAN DEVICES */
    VkPhysicalDevice m_physicalDevice; // VULKAN PHYSICAL DEVICE
    VkDevice m_device; // VULKAN DEVICE

    /* VULKAN CALLBACKS */
    VkAllocationCallbacks* m_allocator = nullptr; // VULKAN ALLOCATOR

    /* VULKAN QUEUE */
    uint32_t m_queueFamily = (uint32_t)-1; // VULKAN QUEUE FAMILY
    VkQueue m_queue = VK_NULL_HANDLE; // VULKAN QUEUE
    VkQueue m_graphicsQueue = VK_NULL_HANDLE; // VULKAN GRAPHIC QUEUE
    VkQueue m_presentQueue = VK_NULL_HANDLE; // VULKAN PRESENT QUEUE

    /* VULKAN DEBUG */
    VkDebugReportCallbackEXT m_debugReport = VK_NULL_HANDLE; // VULKAN DEBUG REPORT CALLBACK

    /* VULKAN PIPELINE */
    VkPipelineCache m_pipelineCache = VK_NULL_HANDLE; // VULKAN PIPELINE CACHE
    VkPipelineLayout m_pipelineLayout; // VULKAN PIPELINE LAYOUT
    VkPipeline m_graphicsPipeline; // VULKAN GRAPHICS PIPELINE

    /* VULKAN POOLS */
    VkDescriptorPool m_descriptorPool = VK_NULL_HANDLE; // VULKAN DESCRIPTOR POOL
    VkCommandPool m_commandPool = VK_NULL_HANDLE; // VULKAN COMMAND POOL

    /* VULKAN SURFACE */
    VkSurfaceKHR m_surface = VK_NULL_HANDLE; // VULKAN SURFACE

    /* VULKAN RENDER PASS*/
    VkRenderPass m_renderPass = VK_NULL_HANDLE; // VULKAN RENDER PASS

    /* VULKAN SWAPCHAIN */
    VkSwapchainKHR m_swapChain = VK_NULL_HANDLE; // VULKAN SWAP CHAIN
    VkFormat m_swapChainImageFormat; // VULKAN SWAP CHAIN IMAGE FORMAT
    VkExtent2D m_swapChainExtent; // VULKAN SWAP CHAIN EXTENT
    std::vector<VkImage> m_swapChainImages; // VULKAN SWAP CHAIN IMAGES
    std::vector<VkImageView> m_swapChainImageViews; // VULKAN SWAP CHAIN IMAGE VIEWS
    std::vector<VkFramebuffer> m_swapChainFramebuffers; // VULKAN SWAP CHAIN FRAME BUFFERS

    /* VULKAN BUFFERS */
    VkCommandBuffer m_commandBuffer; // VULKAN COMMAND BUFFER
    VkBuffer m_vertexBuffer; // VULKAN VERTEX BUFFER
    VkDeviceMemory m_vertexBufferMemory; // VULKAN VERTEX BUFFER MEMORY

    /* VULKAN SEMAPHORE */
    VkSemaphore m_imageAvailableSemaphore; // VULKAN IMAGE AVAILABLE SEMAPHORE
    VkSemaphore m_renderFinishedSemaphore; // VULKAN RENDER FINISHED SEMAPHORE
    VkFence m_inFlightFence; // VULKAN [ IN FLIGHT FENCE ]

};


#endif //VULCRAFT_VULKANMANAGER_H