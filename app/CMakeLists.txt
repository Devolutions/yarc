
if(${PROJECT_NAME} STREQUAL "YARC")

	add_executable(yarc yarc.c)

	if(YARC_LZ4)
		target_link_libraries(yarc yarc-lz4-hc)
	endif()

	target_link_libraries(yarc yarc-api)

else()
	cmake_minimum_required(VERSION 3.0)

	option(YARC_LZ4 "yarc lz4 compression" ON)

	include_directories(../external)

	if(YARC_LZ4)
		add_definitions(-DYARC_LZ4)
		set(YARC_EXTRA_SOURCES
			../external/lz4/lz4.c
			../external/lz4/lz4hc.c)
	endif()

	add_executable(yarc yarc.c ${YARC_EXTRA_SOURCES})

	target_link_libraries(yarc yarc-api)
endif()

