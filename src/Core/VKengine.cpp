#include "VKengine.h"

void VKengine::Init() {
    m_WindowManager.CreateWindow();
    InitVulkan();
}

void VKengine::InitVulkan() {
    m_Device.Init(m_WindowManager.GetWindow());
}

void VKengine::Run() {
    while (!m_WindowManager.ShouldClose()) {
        MainLoop();
    }
}

void VKengine::MainLoop() {
    m_WindowManager.PollEvents();
    m_Renderer.DrawFrame();
}

void VKengine::Cleanup() {
    m_Renderer.Cleanup();
    m_SwapChain.Cleanup();
    m_Device.Cleanup();
    m_WindowManager.Cleanup();
}

