cmake_minimum_required(VERSION 3.10)

# Set the project name
project(SnakeGame C)

# Set the C standard
set(CMAKE_C_STANDARD 99)

# Add the executable
add_executable(snek snek.c)

# Find and link the ncurses library
find_package(Curses REQUIRED)
include_directories(${CURSES_INCLUDE_DIRS})
target_link_libraries(snek ${CURSES_LIBRARIES})
