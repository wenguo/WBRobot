set(OSRobot osrobot)
add_executable(${OSRobot} osrobot.cc)

target_link_libraries(${OSRobot} ${GAZEBO_LIBRARIES} ${Boost_LIBRARIES} protobuf)
message("Gazebo libraries: ${GAZEBO_LIBRARIES}")
#install(TARGETS ${OSRobot} DESTINATION ${GAZEBO_PLUGIN_PATH})
