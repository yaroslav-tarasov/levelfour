project(camera)
set(CMAKE_CONFIGURATION_TYPES Debug Release CACHE STRING "" FORCE) 
SET(CMAKE_DEBUG_POSTFIX _d)

include_directories(
			${CMAKE_CURRENT_SOURCE_DIR}
			)

# camera
set( camera_h ${camera_h}		
			CameraNode.h
			CameraNodePlugin.h
			)

set( camera_moc ${camera_moc}	
			CameraNode.h
			CameraNodePlugin.h
			)

set( camera_src ${camera_src}	
			CameraNode.cpp
			CameraNodePlugin.cpp
			)

set( camera_res ${camera_res}	
			camera.xml
			)

# Create moc files		   
qt4_wrap_cpp(camera_cxx ${camera_moc})

# Create source groups
source_group("Moc Files" FILES ${camera_cxx})
source_group("Header Files" FILES ${camera_h})
source_group("Resources" FILES ${camera_res})

# Add header files to sources to make headers visible in Visual Studio
IF (WIN32)
  set(camera_src ${camera_src} ${camera_h} ${camera_res})
ENDIF(WIN32)

# Create static library
add_library(camera SHARED ${camera_src} ${camera_cxx})

# Add library dependencies
target_link_libraries(camera
			optimized QtCore4 debug QtCored4
			optimized QtGui4 debug QtGuid4
			optimized QtXml4 debug QtXmld4
			optimized OgreMain debug OgreMain_d
			optimized frappercore debug frappercore_d
			)

# Install files
install( FILES ${camera_res} DESTINATION ${NODE_PLUGIN_INSTALL} )
install( TARGETS camera RUNTIME DESTINATION ${NODE_PLUGIN_INSTALL} )