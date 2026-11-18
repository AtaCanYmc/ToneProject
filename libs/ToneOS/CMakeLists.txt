cmake_minimum_required(VERSION 3.20)
project(ToneOS)

set(CMAKE_CXX_STANDARD 14)

include_directories(
    include
)

file(GLOB_RECURSE SOURCES
    src/*.cpp
)

add_executable(ToneOS ${SOURCES})
