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
private:
    VkInstance m_instance = VK_NULL_HANDLE;
    GLFWwindow* m_window = nullptr;

};

#endif //VULCRAFT_VULKANCONTEXT_H
