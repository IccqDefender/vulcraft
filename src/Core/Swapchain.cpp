//
// Created by user on 24.03.2026.
//

#include "Swapchain.h"


void Swapchain::InitSwapchain() {
    CreateSwapchain();
}

void Swapchain::Cleanup() {
    vkDestroySwapchainKHR(m_device, m_swapChain, nullptr);
}

void Swapchain::CreateSwapchain() {
    vkCreateSwapchainKHR(m_device, &m_swapChainCreateInfo, nullptr, &m_swapChain);
}

