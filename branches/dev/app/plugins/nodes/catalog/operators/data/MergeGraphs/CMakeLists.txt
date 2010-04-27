project(MergeGraphs)
set(CMAKE_CONFIGURATION_TYPES Debug Release CACHE STRING "" FORCE) 
SET(CMAKE_DEBUG_POSTFIX _d)

include_directories(
			${CMAKE_CURRENT_SOURCE_DIR}
			)

# MergeGraphs
set( MergeGraphs_h ${MergeGraphs_h}
			MergeGraphsNode.h
			MergeGraphsNodePlugin.h
			)

set( MergeGraphs_moc ${MergeGraphs_moc}	
			MergeGraphsNode.h
			MergeGraphsNodePlugin.h
			)

set( MergeGraphs_src ${MergeGraphs_src}	
			MergeGraphsNode.cpp
			MergeGraphsNodePlugin.cpp
			)

set( MergeGraphs_res ${MergeGraphs_res}	
			MergeGraphs.xml
			)

# Create moc files		   
qt4_wrap_cpp(MergeGraphs_cxx ${MergeGraphs_moc})

# Create source groups
source_group("Moc Files" FILES ${MergeGraphs_cxx})
source_group("Header Files" FILES ${MergeGraphs_h})
source_group("Resources" FILES ${MergeGraphs_res})

# Add header files to sources to make headers visible in Visual Studio
IF (WIN32)
  set(MergeGraphs_src ${MergeGraphs_src} ${MergeGraphs_h} ${MergeGraphs_res})
ENDIF(WIN32)

# Create static library
add_library(MergeGraphs SHARED ${MergeGraphs_src} ${MergeGraphs_cxx})

# Add library dependencies
target_link_libraries(MergeGraphs
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
install( FILES ${MergeGraphs_res} DESTINATION ${NODE_PLUGIN_INSTALL} )
install( TARGETS MergeGraphs RUNTIME DESTINATION ${NODE_PLUGIN_INSTALL} )
