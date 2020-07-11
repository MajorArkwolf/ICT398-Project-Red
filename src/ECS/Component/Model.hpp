#pragma once
#include <string>

namespace Component {
    struct Model {
        explicit Model(const std::string &model);
        size_t id = 0;
    };
}
