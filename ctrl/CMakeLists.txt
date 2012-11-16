########################################
# Find Boost, if not specified manually
include(FindBoost)
find_package(Boost ${MIN_BOOST_VERSION} REQUIRED thread signals system filesystem program_options regex iostreams)

if (NOT Boost_FOUND)
  set (BUILD_GAZEBO OFF CACHE INTERNAL "Build Gazebo" FORCE)
  BUILD_ERROR ("Boost not found. Please install thread signals system filesystem program_options regex boost version ${MIN_BOOST_VERSION} or higher.")
endif()

set(OSRobot osrobot)
add_executable(${OSRobot} osrobot.cc)

target_link_libraries(${OSRobot} ${GAZEBO_LIBRARIES} ${Boost_LIBRARIES} protobuf)
message("Gazebo libraries: ${GAZEBO_LIBRARIES}")
#install(TARGETS ${OSRobot} DESTINATION ${GAZEBO_PLUGIN_PATH})
