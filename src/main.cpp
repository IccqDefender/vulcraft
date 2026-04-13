#include <iostream>

#include "core/vk.h"

#include <memory>
#include <ostream>

int main() {
    auto const _vk = std::make_unique<vk>();

    try {
        _vk->Run();

        _vk->Stop();

    }catch (std::exception& e) {
        std::cerr << e.what() << std::endl;
    }

    return 0;
}
