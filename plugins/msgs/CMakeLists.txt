find_package(Protobuf REQUIRED)

set(msgs req.proto)
PROTOBUF_GENERATE_CPP(PROTO_SRCS PROTO_HDRS ${msgs})
add_library(${my_msg} SHARED ${PROTO_SRCS})
target_link_libraries(${my_msg} ${PROTOBUF_LIBRARY})

install(TARGETS ${my_msg} DESTINATION ${CMAKE_INSTALL_PREFIX}/lib)
