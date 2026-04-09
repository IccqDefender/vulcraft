#ifndef VULCRAFT_DEVICE_H
#define VULCRAFT_DEVICE_H

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include <vector>
#include <iostream>

namespace vulcraft {
    class Device {
        public:
        Device();
        ~Device();

        void PickPhysicalDevice(VkInstance instance);

        [[nodiscard]] VkPhysicalDevice GetPhysicalDevice() const{ return m_physicalDevice; };

        private:
        VkPhysicalDevice m_physicalDevice = VK_NULL_HANDLE;

        uint32_t m_deviceCount = 0;
        std::vector<VkPhysicalDevice> m_devices;

    };
} // vulcraft

#endif //VULCRAFT_DEVICE_H