add_library(
    net
    net/streams/InputStream.cpp
    net/streams/OutputStream.cpp
    net/Allocator.cpp
    net/Address.cpp
    net/Net.cpp
    net/Peer.cpp
    net/Host.cpp
    net/Packet.cpp
    net/NetEventHandler.cpp
    net/Common.cpp
    )
add_library(net::net ALIAS net)
target_include_directories(net
    PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}
    PUBLIC 
    $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/include>
    $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/third_party/enet/include>
    )

target_link_libraries(net 
    PUBLIC
    enet
    eventpp
)

if(WIN32)
    target_link_libraries(net 
        PRIVATE
        ws2_32.lib
        winmm.lib
    )
endif()


set_target_properties( net
    PROPERTIES
    ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/libs"
    LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/libs"
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin"
)