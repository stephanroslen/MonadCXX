cmake_minimum_required(VERSION 3.18)
project(MonadCXX)

add_compile_options(-Werror -Wall -Wextra -Wpedantic)

set(CMAKE_CXX_STANDARD 20)

set(BUILD_TESTS "OFF" CACHE BOOL "Build tests")

add_subdirectory(MonadCXX)

if (BUILD_TESTS)
    enable_testing()
    add_subdirectory(tests)
endif ()
