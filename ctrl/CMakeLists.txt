set(OSRobot osrobot)
set(GUIControl guicamera)
add_executable(${OSRobot} osrobot.cc)
add_executable(${GUIControl} guicamera.cc)

target_link_libraries(${OSRobot} ${GAZEBO_LIBRARIES} ${Boost_LIBRARIES} protobuf)
target_link_libraries(${GUIControl} ${GAZEBO_LIBRARIES} ${Boost_LIBRARIES} protobuf)
message("Gazebo libraries: ${GAZEBO_LIBRARIES}")
#install(TARGETS ${OSRobot} DESTINATION ${GAZEBO_PLUGIN_PATH})
