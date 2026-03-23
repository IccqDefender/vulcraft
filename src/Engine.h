#ifndef VULCRAFT_ENGINE_H
#define VULCRAFT_ENGINE_H


class Engine {
public:
    Engine();
    ~Engine();

    void Run();


private:

    void MainLoop();
};


#endif //VULCRAFT_ENGINE_H