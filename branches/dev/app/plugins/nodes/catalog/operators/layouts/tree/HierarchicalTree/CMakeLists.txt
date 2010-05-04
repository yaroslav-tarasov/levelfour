project(HierarchicalTreeLayouter)
set(CMAKE_CONFIGURATION_TYPES Debug Release CACHE STRING "" FORCE) 
SET(CMAKE_DEBUG_POSTFIX _d)

include_directories(
			${CMAKE_CURRENT_SOURCE_DIR}
			)

# HierarchicalTreeLayouter
set( HierarchicalTreeLayouter_h ${HierarchicalTreeLayouter_h}
			HierarchicalTreeLayouterNode.h
			HierarchicalTreeLayouterNodePlugin.h
			)

set( HierarchicalTreeLayouter_moc ${HierarchicalTreeLayouter_moc}	
			HierarchicalTreeLayouterNode.h
			HierarchicalTreeLayouterNodePlugin.h
			)

set( HierarchicalTreeLayouter_src ${HierarchicalTreeLayouter_src}	
			HierarchicalTreeLayouterNode.cpp
			HierarchicalTreeLayouterNodePlugin.cpp
			)

set( HierarchicalTreeLayouter_res ${HierarchicalTreeLayouter_res}	
			HierarchicalTreeLayouter.xml
			)

# Create moc files		   
qt4_wrap_cpp(HierarchicalTreeLayouter_cxx ${HierarchicalTreeLayouter_moc})

# Create source groups
source_group("Moc Files" FILES ${HierarchicalTreeLayouter_cxx})
source_group("Header Files" FILES ${HierarchicalTreeLayouter_h})
source_group("Resources" FILES ${HierarchicalTreeLayouter_res})

# Add header files to sources to make headers visible in Visual Studio
IF (WIN32)
  set(HierarchicalTreeLayouter_src ${HierarchicalTreeLayouter_src} ${HierarchicalTreeLayouter_h} ${HierarchicalTreeLayouter_res})
ENDIF(WIN32)

# Create static library
add_library(HierarchicalTreeLayouter SHARED ${HierarchicalTreeLayouter_src} ${HierarchicalTreeLayouter_cxx})

# Add library dependencies
target_link_libraries(HierarchicalTreeLayouter
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
install( FILES ${HierarchicalTreeLayouter_res} DESTINATION ${NODE_PLUGIN_INSTALL} )
install( TARGETS HierarchicalTreeLayouter RUNTIME DESTINATION ${NODE_PLUGIN_INSTALL} )
