####################
#### Unit tests  ###
####################

add_executable(eyos_tests 
unittests/main.cpp
unittests/packet_tests.cpp
unittests/stream_tests.cpp
)

target_link_libraries(eyos_tests 
PUBLIC
net::net
Catch2::Catch2
)

set_target_properties( eyos_tests
    PROPERTIES
    ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/libs"
    LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/libs"
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin"
)

include(CTest)
include(Catch)
catch_discover_tests(eyos_tests)

####################
#### benchmarks  ###
####################

add_executable(eyos_benchmark benchmark_main.cpp)

target_link_libraries(eyos_benchmark
PRIVATE
net::net
benchmark
)

if(WIN32)
    target_link_libraries(eyos_benchmark PUBLIC Shlwapi.lib)
endif(WIN32)

set_target_properties( eyos_benchmark
    PROPERTIES
    ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/libs"
    LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/libs"
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin"
)
