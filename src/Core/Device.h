#ifndef VULCRAFT_DEVICE_H
#define VULCRAFT_DEVICE_H

#include <vulkan/vulkan.h>
#include "../Window/WindowManager.h"

#include <vector>
#include <set>
#include <stdexcept>

class Device {
public:
    void Init(GLFWwindow* window);
    void Cleanup();

    VkDevice GetDevice() { return m_device; }
    VkPhysicalDevice GetPhysicalDevice() { return m_physicalDevice; }
    VkQueue GetGraphicsQueue() { return m_graphicsQueue; }
    VkQueue GetPresentQueue() { return m_presentQueue; }
    VkSurfaceKHR GetSurface() { return m_surface; }

private:
    VkInstance m_instance = VK_NULL_HANDLE;
    VkDevice m_device = VK_NULL_HANDLE;
    VkPhysicalDevice m_physicalDevice = VK_NULL_HANDLE;
    VkSurfaceKHR m_surface = VK_NULL_HANDLE;

    VkQueue m_graphicsQueue = VK_NULL_HANDLE;
    VkQueue m_presentQueue = VK_NULL_HANDLE;

    uint32_t m_graphicsQueueFamilyIndex = 0;
    uint32_t m_presentQueueFamilyIndex = 0;

    void CreateInstance();
    void CreateSurface(GLFWwindow* window);
    void PickPhysicalDevice();
    void CreateLogicalDevice();
};

#endif //VULCRAFT_DEVICE_H