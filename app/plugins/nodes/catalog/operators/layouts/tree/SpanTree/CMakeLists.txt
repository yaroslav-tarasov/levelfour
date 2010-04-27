project(SpanTreeLayouter)
set(CMAKE_CONFIGURATION_TYPES Debug Release CACHE STRING "" FORCE) 
SET(CMAKE_DEBUG_POSTFIX _d)

include_directories(
			${CMAKE_CURRENT_SOURCE_DIR}
			)

# SpanTreeLayouter
set( SpanTreeLayouter_h ${SpanTreeLayouter_h}
			SpanTreeLayouterNode.h
			SpanTreeLayouterNodePlugin.h
			)

set( SpanTreeLayouter_moc ${SpanTreeLayouter_moc}	
			SpanTreeLayouterNode.h
			SpanTreeLayouterNodePlugin.h
			)

set( SpanTreeLayouter_src ${SpanTreeLayouter_src}	
			SpanTreeLayouterNode.cpp
			SpanTreeLayouterNodePlugin.cpp
			)

set( SpanTreeLayouter_res ${SpanTreeLayouter_res}	
			SpanTreeLayouter.xml
			)

# Create moc files		   
qt4_wrap_cpp(SpanTreeLayouter_cxx ${SpanTreeLayouter_moc})

# Create source groups
source_group("Moc Files" FILES ${SpanTreeLayouter_cxx})
source_group("Header Files" FILES ${SpanTreeLayouter_h})
source_group("Resources" FILES ${SpanTreeLayouter_res})

# Add header files to sources to make headers visible in Visual Studio
IF (WIN32)
  set(SpanTreeLayouter_src ${SpanTreeLayouter_src} ${SpanTreeLayouter_h} ${SpanTreeLayouter_res})
ENDIF(WIN32)

# Create static library
add_library(SpanTreeLayouter SHARED ${SpanTreeLayouter_src} ${SpanTreeLayouter_cxx})

# Add library dependencies
target_link_libraries(SpanTreeLayouter
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
install( FILES ${SpanTreeLayouter_res} DESTINATION ${NODE_PLUGIN_INSTALL} )
install( TARGETS SpanTreeLayouter RUNTIME DESTINATION ${NODE_PLUGIN_INSTALL} )
