project(light)
set(CMAKE_CONFIGURATION_TYPES Debug Release CACHE STRING "" FORCE) 
SET(CMAKE_DEBUG_POSTFIX _d)

include_directories(
			${CMAKE_CURRENT_SOURCE_DIR}
			)

# light
set( light_h ${light_h}		
			LightNode.h
			LightNodePlugin.h
			)

set( light_moc ${light_moc}	
			LightNode.h
			LightNodePlugin.h
			)

set( light_src ${light_src}	
			LightNode.cpp
			LightNodePlugin.cpp
			)

set( light_res ${light_res}	
			light.xml
			)

# Create moc files		   
qt4_wrap_cpp(light_cxx ${light_moc})

# Create source groups
source_group("Moc Files" FILES ${light_cxx})
source_group("Header Files" FILES ${light_h})
source_group("Resources" FILES ${light_res})

# Add header files to sources to make headers visible in Visual Studio
IF (WIN32)
  set(light_src ${light_src} ${light_h} ${light_res})
ENDIF(WIN32)

# Create static library
add_library(light SHARED ${light_src} ${light_cxx})

# Add library dependencies
target_link_libraries(light
			optimized QtCore4 debug QtCored4
			optimized QtGui4 debug QtGuid4
			optimized QtXml4 debug QtXmld4
			optimized OgreMain debug OgreMain_d
			optimized frappercore debug frappercore_d
			)

# Install files
#IF (WIN32)
#	add_custom_command(TARGET light POST_BUILD COMMAND copy \"${light_BINARY_DIR}/$(ConfigurationName)\\*.dll\" \"${light_SOURCE_DIR}/../../../bin/win32/plugins\" COMMAND copy \"${light_SOURCE_DIR}\\*.xml\" \"${light_SOURCE_DIR}/../../../bin/win32/plugins\")
#ENDIF(WIN32)
install( FILES ${light_res} DESTINATION ${NODE_PLUGIN_INSTALL} )
install( TARGETS light RUNTIME DESTINATION ${NODE_PLUGIN_INSTALL} )