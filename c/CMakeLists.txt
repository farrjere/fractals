cmake_minimum_required(VERSION 3.28)
project(fractal C)
include(CMakeFindDependencyMacro)
set(CMAKE_C_STANDARD 23)

find_package(ZLIB REQUIRED)
find_package(SPNG REQUIRED)

include_directories(ZLIB_INCLUDE_DIRS)
include_directories("/usr/local/include")
include_directories("/usr/local/lib")
add_executable(fractal main.c
        fractal.c
        fractal.h
        rpng.h
)
