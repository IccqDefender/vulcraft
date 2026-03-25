//
// Created by user on 24.03.2026.
//

#ifndef VULCRAFT_SWAPCHAIN_H
#define VULCRAFT_SWAPCHAIN_H

#include <vulkan/vulkan.h>
#include "../Core/Device.h"


class Swapchain {
public:
    void InitSwapchain();
    void Cleanup();

    VkSwapchainKHR GetSwapchain() { return m_swapChain; };
private:
    VkDevice m_device = Device().GetDevice();

    void CreateSwapchain();
    void CreateSwapchainInfo();

    VkSwapchainCreateInfoKHR m_swapChainCreateInfo = {};
    VkSwapchainKHR m_swapChain;
};


#endif //VULCRAFT_SWAPCHAIN_H