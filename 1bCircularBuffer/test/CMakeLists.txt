cmake_minimum_required(VERSION 3.26)
project(Tests)

add_subdirectory(lib)

include_directories(${gtest_SOURCE_DIR}/include ${gtest_SOURCE_DIR})

add_executable(Tests CreationTest.cpp
        TestUtils.cpp
        TestUtils.h
        ChangeMethodsTest.cpp)

target_link_libraries(Tests circularbufferlib)

target_link_libraries(Tests gtest gtest_main)