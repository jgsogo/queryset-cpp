project(queryset-cpp)
cmake_minimum_required(VERSION 3.1)

add_executable (test_queryset ../print_helper.hpp 
                              main.cpp
                              test_class_queryset_eval.cpp
                              test_class_queryset_filter.cpp
                              test_class_queryset_groupby.cpp
                              test_class_queryset_operators.cpp
                              test_datasource_memory.cpp
                              test_datasource_file.cpp)
target_link_libraries (test_queryset
                       ${Boost_LIBRARIES}
                       Threads::Threads
                       )

add_test(NAME TestQuerySet COMMAND $<TARGET_FILE:test_queryset>)
