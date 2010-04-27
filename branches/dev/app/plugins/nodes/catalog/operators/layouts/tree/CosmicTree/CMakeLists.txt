project(CosmicTreeLayouter)
set(CMAKE_CONFIGURATION_TYPES Debug Release CACHE STRING "" FORCE) 
SET(CMAKE_DEBUG_POSTFIX _d)

include_directories(
			${CMAKE_CURRENT_SOURCE_DIR}
			)

# CosmicTreeLayouter
set( CosmicTreeLayouter_h ${CosmicTreeLayouter_h}
			CosmicTreeLayouterNode.h
			CosmicTreeLayouterNodePlugin.h
			)

set( CosmicTreeLayouter_moc ${CosmicTreeLayouter_moc}	
			CosmicTreeLayouterNode.h
			CosmicTreeLayouterNodePlugin.h
			)

set( CosmicTreeLayouter_src ${CosmicTreeLayouter_src}	
			CosmicTreeLayouterNode.cpp
			CosmicTreeLayouterNodePlugin.cpp
			)

set( CosmicTreeLayouter_res ${CosmicTreeLayouter_res}	
			CosmicTreeLayouter.xml
			)

# Create moc files		   
qt4_wrap_cpp(CosmicTreeLayouter_cxx ${CosmicTreeLayouter_moc})

# Create source groups
source_group("Moc Files" FILES ${CosmicTreeLayouter_cxx})
source_group("Header Files" FILES ${CosmicTreeLayouter_h})
source_group("Resources" FILES ${CosmicTreeLayouter_res})

# Add header files to sources to make headers visible in Visual Studio
IF (WIN32)
  set(CosmicTreeLayouter_src ${CosmicTreeLayouter_src} ${CosmicTreeLayouter_h} ${CosmicTreeLayouter_res})
ENDIF(WIN32)

# Create static library
add_library(CosmicTreeLayouter SHARED ${CosmicTreeLayouter_src} ${CosmicTreeLayouter_cxx})

# Add library dependencies
target_link_libraries(CosmicTreeLayouter
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
install( FILES ${CosmicTreeLayouter_res} DESTINATION ${NODE_PLUGIN_INSTALL} )
install( TARGETS CosmicTreeLayouter RUNTIME DESTINATION ${NODE_PLUGIN_INSTALL} )
