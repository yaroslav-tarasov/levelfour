project(advancedrender)
set(CMAKE_CONFIGURATION_TYPES Debug Release CACHE STRING "" FORCE) 
SET(CMAKE_DEBUG_POSTFIX _d)

include_directories(
			${CMAKE_CURRENT_SOURCE_DIR}
			)

# advancedrender
set( advancedrender_h ${advancedrender_h}
			AdvancedRenderNode.h
			AdvancedRenderNodePlugin.h
			)

set( advancedrender_moc ${advancedrender_moc}
			AdvancedRenderNode.h
			AdvancedRenderNodePlugin.h
			)

set( advancedrender_src ${advancedrender_src}	
			AdvancedRenderNode.cpp
			AdvancedRenderNodePlugin.cpp
			)
			
set( advancedrender_res ${advancedrender_res}	
			advancedrender.xml
			)

# Create moc files
qt4_wrap_cpp(advancedrender_cxx ${advancedrender_moc})

# Create source groups
source_group("Moc Files" FILES ${advancedrender_cxx})
source_group("Header Files" FILES ${advancedrender_h})
source_group("Resources" FILES ${advancedrender_res})

# Add header files to sources to make headers visible in Visual Studio
IF (WIN32)
  set(advancedrender_src ${advancedrender_src} ${advancedrender_h} ${advancedrender_res})
ENDIF(WIN32)

# Create static library
add_library(advancedrender SHARED ${advancedrender_src} ${advancedrender_cxx})

# Add library dependencies
target_link_libraries(advancedrender
			optimized QtCore4 debug QtCored4
			optimized QtGui4 debug QtGuid4
			optimized QtXml4 debug QtXmld4
			optimized OgreMain debug OgreMain_d
			optimized frappercore debug frappercore_d
			)

# Install files
#IF (WIN32)
#	add_custom_command(TARGET advancedrender POST_BUILD COMMAND copy \"${advancedrender_BINARY_DIR}/$(ConfigurationName)\\*.dll\" \"${advancedrender_SOURCE_DIR}/../../../bin/win32/plugins\" COMMAND copy \"${advancedrender_SOURCE_DIR}\\*.xml\" \"${advancedrender_SOURCE_DIR}/../../../bin/win32/plugins\")
#ENDIF(WIN32)
install( FILES ${advancedrender_res} DESTINATION ${NODE_PLUGIN_INSTALL} )
install( TARGETS advancedrender RUNTIME DESTINATION ${NODE_PLUGIN_INSTALL} )