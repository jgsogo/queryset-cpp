project(queryset-cpp)
cmake_minimum_required(VERSION 3.1)

add_executable (test_models ../print_helper.hpp 
                            main.cpp
                            test_model.cpp
                            test_model_custom.cpp
                            test_model_custom_manager.cpp)

target_link_libraries (test_models
                       ${Boost_LIBRARIES}
                       Threads::Threads
                       )

add_test(NAME TestModels COMMAND $<TARGET_FILE:test_models>)
