
# lz4, lz4-hc

include_directories(${CMAKE_CURRENT_SOURCE_DIR})

set(LZ4_SOURCES lz4.c)
set(LZ4_HEADERS lz4.h)

set(LZ4_HC_SOURCES lz4hc.c)
set(LZ4_HC_HEADERS lz4hc.h lz4opt.h)

add_library(lz4 STATIC ${LZ4_SOURCES} ${LZ4_HEADERS})
add_library(lz4-hc STATIC ${LZ4_HC_SOURCES} ${LZ4_HC_HEADERS})

target_link_libraries(lz4-hc lz4)

set_target_properties(lz4 PROPERTIES OUTPUT_NAME lz4 POSITION_INDEPENDENT_CODE ON)
set_target_properties(lz4-hc PROPERTIES OUTPUT_NAME lz4-hc POSITION_INDEPENDENT_CODE ON)

