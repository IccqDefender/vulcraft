//
// Created by user on 09.04.2026.
//

#include "Instance.h"

#include <cstring>
#include <stdexcept>

#ifdef NDEBUG
const bool enableValidationLayers = false;
#else
const bool enableValidationLayers = true;
#endif

namespace vulcraft {
    void Instance::InitVulkan() {
        CreateInstance();
    }

    bool Instance::CheckValidationLayerSupport() {
        uint32_t layerCount;
        vkEnumerateInstanceLayerProperties(&layerCount, nullptr);

        std::vector<VkLayerProperties> availableLayers(layerCount);
        vkEnumerateInstanceLayerProperties(&layerCount, availableLayers.data());

        for (const char* layerName : m_validationLayers) {
            bool layerFound = false;

            for (const auto& layerProperties : availableLayers) {
                if (strcmp(layerName, layerProperties.layerName) == 0) {
                    layerFound = true;
                    break;
                }
            }

            if (!layerFound) {
                return false;
            }
        }

        return true;
    }

    std::vector<const char *> Instance::GetRequiredExtensions() {
        uint32_t glfwExtensionCount;
        const char** glfwExtensions;
        glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);

        std::vector<const char*> extensions(glfwExtensions, glfwExtensions + glfwExtensionCount);

        if (enableValidationLayers) {
            extensions.push_back(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);
        }
        return extensions;
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

        auto extensions = GetRequiredExtensions();

        m_createInfo.enabledExtensionCount = static_cast<uint32_t>(extensions.size());
        m_createInfo.ppEnabledExtensionNames = extensions.data();

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