#ifndef VULCRAFT_WINDOWMANAGER_H
#define VULCRAFT_WINDOWMANAGER_H

#define GLFW_INCLUDE_VULKAN
#include <filesystem>
#include <GLFW/glfw3.h>

class WindowManager {
public:
    WindowManager();

    void CreateWindow();
    void Cleanup();

    bool ShouldClose() { return glfwWindowShouldClose(m_window); } ;
    void PollEvents() { glfwPollEvents(); };

    GLFWwindow* GetWindow() { return m_window; }

private:
    void InitWindowManager();

    GLFWwindow* m_window;                                      // CREATE GLOBAL VAR. GLFW WINDOW
    const unsigned int m_WINDOW_WIDTH  = 800;                  //WINDOW SETTINGS ( WIDTH )
    const unsigned int m_WINDOW_HEIGHT = 600;                  //WINDOW SETTINGS ( HEIGHT )
};


#endif //VULCRAFT_WINDOWMANAGER_H