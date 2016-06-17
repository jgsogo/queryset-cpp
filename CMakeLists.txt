project(queryset-cpp)
cmake_minimum_required(VERSION 2.8.12)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED on)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++14")  # Needed for Travis CI
add_compile_options(-std=c++14) # CMake 2.8.12 or newer

include(${CMAKE_BINARY_DIR}/conanbuildinfo.cmake)
conan_basic_setup()

# Files in subdirectories
set_property(GLOBAL PROPERTY USE_FOLDERS ON)

OPTION(SPDLOG_DEBUG_ON "Enable debug logging" OFF) # Disabled by default
IF(SPDLOG_DEBUG_ON)
    ADD_DEFINITIONS(-DSPDLOG_DEBUG_ON)
ENDIF(SPDLOG_DEBUG_ON)

add_subdirectory(queryset)

option(BUILD_TEST "Build test" OFF)
if (BUILD_TEST)
    # Testing
    enable_testing()
    if (UNIX)
        add_definitions(-Wfatal-errors)
        add_custom_target(check ${CMAKE_COMMAND} -E env CTEST_OUTPUT_ON_FAILURE=1
                          ${CMAKE_CTEST_COMMAND} --verbose
                          WORKING_DIRECTORY ${CMAKE_BINARY_DIR})
    else()
        add_custom_target(check ${CMAKE_COMMAND} -E env CTEST_OUTPUT_ON_FAILURE=1
                          ${CMAKE_CTEST_COMMAND} -C $<CONFIG> --verbose
                          WORKING_DIRECTORY ${CMAKE_BINARY_DIR})
    endif()

    #
    add_subdirectory(tests)
endif()

