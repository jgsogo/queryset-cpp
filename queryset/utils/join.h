
#pragma once

#include <vector>
#include <string>

namespace utils {
    template <typename T>
    std::string join(const T& values, const std::string& sep) {
        std::ostringstream ret; ret << *values.begin();
        std::for_each(std::next(values.begin()), values.end(), [&ret, &sep](const auto& item) { ret << sep << item; });
        return ret.str();
    }
}