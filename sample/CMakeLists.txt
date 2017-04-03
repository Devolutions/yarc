
yarc_bundle(NAME sample
	RESOURCES squirrel.jpg colors.json rfc4960.txt
	OUTPUT resources.c
	VERBOSE COMPRESS)

add_executable(yarc-sample sample.c resources.c)

if(YARC_LZ4)
	target_link_libraries(yarc-sample lz4)
endif()

