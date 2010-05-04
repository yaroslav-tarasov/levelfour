project(ClusteringLayouter)
set(CMAKE_CONFIGURATION_TYPES Debug Release CACHE STRING "" FORCE) 
SET(CMAKE_DEBUG_POSTFIX _d)

include_directories(
			${CMAKE_CURRENT_SOURCE_DIR}
			)

# ClusteringLayouter
set( ClusteringLayouter_h ${ClusteringLayouter_h}
			ClusteringLayouterNode.h
			ClusteringLayouterNodePlugin.h
			)

set( ClusteringLayouter_moc ${ClusteringLayouter_moc}	
			ClusteringLayouterNode.h
			ClusteringLayouterNodePlugin.h
			)

set( ClusteringLayouter_src ${ClusteringLayouter_src}	
			ClusteringLayouterNode.cpp
			ClusteringLayouterNodePlugin.cpp
			)

set( ClusteringLayouter_res ${ClusteringLayouter_res}	
			ClusteringLayouter.xml
			)

# Create moc files		   
qt4_wrap_cpp(ClusteringLayouter_cxx ${ClusteringLayouter_moc})

# Create source groups
source_group("Moc Files" FILES ${ClusteringLayouter_cxx})
source_group("Header Files" FILES ${ClusteringLayouter_h})
source_group("Resources" FILES ${ClusteringLayouter_res})

# Add header files to sources to make headers visible in Visual Studio
IF (WIN32)
  set(ClusteringLayouter_src ${ClusteringLayouter_src} ${ClusteringLayouter_h} ${ClusteringLayouter_res})
ENDIF(WIN32)

# Create static library
add_library(ClusteringLayouter SHARED ${ClusteringLayouter_src} ${ClusteringLayouter_cxx})

# Add library dependencies
target_link_libraries(ClusteringLayouter
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
install( FILES ${ClusteringLayouter_res} DESTINATION ${NODE_PLUGIN_INSTALL} )
install( TARGETS ClusteringLayouter RUNTIME DESTINATION ${NODE_PLUGIN_INSTALL} )
