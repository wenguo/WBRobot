include_directories(
  ${GAZEBO_INCLUDE_DIRS}
  ${CMAKE_CURRENT_BINARY_DIR}/msgs
  )

add_subdirectory(msgs)

set(WBRobotController wbr_controller)
set(WBRobotFactory wbr_factory)
set(OSRobotMotion osr_motion)
set(OSRobotVisual osr_visual)
set(OSRobotFactory osr_factory)
set(OSRobotController osr_controller)

add_library(${WBRobotController} MODULE controller.cc)
add_library(${WBRobotFactory} MODULE robot_factory.cc)
add_library(${OSRobotMotion} MODULE osrobot_motion.cc)
add_library(${OSRobotVisual} MODULE osrobot_visual.cc)
add_library(${OSRobotFactory} MODULE osrobot_factory.cc)
add_library(${OSRobotController} MODULE osrobot_controller.cc)

target_link_libraries(${WBRobotController} ${GAZEBO_LIBRARIES} ${Boost_LIBRARIES})
target_link_libraries(${WBRobotFactory} ${GAZEBO_LIBRARIES} ${Boost_LIBRARIES})
target_link_libraries(${OSRobotMotion} my_msgs ${GAZEBO_LIBRARIES} ${Boost_LIBRARIES})
target_link_libraries(${OSRobotVisual} my_msgs ${GAZEBO_LIBRARIES} ${Boost_LIBRARIES})
target_link_libraries(${OSRobotFactory} ${GAZEBO_LIBRARIES} ${Boost_LIBRARIES})
target_link_libraries(${OSRobotController} ${GAZEBO_LIBRARIES} ${Boost_LIBRARIES})
install(TARGETS ${WBRobotController} DESTINATION ${GAZEBO_PLUGIN_PATH})
install(TARGETS ${WBRobotFactory} DESTINATION ${GAZEBO_PLUGIN_PATH})
install(TARGETS ${OSRobotMotion} DESTINATION ${GAZEBO_PLUGIN_PATH})
install(TARGETS ${OSRobotVisual} DESTINATION ${GAZEBO_PLUGIN_PATH})
install(TARGETS ${OSRobotFactory} DESTINATION ${GAZEBO_PLUGIN_PATH})
install(TARGETS ${OSRobotController} DESTINATION ${GAZEBO_PLUGIN_PATH})
