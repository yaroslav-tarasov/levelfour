project(transform)
set(CMAKE_CONFIGURATION_TYPES Debug Release CACHE STRING "" FORCE) 
SET(CMAKE_DEBUG_POSTFIX _d)

include_directories(
			${CMAKE_CURRENT_SOURCE_DIR}
			)

# transform
set( transform_h ${transform_h}		
			TransformNode.h
			TransformNodePlugin.h
			)

set( transform_moc ${transform_moc}	
			TransformNode.h
			TransformNodePlugin.h
			)

set( transform_src ${transform_src}	
			TransformNode.cpp
			TransformNodePlugin.cpp
			)

set( transform_res ${transform_res}	
			transform.xml
			)

# Create moc files		   
qt4_wrap_cpp(transform_cxx ${transform_moc})

# Create source groups
source_group("Moc Files" FILES ${transform_cxx})
source_group("Header Files" FILES ${transform_h})
source_group("Resources" FILES ${transform_res})

# Add header files to sources to make headers visible in Visual Studio
IF (WIN32)
  set(transform_src ${transform_src} ${transform_h} ${transform_res})
ENDIF(WIN32)

# Create static library
add_library(transform SHARED ${transform_src} ${transform_cxx})

# Add library dependencies
target_link_libraries(transform
			optimized QtCore4 debug QtCored4
			optimized QtGui4 debug QtGuid4
			optimized QtXml4 debug QtXmld4
			optimized OgreMain debug OgreMain_d
			optimized frappercore debug frappercore_d
			)

# Install files
#IF (WIN32)
#	add_custom_command(TARGET transform POST_BUILD COMMAND copy \"${transform_BINARY_DIR}/$(ConfigurationName)\\*.dll\" \"${transform_SOURCE_DIR}/../../../bin/win32/plugins\" COMMAND copy \"${transform_SOURCE_DIR}\\*.xml\" \"${transform_SOURCE_DIR}/../../../bin/win32/plugins\")
#ENDIF(WIN32)
install( FILES ${transform_res} DESTINATION ${NODE_PLUGIN_INSTALL} )
install( TARGETS transform RUNTIME DESTINATION ${NODE_PLUGIN_INSTALL} )