#ifndef VULCRAFT_WINDOWMANAGER_H
#define VULCRAFT_WINDOWMANAGER_H

#include <GLFW/glfw3.h>
#include <iostream>


class WindowManager {
    public:
    void InitializeWindow(); // VOID FUNCTION TO CREATE A WINDOW

    uint32_t m_WINDOW_WIDTH = 800; // WINDOW WIDTH
    uint32_t m_WINDOW_HEIGHT = 600; // WINDOW HEIGHT

    GLFWwindow* window;

    private:
    /* WINDOW SETTINGS */



    std::string m_WINDOW_TITLE = "Vulcraft"; // WINDOW TITLE

};


#endif //VULCRAFT_WINDOWMANAGER_H