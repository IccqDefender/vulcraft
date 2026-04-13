#ifndef VULCRAFT_VK_H
#define VULCRAFT_VK_H

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include "../utils/Debug.h"
#include "../utils/Logger.h"

#include "window.h"

class vk {
public:
    void Run();
    void Stop();

private:
    window* _window = nullptr;

    const uint32_t WIDTH = 800;
    const uint32_t HEIGHT = 600;
    const char* TITLE = "vulcraft";

    void MainLoop();

};



#endif //VULCRAFT_VK_H
