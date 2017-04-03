
add_library(yarc-api STATIC yarc.c)

if(YARC_LZ4)
	target_link_libraries(yarc-api yarc-lz4)
endif()

