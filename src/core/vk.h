#ifndef VULCRAFT_VK_H
#define VULCRAFT_VK_H

#include <memory>

#include "window.h"

class vk {
public:
    void Run();
    void Stop();

private:
    std::unique_ptr<window> _window;

    const uint32_t WIDTH = 800;
    const uint32_t HEIGHT = 600;
    const char* TITLE = "vulcraft";

    void MainLoop();

    bool m_running = false;
};



#endif //VULCRAFT_VK_H
