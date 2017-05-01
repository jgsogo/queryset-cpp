
#pragma once

#include <vector>
#include <string>
#include "to_string.h"

namespace utils {
    template <typename T>
    std::string join(const T& values, const std::string& sep) {
		std::ostringstream ret; ret << utils::to_string(*values.begin());
		std::for_each(std::next(values.begin()), values.end(), [&ret, &sep](const auto& item) { ret << sep << utils::to_string(item); });
        return ret.str();
    }
}