project(queryset-cpp)
cmake_minimum_required(VERSION 2.8.12)

set(THREADS_PREFER_PTHREAD_FLAG TRUE)
find_package (Threads REQUIRED)
find_package (Boost COMPONENTS system filesystem unit_test_framework REQUIRED)

include_directories (${TEST_SOURCE_DIR}/src
                     ${Boost_INCLUDE_DIRS}
                    )

# Copy test data
SET(TEST_DATA_PATH ${CMAKE_BINARY_DIR}/bin/test_data)
file(MAKE_DIRECTORY ${TEST_DATA_PATH})
configure_file(config_tests.h.cmake ${CMAKE_CURRENT_LIST_DIR}/config_tests.h)
file(COPY data/ex_filequeryset.tsv DESTINATION ${TEST_DATA_PATH})
file(COPY data/fixture_datasource.tsv DESTINATION ${TEST_DATA_PATH})

if(BUILD_SHARED_LIBS)
    add_definitions(-DBOOST_TEST_DYN_LINK)
endif(BUILD_SHARED_LIBS)

# Add subdirectories with tests
add_subdirectory(queryset)
add_subdirectory(utils)
add_subdirectory(models)
add_subdirectory(backends)
