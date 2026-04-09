#ifndef VULCRAFT_VKENGINE_H
#define VULCRAFT_VKENGINE_H

#include "../Window/WindowManager.h"
#include "Instance.h"
#include "Device.h"

namespace vulcraft {
    class VKEngine {
        public:
        VKEngine();

        void Run();
        void Stop();

        private:
        WindowManager* _windowManager = nullptr;
        Instance* _instance = nullptr;
        Device* _device = nullptr;

        void MainLoop(GLFWwindow* window);
        void Cleanup();
    };
}

#endif //VULCRAFT_VKENGINE_H