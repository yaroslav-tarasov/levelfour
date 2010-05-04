project(ForceDirectedLayouter)
set(CMAKE_CONFIGURATION_TYPES Debug Release CACHE STRING "" FORCE) 
SET(CMAKE_DEBUG_POSTFIX _d)

include_directories(
			${CMAKE_CURRENT_SOURCE_DIR}
			)

# ForceDirectedLayouter
set( ForceDirectedLayouter_h ${ForceDirectedLayouter_h}
			ForceDirectedLayouterNode.h
			ForceDirectedLayouterNodePlugin.h
			)

set( ForceDirectedLayouter_moc ${ForceDirectedLayouter_moc}	
			ForceDirectedLayouterNode.h
			ForceDirectedLayouterNodePlugin.h
			)

set( ForceDirectedLayouter_src ${ForceDirectedLayouter_src}	
			ForceDirectedLayouterNode.cpp
			ForceDirectedLayouterNodePlugin.cpp
			)

set( ForceDirectedLayouter_res ${ForceDirectedLayouter_res}	
			ForceDirectedLayouter.xml
			)

# Create moc files		   
qt4_wrap_cpp(ForceDirectedLayouter_cxx ${ForceDirectedLayouter_moc})

# Create source groups
source_group("Moc Files" FILES ${ForceDirectedLayouter_cxx})
source_group("Header Files" FILES ${ForceDirectedLayouter_h})
source_group("Resources" FILES ${ForceDirectedLayouter_res})

# Add header files to sources to make headers visible in Visual Studio
IF (WIN32)
  set(ForceDirectedLayouter_src ${ForceDirectedLayouter_src} ${ForceDirectedLayouter_h} ${ForceDirectedLayouter_res})
ENDIF(WIN32)

# Create static library
add_library(ForceDirectedLayouter SHARED ${ForceDirectedLayouter_src} ${ForceDirectedLayouter_cxx})

# Add library dependencies
target_link_libraries(ForceDirectedLayouter
			optimized QtCore4 debug QtCored4
			optimized QtGui4 debug QtGuid4
			optimized QtXml4 debug QtXmld4
			optimized OgreMain debug OgreMain_d
			optimized frappercore debug frappercore_d
			optimized vtkFiltering debug vtkFiltering
			optimized vtkInfovis debug vtkInfovis
			optimized vtkCommon debug vtkCommon
			)

# Install files
install( FILES ${ForceDirectedLayouter_res} DESTINATION ${NODE_PLUGIN_INSTALL} )
install( TARGETS ForceDirectedLayouter RUNTIME DESTINATION ${NODE_PLUGIN_INSTALL} )
