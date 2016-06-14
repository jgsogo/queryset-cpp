project(queryset-cpp)
cmake_minimum_required(VERSION 2.8.12)

add_executable (test_models main.cpp
                            test_model.cpp)
target_link_libraries (test_models
                       ${Boost_LIBRARIES}
                       )

add_test(NAME TestModels COMMAND $<TARGET_FILE:test_models>)
