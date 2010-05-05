project(TableToGraph)
set(CMAKE_CONFIGURATION_TYPES Debug Release CACHE STRING "" FORCE) 
SET(CMAKE_DEBUG_POSTFIX _d)

include_directories(
			${CMAKE_CURRENT_SOURCE_DIR}
			)

# TableToGraph
set( TableToGraph_h ${TableToGraph_h}
			TableToGraphNode.h
			TableToGraphNodePlugin.h
			)

set( TableToGraph_moc ${TableToGraph_moc}	
			TableToGraphNode.h
			TableToGraphNodePlugin.h
			)

set( TableToGraph_src ${TableToGraph_src}	
			TableToGraphNode.cpp
			TableToGraphNodePlugin.cpp
			)

set( TableToGraph_res ${TableToGraph_res}	
			TableToGraph.xml
			)

# Create moc files		   
qt4_wrap_cpp(TableToGraph_cxx ${TableToGraph_moc})

# Create source groups
source_group("Moc Files" FILES ${TableToGraph_cxx})
source_group("Header Files" FILES ${TableToGraph_h})
source_group("Resources" FILES ${TableToGraph_res})

# Add header files to sources to make headers visible in Visual Studio
IF (WIN32)
  set(TableToGraph_src ${TableToGraph_src} ${TableToGraph_h} ${TableToGraph_res})
ENDIF(WIN32)

# Create static library
add_library(TableToGraph SHARED ${TableToGraph_src} ${TableToGraph_cxx})

# Add library dependencies
target_link_libraries(TableToGraph
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
install( FILES ${TableToGraph_res} DESTINATION ${NODE_PLUGIN_INSTALL} )
install( TARGETS TableToGraph 
	RUNTIME DESTINATION ${NODE_PLUGIN_INSTALL} 
)