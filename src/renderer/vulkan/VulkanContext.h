#ifndef VULCRAFT_VULKANCONTEXT_H
#define VULCRAFT_VULKANCONTEXT_H

#include <vulkan/vulkan.h>
#include <GLFW/glfw3.h>
#include <vector>

class VulkanContext {
public:
    void Init(GLFWwindow* window);
    void Cleanup();
private:
    void CreateInstance();
    void SetupDebugManager();
    void DestroyDebugManager();
private:
    GLFWwindow* m_window = nullptr;

    VkInstance m_instance = VK_NULL_HANDLE;
    VkDebugUtilsMessengerEXT m_debugMessenger = VK_NULL_HANDLE;
};

#endif //VULCRAFT_VULKANCONTEXT_H
