project(compositortemplate)
set(CMAKE_CONFIGURATION_TYPES Debug Release CACHE STRING "" FORCE) 
SET(CMAKE_DEBUG_POSTFIX _d)

include_directories(
			${CMAKE_CURRENT_SOURCE_DIR}
			)

# compositortemplate
set( compositortemplate_h ${compositortemplate_h}		
			CompositorTemplateNode.h
			CompositorTemplateNodePlugin.h
			)

set( compositortemplate_moc ${compositortemplate_moc}	
			CompositorTemplateNode.h
			CompositorTemplateNodePlugin.h
			)

set( compositortemplate_src ${compositortemplate_src}	
			CompositorTemplateNode.cpp
			CompositorTemplateNodePlugin.cpp
			)

set( compositortemplate_res ${compositortemplate_res}	
			compositortemplate.xml
			)

# Create moc files		   
qt4_wrap_cpp(compositortemplate_cxx ${compositortemplate_moc})

# Create source groups
source_group("Moc Files" FILES ${compositortemplate_cxx})
source_group("Header Files" FILES ${compositortemplate_h})
source_group("Resources" FILES ${compositortemplate_res})

# Add header files to sources to make headers visible in Visual Studio
IF (WIN32)
  set(compositortemplate_src ${compositortemplate_src} ${compositortemplate_h} ${compositortemplate_res})
ENDIF(WIN32)

# Create static library
add_library(compositortemplate SHARED ${compositortemplate_src} ${compositortemplate_cxx})

# Add library dependencies
target_link_libraries(compositortemplate
			optimized QtCore4 debug QtCored4
			optimized QtGui4 debug QtGuid4
			optimized QtXml4 debug QtXmld4
			optimized OgreMain debug OgreMain_d
			optimized frappercore debug frappercore_d
			)

# Install files
install( FILES ${compositortemplate_res} DESTINATION ${NODE_PLUGIN_INSTALL} )
install( TARGETS compositortemplate RUNTIME DESTINATION ${NODE_PLUGIN_INSTALL} )
