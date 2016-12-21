project(queryset-cpp)
cmake_minimum_required(VERSION 3.1)

file(GLOB _TESTS
    "test_*.cpp"
)

add_executable (test_backends ../print_helper.hpp 
                              main.cpp
                              fixture_datasource.hpp
                              ${_TESTS})
target_link_libraries (test_backends
                       ${Boost_LIBRARIES}
                       ${CONAN_LIBS}
                       Threads::Threads
                       )
set_target_properties(test_backends PROPERTIES FOLDER tests)

add_test(NAME TestBackends COMMAND $<TARGET_FILE:test_backends>)
