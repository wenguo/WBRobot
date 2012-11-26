find_package(Protobuf REQUIRED)

set (msgs req.proto)
PROTOBUF_GENERATE_CPP(PROTO_SRCS PROTO_HDRS ${msgs})
add_library(my_msgs SHARED ${PROTO_SRCS})
target_link_libraries(my_msgs ${PROTOBUF_LIBRARY})

install(TARGETS my_msgs DESTINATION ${GAZEBO_PLUGIN_PATH})
