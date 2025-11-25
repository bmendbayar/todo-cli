cmake_minimum_required(VERSION 3.14)
project(cli-todo VERSION 0.1)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_BUILD_TYPE Debug)

find_package(Boost REQUIRED COMPONENTS json CONFIG)

set(CURSES_NEED_NCURSES TRUE)
find_package(Curses REQUIRED)

set(SRC_FILES
    src/main.cpp
    src/basic_view.cpp
    src/model.cpp
    src/controller.cpp
)

include_directories(
    src
)

add_executable(todo ${SRC_FILES})
target_link_libraries(todo PRIVATE Boost::json)
target_link_libraries(todo PRIVATE ${CURSES_LIBRARIES})
