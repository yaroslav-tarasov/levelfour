project(StackedTreeLayouter)
set(CMAKE_CONFIGURATION_TYPES Debug Release CACHE STRING "" FORCE) 
SET(CMAKE_DEBUG_POSTFIX _d)

include_directories(
			${CMAKE_CURRENT_SOURCE_DIR}
			)

# StackedTreeLayouter
set( StackedTreeLayouter_h ${StackedTreeLayouter_h}
			StackedTreeLayouterNode.h
			StackedTreeLayouterNodePlugin.h
			)

set( StackedTreeLayouter_moc ${StackedTreeLayouter_moc}	
			StackedTreeLayouterNode.h
			StackedTreeLayouterNodePlugin.h
			)

set( StackedTreeLayouter_src ${StackedTreeLayouter_src}	
			StackedTreeLayouterNode.cpp
			StackedTreeLayouterNodePlugin.cpp
			)

set( StackedTreeLayouter_res ${StackedTreeLayouter_res}	
			StackedTreeLayouter.xml
			)

# Create moc files		   
qt4_wrap_cpp(StackedTreeLayouter_cxx ${StackedTreeLayouter_moc})

# Create source groups
source_group("Moc Files" FILES ${StackedTreeLayouter_cxx})
source_group("Header Files" FILES ${StackedTreeLayouter_h})
source_group("Resources" FILES ${StackedTreeLayouter_res})

# Add header files to sources to make headers visible in Visual Studio
IF (WIN32)
  set(StackedTreeLayouter_src ${StackedTreeLayouter_src} ${StackedTreeLayouter_h} ${StackedTreeLayouter_res})
ENDIF(WIN32)

# Create static library
add_library(StackedTreeLayouter SHARED ${StackedTreeLayouter_src} ${StackedTreeLayouter_cxx})

# Add library dependencies
target_link_libraries(StackedTreeLayouter
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
install( FILES ${StackedTreeLayouter_res} DESTINATION ${NODE_PLUGIN_INSTALL} )
install( TARGETS StackedTreeLayouter RUNTIME DESTINATION ${NODE_PLUGIN_INSTALL} )
