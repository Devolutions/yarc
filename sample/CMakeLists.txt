
yarc_bundle(NAME sample
	RESOURCES squirrel.jpg colors.json
	OUTPUT resources.c
	VERBOSE)

add_executable(yarc-sample sample.c resources.c)

