#include <iostream>

#include "core/vk.h"

#include <memory>
#include <ostream>

int main() {
    auto const _vk = std::make_unique<vk>();

    _vk->Run();

    _vk->Stop();

    return 0;
}
