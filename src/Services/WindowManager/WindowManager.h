#ifndef VULCRAFT_WINDOWMANAGER_H
#define VULCRAFT_WINDOWMANAGER_H

#define GLFW_INCLUDE_VULKAN
#include <filesystem>
#include <GLFW/glfw3.h>

class WindowManager {
public:
    WindowManager();
    ~WindowManager();

    void InitWindowManager();
    void CreateWindow();

    GLFWwindow* g_window;                                       // CREATE GLOBAL VAR. GLFW WINDOW
private:
    const unsigned int m_WINDOW_WIDTH  = 1000;                  //WINDOW SETTINGS ( WIDTH )
    const unsigned int m_WINDOW_HEIGHT = 1000;                  //WINDOW SETTINGS ( HEIGHT )
};


#endif //VULCRAFT_WINDOWMANAGER_H