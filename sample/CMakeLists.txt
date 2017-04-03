
yarc_bundle(NAME sample
	RESOURCES squirrel.jpg colors.json rfc4960.txt
	OUTPUT resources.c
	VERBOSE COMPRESS)

add_executable(yarc-sample sample.c resources.c)

target_link_libraries(yarc-sample yarc-api)

if(YARC_LZ4)
	target_link_libraries(yarc-sample yarc-lz4)
endif()

