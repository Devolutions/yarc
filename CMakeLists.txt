cmake_minimum_required(VERSION 3.0)

project(YARC C)

include(ExternalProject)
include(cmake/YarcBundle.cmake)

option(YARC_API "yarc api library" ON)
option(YARC_LZ4 "yarc lz4 compression" ON)
option(YARC_SAMPLE "yarc sample project" ON)

include_directories(external)
add_subdirectory(external)

if(YARC_LZ4)
	add_definitions(-DYARC_LZ4)
endif()

include_directories(include)

if(NOT CMAKE_CROSSCOMPILING)
	add_subdirectory(app)
else()
	set(YARC_HOST_PREFIX "yarc-host")
	ExternalProject_Add(yarc-host
		PREFIX "${YARC_HOST_PREFIX}"
		DOWNLOAD_COMMAND ""
		SOURCE_DIR "${CMAKE_SOURCE_DIR}/app"
		BINARY_DIR "${YARC_HOST_PREFIX}"
		INSTALL_COMMAND "")
	add_executable(yarc IMPORTED)
	set_property(TARGET yarc PROPERTY IMPORTED_LOCATION "${CMAKE_BINARY_DIR}/${YARC_HOST_PREFIX}/yarc")
endif()

if(YARC_API)
	add_subdirectory(libyarc)
endif()

if(YARC_SAMPLE)
	add_subdirectory(sample)
endif()

