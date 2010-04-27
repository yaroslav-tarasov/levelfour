project(DendogramLayouter)
set(CMAKE_CONFIGURATION_TYPES Debug Release CACHE STRING "" FORCE) 
SET(CMAKE_DEBUG_POSTFIX _d)

include_directories(
			${CMAKE_CURRENT_SOURCE_DIR}
			)

# DendogramLayouter
set( DendogramLayouter_h ${DendogramLayouter_h}
			DendogramLayouterNode.h
			DendogramLayouterNodePlugin.h
			)

set( DendogramLayouter_moc ${DendogramLayouter_moc}	
			DendogramLayouterNode.h
			DendogramLayouterNodePlugin.h
			)

set( DendogramLayouter_src ${DendogramLayouter_src}	
			DendogramLayouterNode.cpp
			DendogramLayouterNodePlugin.cpp
			)

set( DendogramLayouter_res ${DendogramLayouter_res}	
			DendogramLayouter.xml
			)

# Create moc files		   
qt4_wrap_cpp(DendogramLayouter_cxx ${DendogramLayouter_moc})

# Create source groups
source_group("Moc Files" FILES ${DendogramLayouter_cxx})
source_group("Header Files" FILES ${DendogramLayouter_h})
source_group("Resources" FILES ${DendogramLayouter_res})

# Add header files to sources to make headers visible in Visual Studio
IF (WIN32)
  set(DendogramLayouter_src ${DendogramLayouter_src} ${DendogramLayouter_h} ${DendogramLayouter_res})
ENDIF(WIN32)

# Create static library
add_library(DendogramLayouter SHARED ${DendogramLayouter_src} ${DendogramLayouter_cxx})

# Add library dependencies
target_link_libraries(DendogramLayouter
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
install( FILES ${DendogramLayouter_res} DESTINATION ${NODE_PLUGIN_INSTALL} )
install( TARGETS DendogramLayouter RUNTIME DESTINATION ${NODE_PLUGIN_INSTALL} )
