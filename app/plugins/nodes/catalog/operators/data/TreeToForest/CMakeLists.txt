project(TreeToForest)
set(CMAKE_CONFIGURATION_TYPES Debug Release CACHE STRING "" FORCE) 
SET(CMAKE_DEBUG_POSTFIX _d)

include_directories(
			${CMAKE_CURRENT_SOURCE_DIR}
			)

# TreeToForest
set( TreeToForest_h ${TreeToForest_h}
			TreeToForestNode.h
			TreeToForestNodePlugin.h
			)

set( TreeToForest_moc ${TreeToForest_moc}	
			TreeToForestNode.h
			TreeToForestNodePlugin.h
			)

set( TreeToForest_src ${TreeToForest_src}	
			TreeToForestNode.cpp
			TreeToForestNodePlugin.cpp
			)

set( TreeToForest_res ${TreeToForest_res}	
			TreeToForest.xml
			)

# Create moc files		   
qt4_wrap_cpp(TreeToForest_cxx ${TreeToForest_moc})

# Create source groups
source_group("Moc Files" FILES ${TreeToForest_cxx})
source_group("Header Files" FILES ${TreeToForest_h})
source_group("Resources" FILES ${TreeToForest_res})

# Add header files to sources to make headers visible in Visual Studio
IF (WIN32)
  set(TreeToForest_src ${TreeToForest_src} ${TreeToForest_h} ${TreeToForest_res})
ENDIF(WIN32)

# Create static library
add_library(TreeToForest SHARED ${TreeToForest_src} ${TreeToForest_cxx})

# Add library dependencies
target_link_libraries(TreeToForest
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
install( FILES ${TreeToForest_res} DESTINATION ${NODE_PLUGIN_INSTALL} )
install( TARGETS TreeToForest RUNTIME DESTINATION ${NODE_PLUGIN_INSTALL} )
