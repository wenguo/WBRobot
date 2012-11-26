set(OSRobot osrobot)
set(GUIControl guicamera)

include_directories(
    ${GAZEBO_INCLUDE_DIRS}
    ${CMAKE_CURRENT_BINARY_DIR}/../plugins/msgs
    )

add_executable(${OSRobot} osrobot.cc)
add_executable(${GUIControl} guicamera.cc)

target_link_libraries(${OSRobot} my_msgs ${GAZEBO_LIBRARIES} ${Boost_LIBRARIES} protobuf)
target_link_libraries(${GUIControl} ${GAZEBO_LIBRARIES} ${Boost_LIBRARIES} protobuf)
message("Gazebo libraries: ${GAZEBO_LIBRARIES}")
#install(TARGETS ${OSRobot} DESTINATION ${GAZEBO_PLUGIN_PATH})
