project(queryset-cpp)
cmake_minimum_required(VERSION 3.1)

add_executable (test_queryset_utils ../print_helper.hpp 
                                    main.cpp
                                    test_tuple.cpp
                                    test_queryset.cpp
                                    test_queryset_list.cpp
                                    test_queryset_filter.cpp
                                    test_queryset_project.cpp
                                    test_queryset_marginalize.cpp)
target_link_libraries (test_queryset_utils
                       #utils
                       ${Boost_LIBRARIES}
                       Threads::Threads
                       )

add_test(NAME TestUtils COMMAND $<TARGET_FILE:test_queryset_utils>)
