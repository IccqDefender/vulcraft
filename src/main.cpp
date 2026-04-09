#include <iostream>
#include <memory>

#include "Core/VKEngine.h"

using namespace vulcraft;

int main() {
    const auto vk_engine = std::make_unique<VKEngine>();

    try {
        vk_engine->Run();
        vk_engine->Stop();

    }catch (std::exception& e) {
        std::cerr << e.what() << std::endl;
    }

    return 0;
}