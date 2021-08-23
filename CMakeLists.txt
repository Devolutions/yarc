cmake_minimum_required(VERSION 3.0)

project(YARC C)

include(ExternalProject)
include(cmake/YarcBundle.cmake)

option(YARC_API "yarc api library" ON)
option(YARC_LZ4 "yarc lz4 compression" ON)
option(YARC_SAMPLE "yarc sample project" OFF)

include_directories(external)
add_subdirectory(external)

if(YARC_LZ4)
	add_definitions(-DYARC_LZ4)
endif()

include_directories(include)

add_subdirectory(app)

if(YARC_API)
	add_subdirectory(libyarc)
endif()

if(YARC_SAMPLE)
	add_subdirectory(sample)
endif()
