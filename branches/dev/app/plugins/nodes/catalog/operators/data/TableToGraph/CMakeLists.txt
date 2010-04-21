project(tableToGraph)
set(CMAKE_CONFIGURATION_TYPES Debug Release CACHE STRING "" FORCE) 
SET(CMAKE_DEBUG_POSTFIX _d)

include_directories(
			${CMAKE_CURRENT_SOURCE_DIR}
			)

# tableToGraph
set( tableToGraph_h ${tableToGraph_h}
			tableToGraphNode.h
			tableToGraphNodePlugin.h
			)

set( tableToGraph_moc ${tableToGraph_moc}	
			tableToGraphNode.h
			tableToGraphNodePlugin.h
			)

set( tableToGraph_src ${tableToGraph_src}	
			tableToGraphNode.cpp
			tableToGraphNodePlugin.cpp
			)

set( tableToGraph_res ${tableToGraph_res}	
			tableToGraph.xml
			)

# Create moc files		   
qt4_wrap_cpp(tableToGraph_cxx ${tableToGraph_moc})

# Create source groups
source_group("Moc Files" FILES ${tableToGraph_cxx})
source_group("Header Files" FILES ${tableToGraph_h})
source_group("Resources" FILES ${tableToGraph_res})

# Add header files to sources to make headers visible in Visual Studio
IF (WIN32)
  set(tableToGraph_src ${tableToGraph_src} ${tableToGraph_h} ${tableToGraph_res})
ENDIF(WIN32)

# Create static library
add_library(tableToGraph SHARED ${tableToGraph_src} ${tableToGraph_cxx})

# Add library dependencies
target_link_libraries(tableToGraph
			optimized QtCore4 debug QtCored4
			optimized QtGui4 debug QtGuid4
			optimized QtXml4 debug QtXmld4
			optimized OgreMain debug OgreMain_d
			optimized frappercore debug frappercore_d
			)

# Install files
install( FILES ${tableToGraph_res} DESTINATION ${NODE_PLUGIN_INSTALL} )
install( TARGETS tableToGraph RUNTIME DESTINATION ${NODE_PLUGIN_INSTALL} )
