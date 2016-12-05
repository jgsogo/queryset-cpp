project(queryset-cpp)
cmake_minimum_required(VERSION 2.8.12)

file(GLOB queryset_SRC
    "*.h"
    "*.cpp"
    "*.hpp"
)

file(GLOB queryset_impl_SRC
    "_impl/*.h"
    "_impl/*.cpp"
    "_impl/*.hpp"
)

source_group(impl FILES ${queryset_impl_SRC} )
add_library(queryset ${queryset_SRC} ${queryset_impl_SRC})
# set_target_properties(queryset PROPERTIES LINKER_LANGUAGE CXX)

add_subdirectory(utils)
add_subdirectory(models)
