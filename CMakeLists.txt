cmake_minimum_required(VERSION 2.8 FATAL_ERROR)

include (FindPkgConfig)
if (PKG_CONFIG_FOUND)
  pkg_check_modules(GAZEBO gazebo)
endif()
include_directories(${GAZEBO_INCLUDE_DIRS})
link_directories(${GAZEBO_LIBRARY_DIRS})

#change this according to your gazebo installation
set (GAZEBO_PLUGIN_PATH /usr/lib/gazebo-1.2/plugins)
set (GAZEBO_RESOURCE_PATH /usr/share/gazebo-1.2)

add_subdirectory(src)
add_subdirectory(world)


