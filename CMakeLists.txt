cmake_minimum_required(VERSION 3.0)

project(YARC C)

include(cmake/YarcBundle.cmake)

option(YARC_LZ4 "yarc lz4 compression" ON)

include_directories(external)
add_subdirectory(external)

if(YARC_LZ4)
	add_definitions(-DYARC_LZ4)
endif()

include_directories(include)

if(NOT CMAKE_CROSSCOMPILING)
	add_subdirectory(app)
endif()

add_subdirectory(libyarc)

add_subdirectory(sample)

