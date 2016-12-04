project(queryset-cpp)
cmake_minimum_required(VERSION 2.8.12)

file(GLOB _SRC
    "*.h"
    "*.cpp"
    "*.hpp"
)

add_library(backends ${_SRC})
set_target_properties(backends PROPERTIES LINKER_LANGUAGE CXX)
