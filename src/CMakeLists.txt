set(WBRobotController wbr_controller)
set(WBRobotFactory wbr_factory)

add_library(${WBRobotController} SHARED controller.cc)
add_library(${WBRobotFactory} SHARED robot_factory.cc)

target_link_libraries(${WBRobotController} ${GAZEBO_libraries})
target_link_libraries(${WBRobotFactory} ${GAZEBO_libraries})
install(TARGETS ${WBRobotController} DESTINATION ${GAZEBO_PLUGIN_PATH})
install(TARGETS ${WBRobotFactory} DESTINATION ${GAZEBO_PLUGIN_PATH})
