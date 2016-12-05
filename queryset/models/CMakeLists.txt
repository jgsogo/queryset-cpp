project(queryset-cpp)
cmake_minimum_required(VERSION 2.8.12)

file(GLOB utils_SRC
    "*.h"
    "*.cpp"
    "*.hpp"
)

add_library(models ${utils_SRC})
set_target_properties(models PROPERTIES LINKER_LANGUAGE CXX)
