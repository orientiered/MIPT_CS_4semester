cmake_minimum_required(VERSION 3.26)
project(SnakeGame
  LANGUAGES CXX
  VERSION 0.1
)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(EXEC_NAME snake.exe)

file(GLOB sources src/*.cpp)
message("SOURCES = ${sources}")

add_executable(${EXEC_NAME} ${sources})

target_include_directories(${EXEC_NAME} PRIVATE include)
target_include_directories(${EXEC_NAME} PRIVATE .)
