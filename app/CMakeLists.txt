
add_executable(yarc yarc.c)

if(YARC_LZ4)
    target_link_libraries(yarc yarc-lz4-hc)
endif()

target_link_libraries(yarc yarc-api)
