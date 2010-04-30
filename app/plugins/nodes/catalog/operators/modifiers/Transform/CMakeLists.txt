project(Transform)
set(CMAKE_CONFIGURATION_TYPES Debug Release CACHE STRING "" FORCE) 
SET(CMAKE_DEBUG_POSTFIX _d)

include_directories(
			${CMAKE_CURRENT_SOURCE_DIR}
			)

# Transform
set( Transform_h ${Transform_h}		
			TransformNode.h
			TransformNodePlugin.h
			)

set( Transform_moc ${Transform_moc}	
			TransformNode.h
			TransformNodePlugin.h
			)

set( Transform_src ${Transform_src}	
			TransformNode.cpp
			TransformNodePlugin.cpp
			)

set( Transform_res ${Transform_res}	
			Transform.xml
			)

# Create moc files		   
qt4_wrap_cpp(Transform_cxx ${Transform_moc})

# Create source groups
source_group("Moc Files" FILES ${Transform_cxx})
source_group("Header Files" FILES ${Transform_h})
source_group("Resources" FILES ${Transform_res})

# Add header files to sources to make headers visible in Visual Studio
IF (WIN32)
  set(Transform_src ${Transform_src} ${Transform_h} ${Transform_res})
ENDIF(WIN32)

# Create static library
add_library(Transform SHARED ${Transform_src} ${Transform_cxx})

# Add library dependencies
target_link_libraries(Transform
			optimized QtCore4 debug QtCored4
			optimized QtGui4 debug QtGuid4
			optimized QtXml4 debug QtXmld4
			optimized OgreMain debug OgreMain_d
			optimized frappercore debug frappercore_d
			)

# Install files
#IF (WIN32)
#	add_custom_command(TARGET Transform POST_BUILD COMMAND copy \"${Transform_BINARY_DIR}/$(ConfigurationName)\\*.dll\" \"${Transform_SOURCE_DIR}/../../../bin/win32/plugins\" COMMAND copy \"${Transform_SOURCE_DIR}\\*.xml\" \"${Transform_SOURCE_DIR}/../../../bin/win32/plugins\")
#ENDIF(WIN32)
install( FILES ${Transform_res} DESTINATION ${NODE_PLUGIN_INSTALL} )
install( TARGETS Transform RUNTIME DESTINATION ${NODE_PLUGIN_INSTALL} )