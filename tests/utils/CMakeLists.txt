project(queryset-cpp)
cmake_minimum_required(VERSION 3.1)

file(GLOB _TESTS
    "test_*.cpp"
)

add_executable (test_queryset_utils ../print_helper.hpp 
                                    main.cpp
                                    ${_TESTS})
target_link_libraries (test_queryset_utils
                       #utils
                       ${Boost_LIBRARIES}
                       Threads::Threads
                       )
set_target_properties(test_queryset_utils PROPERTIES FOLDER tests)

add_test(NAME TestUtils COMMAND $<TARGET_FILE:test_queryset_utils>)
