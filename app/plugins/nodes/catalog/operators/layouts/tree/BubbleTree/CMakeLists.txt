project(BubbleTreeLayouter)
set(CMAKE_CONFIGURATION_TYPES Debug Release CACHE STRING "" FORCE) 
SET(CMAKE_DEBUG_POSTFIX _d)

include_directories(
			${CMAKE_CURRENT_SOURCE_DIR}
			)

# BubbleTreeLayouter
set( BubbleTreeLayouter_h ${BubbleTreeLayouter_h}
			BubbleTreeLayouterNode.h
			BubbleTreeLayouterNodePlugin.h
			)

set( BubbleTreeLayouter_moc ${BubbleTreeLayouter_moc}	
			BubbleTreeLayouterNode.h
			BubbleTreeLayouterNodePlugin.h
			)

set( BubbleTreeLayouter_src ${BubbleTreeLayouter_src}	
			BubbleTreeLayouterNode.cpp
			BubbleTreeLayouterNodePlugin.cpp
			)

set( BubbleTreeLayouter_res ${BubbleTreeLayouter_res}	
			BubbleTreeLayouter.xml
			)

# Create moc files		   
qt4_wrap_cpp(BubbleTreeLayouter_cxx ${BubbleTreeLayouter_moc})

# Create source groups
source_group("Moc Files" FILES ${BubbleTreeLayouter_cxx})
source_group("Header Files" FILES ${BubbleTreeLayouter_h})
source_group("Resources" FILES ${BubbleTreeLayouter_res})

# Add header files to sources to make headers visible in Visual Studio
IF (WIN32)
  set(BubbleTreeLayouter_src ${BubbleTreeLayouter_src} ${BubbleTreeLayouter_h} ${BubbleTreeLayouter_res})
ENDIF(WIN32)

# Create static library
add_library(BubbleTreeLayouter SHARED ${BubbleTreeLayouter_src} ${BubbleTreeLayouter_cxx})

# Add library dependencies
target_link_libraries(BubbleTreeLayouter
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
install( FILES ${BubbleTreeLayouter_res} DESTINATION ${NODE_PLUGIN_INSTALL} )
install( TARGETS BubbleTreeLayouter RUNTIME DESTINATION ${NODE_PLUGIN_INSTALL} )
