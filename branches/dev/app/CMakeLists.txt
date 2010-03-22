cmake_minimum_required(VERSION 2.6)
set(CMAKE_ALLOW_LOOSE_LOOP_CONSTRUCTS TRUE)
cmake_policy(SET CMP0003 NEW)

project(levelfour)

# set debug and release, and debug postfix
#set(CMAKE_CONFIGURATION_TYPES Debug Release CACHE STRING "" FORCE) 
#SET(CMAKE_DEBUG_POSTFIX _d)

# find Qt4
find_package(Qt4 REQUIRED)
include(${QT_USE_FILE})

# global include directories
include_directories(  
			${frappercore_SOURCE_DIR} 
			${frappergui_SOURCE_DIR}
			${frappercore_BINARY_DIR}
			${frappergui_BINARY_DIR}
			$ENV{OGRE_HOME}/include
			${CMAKE_SOURCE_DIR}/thirdparty_libraries/collada/include
			${CMAKE_SOURCE_DIR}/thirdparty_libraries/collada/include/1.5
		    )

# global link directories
link_directories(
			${frappercore_BINARY_DIR}
			${frappergui_BINARY_DIR}
			${QT_LIBRARY_DIR}
			$ENV{OGRE_HOME}/lib
			${CMAKE_SOURCE_DIR}/thirdparty_libraries/collada/lib
			)

# build options


# set install dirs
IF(CMAKE_INSTALL_PREFIX_INITIALIZED_TO_DEFAULT)
SET(CMAKE_INSTALL_PREFIX ${CMAKE_CURRENT_SOURCE_DIR}/bin/win32 CACHE PATH "FOO install prefix" FORCE )
ENDIF(CMAKE_INSTALL_PREFIX_INITIALIZED_TO_DEFAULT)
#set ( CMAKE_INSTALL_PREFIX ${CMAKE_CURRENT_SOURCE_DIR}/bin/win32 )
set ( NODE_PLUGIN_INSTALL plugins/nodes )
set ( PANEL_PLUGIN_INSTALL plugins/panels )
set ( WIDGET_PLUGIN_INSTALL plugins/widgets )

# add sub-folders
add_subdirectory(applications)
add_subdirectory(core)
add_subdirectory(gui)
add_subdirectory(plugins)

