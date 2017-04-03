cmake_minimum_required(VERSION 3.0)

project(YARC C)

include_directories(${CMAKE_CURRENT_SOURCE_DIR})
add_executable(yarc yarc.c)

include(cmake/YarcBundle.cmake)

add_subdirectory(sample)

