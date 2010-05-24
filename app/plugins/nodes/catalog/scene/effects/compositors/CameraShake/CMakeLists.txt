project(CameraShake)
set(CMAKE_CONFIGURATION_TYPES Debug Release CACHE STRING "" FORCE) 
SET(CMAKE_DEBUG_POSTFIX _d)

include_directories(
			${CMAKE_CURRENT_SOURCE_DIR}
			)

# CameraShake
set( CameraShake_h ${CameraShake_h}		
			CameraShakeNode.h
			CameraShakeNodePlugin.h
			)

set( CameraShake_moc ${CameraShake_moc}	
			CameraShakeNode.h
			CameraShakeNodePlugin.h
			)

set( CameraShake_src ${CameraShake_src}	
			CameraShakeNode.cpp
			CameraShakeNodePlugin.cpp
			)

set( CameraShake_res ${CameraShake_res}	
			CameraShake.xml
			)

# Create moc files		   
qt4_wrap_cpp(CameraShake_cxx ${CameraShake_moc})

# Create source groups
source_group("Moc Files" FILES ${CameraShake_cxx})
source_group("Header Files" FILES ${CameraShake_h})
source_group("Resources" FILES ${CameraShake_res})

# Add header files to sources to make headers visible in Visual Studio
IF (WIN32)
  set(CameraShake_src ${CameraShake_src} ${CameraShake_h} ${CameraShake_res})
ENDIF(WIN32)

# Create static library
add_library(CameraShake SHARED ${CameraShake_src} ${CameraShake_cxx})

# Add library dependencies
target_link_libraries(CameraShake
			optimized QtCore4 debug QtCored4
			optimized QtGui4 debug QtGuid4
			optimized QtXml4 debug QtXmld4
			optimized OgreMain debug OgreMain_d
			optimized frappercore debug frappercore_d
			)

# Install files
install( FILES ${CameraShake_res} DESTINATION ${NODE_PLUGIN_INSTALL} )
install( TARGETS CameraShake RUNTIME DESTINATION ${NODE_PLUGIN_INSTALL} )
