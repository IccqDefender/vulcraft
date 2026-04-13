#ifndef VULCRAFT_VK_H
#define VULCRAFT_VK_H

#include <memory>

#include "window.h"
#include "../renderer/vulkan/VulkanContext.h"

class vk {
public:
    void Run();
    void Stop();

private:
    std::unique_ptr<window> _window;
    std::unique_ptr<VulkanContext> _vkContext;

private:
    const uint32_t WIDTH = 800;
    const uint32_t HEIGHT = 600;
    const char* TITLE = "vulcraft";

    void MainLoop();

    bool m_running = false;
};



#endif //VULCRAFT_VK_H
