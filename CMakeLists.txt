cmake_minimum_required(VERSION 2.8 FATAL_ERROR)

include (FindPkgConfig)
if (PKG_CONFIG_FOUND)
  pkg_check_modules(GAZEBO gazebo)
endif()
include_directories(${GAZEBO_INCLUDE_DIRS})
link_directories(${GAZEBO_LIBRARY_DIRS})

########################################
# Find Boost, if not specified manually
include(FindBoost)
find_package(Boost ${MIN_BOOST_VERSION} REQUIRED thread signals system filesystem program_options regex iostreams)

if (NOT Boost_FOUND)
  set (BUILD_GAZEBO OFF CACHE INTERNAL "Build Gazebo" FORCE)
  BUILD_ERROR ("Boost not found. Please install thread signals system filesystem program_options regex boost version ${MIN_BOOST_VERSION} or higher.")
endif()

set (CMAKE_C_FLAGS "-W -O2 -fPIC")
set (CMAKE_CXX_FLAGS "-W -O2 -fPIC")
set (CMAKE_EXE_LINKER_FLAGS "-undefined dynamic_lookup")
set (CMAKE_SHARED_LINKER_FLAGS "-undefined dynamic_lookup")
set (CMAKE_MODULE_LINKER_FLAGS "-undefined dynamic_lookup")

#change this according to your gazebo installation
set (GAZEBO_PLUGIN_PATH /usr/local/lib/gazebo-1.2/plugins)
set (GAZEBO_RESOURCE_PATH /usr/local/share/gazebo-1.2)

add_subdirectory(plugins)
add_subdirectory(ctrl)
add_subdirectory(world)


