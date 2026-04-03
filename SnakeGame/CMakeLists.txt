cmake_minimum_required(VERSION 3.26)
project(SnakeGame
  LANGUAGES CXX
  VERSION 0.1
)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

option(ENABLE_ASAN "Enable Address Sanitizer" OFF)

if(ENABLE_ASAN)
    set(SAN_FLAGS "-fsanitize=address,undefined")
    add_compile_options(${SAN_FLAGS} -fno-omit-frame-pointer)
    add_link_options(${SAN_FLAGS})
endif()

# ---------------------------------------
add_subdirectory(3rd-party)

# Copying SFML headers
set(SFML_DOWNLOAD_INCLUDE ${CMAKE_CURRENT_BINARY_DIR}/_deps/sfml-src/include/)

file(COPY ${SFML_DOWNLOAD_INCLUDE} DESTINATION 3rd-party/)

# ---------------------------------------

set(EXEC_NAME snake.exe)

file(GLOB sources src/*.cpp)
message("SOURCES = ${sources}")


add_executable(${EXEC_NAME} ${sources})

target_include_directories(${EXEC_NAME} PRIVATE . include)
target_link_libraries(${EXEC_NAME} PRIVATE SFML::Graphics)
