cmake_minimum_required(VERSION 2.6)
set(CMAKE_ALLOW_LOOSE_LOOP_CONSTRUCTS TRUE)
cmake_policy(SET CMP0003 NEW)

project(nodekits)

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
option( FRAPPER_BUILD_NODES_3D "Build 3d nodes." TRUE )
option( FRAPPER_BUILD_NODES_AGENT "Build Agent nodes." TRUE )
option( FRAPPER_BUILD_NODES_COMPOSITING "Build Compositing nodes." TRUE )
option( FRAPPER_BUILD_NODES_INPUT "Build Input nodes." TRUE )
option( FRAPPER_BUILD_NODES_MISC "Build Misc nodes." TRUE )
option( FRAPPER_BUILD_NODES_NPR "Build NPR nodes." TRUE )
option( FRAPPER_BUILD_NODES_SHADING "Build Shading nodes." TRUE )
option( FRAPPER_BUILD_NODES_ADDITIONAL "Build Additional nodes." FALSE )
option( FRAPPER_BUILD_PANELS "Build Panel plugins." FALSE )
option( FRAPPER_BUILD_WIDGETS "Build Widget plugins." FALSE )

# set install dirs
IF(CMAKE_INSTALL_PREFIX_INITIALIZED_TO_DEFAULT)
SET(CMAKE_INSTALL_PREFIX ${CMAKE_CURRENT_SOURCE_DIR}/bin/win32 CACHE PATH "FOO install prefix" FORCE )
ENDIF(CMAKE_INSTALL_PREFIX_INITIALIZED_TO_DEFAULT)
#set ( CMAKE_INSTALL_PREFIX ${CMAKE_CURRENT_SOURCE_DIR}/bin/win32 )
set ( NODE_PLUGIN_INSTALL plugins/nodes )
set ( PANEL_PLUGIN_INSTALL plugins/panels )
set ( WIDGET_PLUGIN_INSTALL plugins/widgets )

# add sub-folders
add_subdirectory(plugins)

