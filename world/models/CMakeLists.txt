set (dirs
    wbrobot
    osrobot)

set (files
    wbr.model
    manifest.xml)

install(FILES ${files} DESTINATION ${GAZEBO_RESOURCE_PATH}/models)
install(DIRECTORY ${dirs} DESTINATION ${GAZEBO_RESOURCE_PATH}/models)
