//
// Created by user on 09.04.2026.
//

#include "Instance.h"

#include <stdexcept>

namespace vulcraft {
    void Instance::InitVulkan() {
        CreateInstance();
    }

    void Instance::CreateInstance() {
        m_appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
        m_appInfo.pApplicationName = "vulcraft";
        m_appInfo.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
        m_appInfo.pEngineName = "";
        m_appInfo.engineVersion = VK_MAKE_VERSION(1, 0, 0);
        m_appInfo.apiVersion = VK_API_VERSION_1_4;

        m_createInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
        m_createInfo.pApplicationInfo = &m_appInfo;

        vkEnumerateInstanceExtensionProperties(nullptr, &m_extensionCount, nullptr);

        m_extensions = glfwGetRequiredInstanceExtensions(&m_extensionCount);

        m_createInfo.enabledExtensionCount = m_extensionCount;
        m_createInfo.ppEnabledExtensionNames = m_extensions;

        m_createInfo.enabledLayerCount = 0;

        m_result = vkCreateInstance(&m_createInfo, nullptr, &m_instance);
        if (m_result != VK_SUCCESS) {
            throw std::runtime_error("[VULKAN]: Failed to create Vulkan instance!");
        }
    }

    void Instance::CleanupVulkan() const {
        vkDestroyInstance(m_instance, nullptr);
    }
} // vulcraft