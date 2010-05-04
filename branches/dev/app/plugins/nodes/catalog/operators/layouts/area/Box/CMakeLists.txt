project(BoxLayouter)
set(CMAKE_CONFIGURATION_TYPES Debug Release CACHE STRING "" FORCE) 
SET(CMAKE_DEBUG_POSTFIX _d)

include_directories(
			${CMAKE_CURRENT_SOURCE_DIR}
			)

# BoxLayouter
set( BoxLayouter_h ${BoxLayouter_h}
			BoxLayouterNode.h
			BoxLayouterNodePlugin.h
			)

set( BoxLayouter_moc ${BoxLayouter_moc}	
			BoxLayouterNode.h
			BoxLayouterNodePlugin.h
			)

set( BoxLayouter_src ${BoxLayouter_src}	
			BoxLayouterNode.cpp
			BoxLayouterNodePlugin.cpp
			)

set( BoxLayouter_res ${BoxLayouter_res}	
			BoxLayouter.xml
			)

# Create moc files		   
qt4_wrap_cpp(BoxLayouter_cxx ${BoxLayouter_moc})

# Create source groups
source_group("Moc Files" FILES ${BoxLayouter_cxx})
source_group("Header Files" FILES ${BoxLayouter_h})
source_group("Resources" FILES ${BoxLayouter_res})

# Add header files to sources to make headers visible in Visual Studio
IF (WIN32)
  set(BoxLayouter_src ${BoxLayouter_src} ${BoxLayouter_h} ${BoxLayouter_res})
ENDIF(WIN32)

# Create static library
add_library(BoxLayouter SHARED ${BoxLayouter_src} ${BoxLayouter_cxx})

# Add library dependencies
target_link_libraries(BoxLayouter
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
install( FILES ${BoxLayouter_res} DESTINATION ${NODE_PLUGIN_INSTALL} )
install( TARGETS BoxLayouter RUNTIME DESTINATION ${NODE_PLUGIN_INSTALL} )
