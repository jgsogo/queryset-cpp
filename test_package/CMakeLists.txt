project(HelloQuerysetCPP)
cmake_minimum_required(VERSION 2.8.12)

include(${CMAKE_BINARY_DIR}/conanbuildinfo.cmake)
conan_basic_setup()

add_executable(queryset_cpp main.cpp)
include_directories(${CONAN_INCLUDE_DIRS})
target_link_libraries(queryset_cpp ${CONAN_LIBRARIES})
