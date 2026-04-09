//
// Created by user on 09.04.2026.
//

#include "Device.h"

namespace vulcraft {

    bool isDeviceSuitable(VkPhysicalDevice device) {
        return true;
    }

    void Device::PickPhysicalDevice(VkInstance instance) {
        vkEnumeratePhysicalDevices(instance, &m_deviceCount, nullptr);
        if (m_deviceCount == 0) {
            throw std::runtime_error("[VULKAN]: No Physical Device found!");
        }

        m_devices.resize(m_deviceCount);
        vkEnumeratePhysicalDevices(instance, &m_deviceCount, m_devices.data());

        for (const auto& device : m_devices) {
            if (isDeviceSuitable(device)) {
                m_physicalDevice = device;
                break;
            }
        }

        if (m_physicalDevice == VK_NULL_HANDLE) {
            throw std::runtime_error("[VULKAN]: Failed to find a suitable GPU!");
        }
    }
} // vulcraft