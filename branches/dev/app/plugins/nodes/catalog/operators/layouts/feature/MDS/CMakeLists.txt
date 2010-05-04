project(MdsLayouter)
set(CMAKE_CONFIGURATION_TYPES Debug Release CACHE STRING "" FORCE) 
SET(CMAKE_DEBUG_POSTFIX _d)

include_directories(
			${CMAKE_CURRENT_SOURCE_DIR}
			)

# MdsLayouter
set( MdsLayouter_h ${MdsLayouter_h}
			MdsLayouterNode.h
			MdsLayouterNodePlugin.h
			)

set( MdsLayouter_moc ${MdsLayouter_moc}	
			MdsLayouterNode.h
			MdsLayouterNodePlugin.h
			)

set( MdsLayouter_src ${MdsLayouter_src}	
			MdsLayouterNode.cpp
			MdsLayouterNodePlugin.cpp
			)

set( MdsLayouter_res ${MdsLayouter_res}	
			MdsLayouter.xml
			)

# Create moc files		   
qt4_wrap_cpp(MdsLayouter_cxx ${MdsLayouter_moc})

# Create source groups
source_group("Moc Files" FILES ${MdsLayouter_cxx})
source_group("Header Files" FILES ${MdsLayouter_h})
source_group("Resources" FILES ${MdsLayouter_res})

# Add header files to sources to make headers visible in Visual Studio
IF (WIN32)
  set(MdsLayouter_src ${MdsLayouter_src} ${MdsLayouter_h} ${MdsLayouter_res})
ENDIF(WIN32)

# Create static library
add_library(MdsLayouter SHARED ${MdsLayouter_src} ${MdsLayouter_cxx})

# Add library dependencies
target_link_libraries(MdsLayouter
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
install( FILES ${MdsLayouter_res} DESTINATION ${NODE_PLUGIN_INSTALL} )
install( TARGETS MdsLayouter RUNTIME DESTINATION ${NODE_PLUGIN_INSTALL} )
