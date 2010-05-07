project(TreeLayouter)
set(CMAKE_CONFIGURATION_TYPES Debug Release CACHE STRING "" FORCE) 
SET(CMAKE_DEBUG_POSTFIX _d)

include_directories(
			${CMAKE_CURRENT_SOURCE_DIR}
			)

# TreeLayouter
set( TreeLayouter_h ${TreeLayouter_h}
			TreeLayouterNode.h
			TreeLayouterNodePlugin.h
			)

set( TreeLayouter_moc ${TreeLayouter_moc}	
			TreeLayouterNode.h
			TreeLayouterNodePlugin.h
			)

set( TreeLayouter_src ${TreeLayouter_src}	
			TreeLayouterNode.cpp
			TreeLayouterNodePlugin.cpp
			)

set( TreeLayouter_res ${TreeLayouter_res}	
			TreeLayouter.xml
			)

# Create moc files		   
qt4_wrap_cpp(TreeLayouter_cxx ${TreeLayouter_moc})

# Create source groups
source_group("Moc Files" FILES ${TreeLayouter_cxx})
source_group("Header Files" FILES ${TreeLayouter_h})
source_group("Resources" FILES ${TreeLayouter_res})

# Add header files to sources to make headers visible in Visual Studio
IF (WIN32)
  set(TreeLayouter_src ${TreeLayouter_src} ${TreeLayouter_h} ${TreeLayouter_res})
ENDIF(WIN32)

# Create static library
add_library(TreeLayouter SHARED ${TreeLayouter_src} ${TreeLayouter_cxx})

# Add library dependencies
target_link_libraries(TreeLayouter
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
install( FILES ${TreeLayouter_res} DESTINATION ${NODE_PLUGIN_INSTALL} )
install( TARGETS TreeLayouter RUNTIME DESTINATION ${NODE_PLUGIN_INSTALL} )
