cmake_minimum_required(VERSION 3.14)
project(cli-todo VERSION 0.1)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_BUILD_TYPE Debug)

find_package(Boost REQUIRED COMPONENTS json CONFIG)

set(SRC_FILES
    src/main.cpp
    src/todo_view.cpp
    src/todo_list.cpp
    src/todo_controller.cpp
)

include_directories(
    src
)

add_executable(todo ${SRC_FILES})
target_link_libraries(todo PRIVATE Boost::json)
