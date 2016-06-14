project(queryset-cpp)
cmake_minimum_required(VERSION 2.8.12)

add_executable (test_queryset print_helper.hpp 
                              main.cpp
                              test_class_queryset_filter.cpp
                              test_class_queryset_groupby.cpp
                              test_datasource_memory.cpp
                              test_datasource_file.cpp)
target_link_libraries (test_queryset
                       #utils
                       ${Boost_LIBRARIES}
                       )

add_test(NAME TestQuerySet COMMAND $<TARGET_FILE:test_queryset>)
