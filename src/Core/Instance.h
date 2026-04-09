#ifndef VULCRAFT_INSTANCE_H
#define VULCRAFT_INSTANCE_H

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include <iostream>

namespace vulcraft {
    class Instance {
        public:
        void InitVulkan();
        void CleanupVulkan() const;

        [[nodiscard]] VkInstance GetVulkanInstance()         const { return m_instance; };      //GET INSTANCE
        [[nodiscard]] VkApplicationInfo GetApplicationInfo() const { return m_appInfo; };       //GET APP INFO
        [[nodiscard]] VkInstanceCreateInfo GetCreateInfo()   const { return m_createInfo; };    //GET CREATE INFO

        private:
        VkInstance m_instance = VK_NULL_HANDLE;

        VkApplicationInfo m_appInfo = {};
        VkInstanceCreateInfo m_createInfo = {};

        uint32_t m_extensionCount = 0;
        const char** m_extensions = nullptr;

        VkResult m_result;

        void CreateInstance();

    };
} // vulcraft

#endif //VULCRAFT_INSTANCE_H