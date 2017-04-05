
set(RESOURCES
	squirrel.jpg
	colors.json
	rfc4960.txt)

yarc_bundle(NAME sample
	RESOURCES ${RESOURCES}
	OUTPUT resources.c
	VERBOSE COMPRESS)

add_library(yarc-sample-lib STATIC sample.c resources.c)

target_link_libraries(yarc-sample-lib yarc-api)

if(YARC_LZ4)
	target_link_libraries(yarc-sample-lib yarc-lz4)
endif()

add_executable(yarc-sample-app main.c)

target_link_libraries(yarc-sample-app yarc-sample-lib)

