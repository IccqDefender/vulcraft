#ifndef VULCRAFT_VKENGINE_H
#define VULCRAFT_VKENGINE_H

#include "../Window/WindowManager.h"                // INCLUDE WINDOW MANAGER
#include "Device.h"                                 // INCLUDE DEVICE
#include "Swapchain.h"                              // INCLUDE SWAPCHAIN
#include "../Renderer/Renderer.h"                   // INCLUDE RENDERER

class VKengine {
public:
    void Init();                                    // VULKAN ENGINE INIT
    void Run();                                     // VULKAN ENGINE RUN
    void Cleanup();                                 // VULKAN ENGINE CLEANUP

private:
    WindowManager m_WindowManager;                  // VULKAN ENGINE WINDOW MANAGER
    Device m_Device;                                // VULKAN ENGINE DEVICE
    Swapchain m_SwapChain;                          // VULKAN ENGINE SWAPCHAIN
    Renderer m_Renderer;                            // VULKAN ENGINE RENDERER

    void InitVulkan();                              // VULKAN ENGINE INIT VULKAN
    void MainLoop();                                // VULKAN ENGINE MAIN LOOP

};


#endif //VULCRAFT_VKENGINE_H