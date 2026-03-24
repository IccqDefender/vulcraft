#ifndef VULCRAFT_VKENGINE_H
#define VULCRAFT_VKENGINE_H

#define GLFW_INCLUDE_VULKAN
#include <filesystem>
#include <GLFW/glfw3.h>

#include <iostream>
#include <vector>
#include <set>
#include <fstream>
#include <queue>


class vkengine {
public:
    void Run();

    void CreateVulkanInstance();
private:
    std::vector<char> readFile(std::string filePath);
    VkShaderModule createShaderModule(VkDevice device, const std::vector<char>& code);
    void recordCommandBuffer(VkCommandBuffer commandBuffer, uint32_t imageIndex, VkRenderPass renderPass, std::vector<VkFramebuffer> swapChainFramebuffers, VkExtent2D swapChainExtent, VkPipeline graphicPipeline);

    VkInstance m_instance = VK_NULL_HANDLE;
};


#endif //VULCRAFT_VKENGINE_H