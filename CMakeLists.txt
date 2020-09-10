cmake_minimum_required(VERSION 3.10)

project(test_task_2 VERSION 1)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_POSITION_INDEPENDENT_CODE ON)

find_package(Threads REQUIRED)

set(SOURCE_FILES main.cpp test.h test.cpp config.h institution.h institution.cpp)

add_executable(test_executable ${SOURCE_FILES})
target_link_libraries(test_executable Threads::Threads)
