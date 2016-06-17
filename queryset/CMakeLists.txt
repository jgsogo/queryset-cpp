project(queryset-cpp)
cmake_minimum_required(VERSION 2.8.12)

file(GLOB queryset_SRC
    "*.h"
    "*.cpp"
    "*.hpp"
)

add_library(queryset ${queryset_SRC})
set_target_properties(queryset PROPERTIES LINKER_LANGUAGE CXX)

add_subdirectory(utils)
add_subdirectory(models)
