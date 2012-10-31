add_subdirectory(models)
set (files
    wbrobots.world)

install(FILES ${files} DESTINATION ${GAZEBO_RESOURCE_PATH}/worlds)
