cmake_minimum_required(VERSION 3.0)

project(YARC C)

option(YARC_LZ4 "yarc lz4 compression" ON)

include_directories(external)
add_subdirectory(external)

if(YARC_LZ4)
	add_definitions(-DYARC_LZ4)
endif()

include_directories(${CMAKE_CURRENT_SOURCE_DIR})
add_executable(yarc yarc.c)

if(YARC_LZ4)
	target_link_libraries(yarc lz4-hc)
endif()

include(cmake/YarcBundle.cmake)

add_subdirectory(sample)

