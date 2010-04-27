project(TreeOrbitLayouter)
set(CMAKE_CONFIGURATION_TYPES Debug Release CACHE STRING "" FORCE) 
SET(CMAKE_DEBUG_POSTFIX _d)

include_directories(
			${CMAKE_CURRENT_SOURCE_DIR}
			)

# TreeOrbitLayouter
set( TreeOrbitLayouter_h ${TreeOrbitLayouter_h}
			TreeOrbitLayouterNode.h
			TreeOrbitLayouterNodePlugin.h
			)

set( TreeOrbitLayouter_moc ${TreeOrbitLayouter_moc}	
			TreeOrbitLayouterNode.h
			TreeOrbitLayouterNodePlugin.h
			)

set( TreeOrbitLayouter_src ${TreeOrbitLayouter_src}	
			TreeOrbitLayouterNode.cpp
			TreeOrbitLayouterNodePlugin.cpp
			)

set( TreeOrbitLayouter_res ${TreeOrbitLayouter_res}	
			TreeOrbitLayouter.xml
			)

# Create moc files		   
qt4_wrap_cpp(TreeOrbitLayouter_cxx ${TreeOrbitLayouter_moc})

# Create source groups
source_group("Moc Files" FILES ${TreeOrbitLayouter_cxx})
source_group("Header Files" FILES ${TreeOrbitLayouter_h})
source_group("Resources" FILES ${TreeOrbitLayouter_res})

# Add header files to sources to make headers visible in Visual Studio
IF (WIN32)
  set(TreeOrbitLayouter_src ${TreeOrbitLayouter_src} ${TreeOrbitLayouter_h} ${TreeOrbitLayouter_res})
ENDIF(WIN32)

# Create static library
add_library(TreeOrbitLayouter SHARED ${TreeOrbitLayouter_src} ${TreeOrbitLayouter_cxx})

# Add library dependencies
target_link_libraries(TreeOrbitLayouter
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
install( FILES ${TreeOrbitLayouter_res} DESTINATION ${NODE_PLUGIN_INSTALL} )
install( TARGETS TreeOrbitLayouter RUNTIME DESTINATION ${NODE_PLUGIN_INSTALL} )
