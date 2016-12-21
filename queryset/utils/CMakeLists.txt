project(queryset-cpp)
cmake_minimum_required(VERSION 2.8.12)

file(GLOB utils_SRC
    "*.h"
    "*.cpp"
    "*.hpp"
)

add_library(utils ${utils_SRC})
set_target_properties(utils PROPERTIES LINKER_LANGUAGE CXX)
