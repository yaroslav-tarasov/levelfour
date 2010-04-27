project(SliceAndDiceLayouter)
set(CMAKE_CONFIGURATION_TYPES Debug Release CACHE STRING "" FORCE) 
SET(CMAKE_DEBUG_POSTFIX _d)

include_directories(
			${CMAKE_CURRENT_SOURCE_DIR}
			)

# SliceAndDiceLayouter
set( SliceAndDiceLayouter_h ${SliceAndDiceLayouter_h}
			SliceAndDiceLayouterNode.h
			SliceAndDiceLayouterNodePlugin.h
			)

set( SliceAndDiceLayouter_moc ${SliceAndDiceLayouter_moc}	
			SliceAndDiceLayouterNode.h
			SliceAndDiceLayouterNodePlugin.h
			)

set( SliceAndDiceLayouter_src ${SliceAndDiceLayouter_src}	
			SliceAndDiceLayouterNode.cpp
			SliceAndDiceLayouterNodePlugin.cpp
			)

set( SliceAndDiceLayouter_res ${SliceAndDiceLayouter_res}	
			SliceAndDiceLayouter.xml
			)

# Create moc files		   
qt4_wrap_cpp(SliceAndDiceLayouter_cxx ${SliceAndDiceLayouter_moc})

# Create source groups
source_group("Moc Files" FILES ${SliceAndDiceLayouter_cxx})
source_group("Header Files" FILES ${SliceAndDiceLayouter_h})
source_group("Resources" FILES ${SliceAndDiceLayouter_res})

# Add header files to sources to make headers visible in Visual Studio
IF (WIN32)
  set(SliceAndDiceLayouter_src ${SliceAndDiceLayouter_src} ${SliceAndDiceLayouter_h} ${SliceAndDiceLayouter_res})
ENDIF(WIN32)

# Create static library
add_library(SliceAndDiceLayouter SHARED ${SliceAndDiceLayouter_src} ${SliceAndDiceLayouter_cxx})

# Add library dependencies
target_link_libraries(SliceAndDiceLayouter
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
install( FILES ${SliceAndDiceLayouter_res} DESTINATION ${NODE_PLUGIN_INSTALL} )
install( TARGETS SliceAndDiceLayouter RUNTIME DESTINATION ${NODE_PLUGIN_INSTALL} )
