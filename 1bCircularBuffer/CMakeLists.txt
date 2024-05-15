cmake_minimum_required(VERSION 3.26)
project(1bCircularBuffer)

set(CMAKE_CXX_STANDARD 20)

include_directories(./)

add_library(circularbufferlib SHARED CircularBuffer.h)

if (MSVC)
    add_compile_options(/W4)
else ()
    add_compile_options(-Wall -Wextra -Wpedantic -Werror)
endif ()

set_target_properties(circularbufferlib PROPERTIES LINKER_LANGUAGE CXX)

add_subdirectory(test)

add_executable(1bCircularBuffer main.cpp)

target_link_libraries(1bCircularBuffer PRIVATE circularbufferlib)