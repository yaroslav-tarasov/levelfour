project(animationclip)
set(CMAKE_CONFIGURATION_TYPES Debug Release CACHE STRING "" FORCE) 
SET(CMAKE_DEBUG_POSTFIX _d)

include_directories(
			${CMAKE_CURRENT_SOURCE_DIR}
			${CMAKE_CURRENT_BINARY_DIR}
			)

# animationclip
set( animationclip_h ${animationclip_h}		
			AnimationClipNode.h
			AnimationClipNodePlugin.h
			)

set( animationclip_moc ${animationclip_moc}	
			AnimationClipNode.h
			AnimationClipNodePlugin.h
			)

set( animationclip_src ${animationclip_src}	
			AnimationClipNode.cpp
			AnimationClipNodePlugin.cpp
			)

set( animationclip_res ${animationclip_res}	
			animationclip.xml
			)

# Create moc files		   
qt4_wrap_cpp(animationclip_cxx ${animationclip_moc})

# Create source groups
source_group("Moc Files" FILES ${animationclip_cxx})
source_group("Header Files" FILES ${animationclip_h})
source_group("Resources" FILES ${animationclip_res})

# Add header files to sources to make headers visible in Visual Studio
IF (WIN32)
  set(animationclip_src ${animationclip_src} ${animationclip_h} ${animationclip_res})
ENDIF(WIN32)

# Create static library
add_library(animationclip SHARED ${animationclip_src} ${animationclip_cxx})

# Add library dependencies
target_link_libraries(animationclip
			optimized QtCore4 debug QtCored4
			optimized QtGui4 debug QtGuid4
			optimized QtXml4 debug QtXmld4
			optimized OgreMain debug OgreMain_d
			optimized frappercore debug frappercore_d
			)

# Install files
#IF (WIN32)
#	add_custom_command(TARGET animationclip POST_BUILD COMMAND copy \"${animationclip_BINARY_DIR}/$(ConfigurationName)\\*.dll\" \"${animationclip_SOURCE_DIR}/../../../bin/win32/plugins\" COMMAND copy \"${animationclip_SOURCE_DIR}\\*.xml\" \"${animationclip_SOURCE_DIR}/../../../bin/win32/plugins\")
#ENDIF(WIN32)
install( FILES ${animationclip_res} DESTINATION ${NODE_PLUGIN_INSTALL} )
install( TARGETS animationclip RUNTIME DESTINATION ${NODE_PLUGIN_INSTALL} )