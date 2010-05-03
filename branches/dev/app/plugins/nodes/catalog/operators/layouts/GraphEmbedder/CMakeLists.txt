project(GraphEmbedderLayouter)
set(CMAKE_CONFIGURATION_TYPES Debug Release CACHE STRING "" FORCE) 
SET(CMAKE_DEBUG_POSTFIX _d)

include_directories(
			${CMAKE_CURRENT_SOURCE_DIR}
			)

# GraphEmbedderLayouter
set( GraphEmbedderLayouter_h ${GraphEmbedderLayouter_h}
			GraphEmbedderLayouterNode.h
			GraphEmbedderLayouterNodePlugin.h
			)

set( GraphEmbedderLayouter_moc ${GraphEmbedderLayouter_moc}	
			GraphEmbedderLayouterNode.h
			GraphEmbedderLayouterNodePlugin.h
			)

set( GraphEmbedderLayouter_src ${GraphEmbedderLayouter_src}	
			GraphEmbedderLayouterNode.cpp
			GraphEmbedderLayouterNodePlugin.cpp
			)

set( GraphEmbedderLayouter_res ${GraphEmbedderLayouter_res}	
			GraphEmbedderLayouter.xml
			)

# Create moc files		   
qt4_wrap_cpp(GraphEmbedderLayouter_cxx ${GraphEmbedderLayouter_moc})

# Create source groups
source_group("Moc Files" FILES ${GraphEmbedderLayouter_cxx})
source_group("Header Files" FILES ${GraphEmbedderLayouter_h})
source_group("Resources" FILES ${GraphEmbedderLayouter_res})

# Add header files to sources to make headers visible in Visual Studio
IF (WIN32)
  set(GraphEmbedderLayouter_src ${GraphEmbedderLayouter_src} ${GraphEmbedderLayouter_h} ${GraphEmbedderLayouter_res})
ENDIF(WIN32)

# Create static library
add_library(GraphEmbedderLayouter SHARED ${GraphEmbedderLayouter_src} ${GraphEmbedderLayouter_cxx})

# Add library dependencies
target_link_libraries(GraphEmbedderLayouter
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
install( FILES ${GraphEmbedderLayouter_res} DESTINATION ${NODE_PLUGIN_INSTALL} )
install( TARGETS GraphEmbedderLayouter RUNTIME DESTINATION ${NODE_PLUGIN_INSTALL} )
