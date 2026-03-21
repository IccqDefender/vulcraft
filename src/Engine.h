#ifndef VULCRAFT_ENGINE_H
#define VULCRAFT_ENGINE_H
#include "Managers/WindowManager/WindowManager.h"


class Engine {

    public:
        void Run();
        void InitGLFW();

    private:
        void MainLoop(WindowManager* windowManager);
        void Cleanup(WindowManager* windowManager);
};


#endif //VULCRAFT_ENGINE_H