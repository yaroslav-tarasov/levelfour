project(ReingoldTilfordTreeLayouter)
set(CMAKE_CONFIGURATION_TYPES Debug Release CACHE STRING "" FORCE) 
SET(CMAKE_DEBUG_POSTFIX _d)

include_directories(
			${CMAKE_CURRENT_SOURCE_DIR}
			)

# ReingoldTilfordTreeLayouter
set( ReingoldTilfordTreeLayouter_h ${ReingoldTilfordTreeLayouter_h}
			ReingoldTilfordTreeLayouterNode.h
			ReingoldTilfordTreeLayouterNodePlugin.h
			)

set( ReingoldTilfordTreeLayouter_moc ${ReingoldTilfordTreeLayouter_moc}	
			ReingoldTilfordTreeLayouterNode.h
			ReingoldTilfordTreeLayouterNodePlugin.h
			)

set( ReingoldTilfordTreeLayouter_src ${ReingoldTilfordTreeLayouter_src}	
			ReingoldTilfordTreeLayouterNode.cpp
			ReingoldTilfordTreeLayouterNodePlugin.cpp
			)

set( ReingoldTilfordTreeLayouter_res ${ReingoldTilfordTreeLayouter_res}	
			ReingoldTilfordTreeLayouter.xml
			)

# Create moc files		   
qt4_wrap_cpp(ReingoldTilfordTreeLayouter_cxx ${ReingoldTilfordTreeLayouter_moc})

# Create source groups
source_group("Moc Files" FILES ${ReingoldTilfordTreeLayouter_cxx})
source_group("Header Files" FILES ${ReingoldTilfordTreeLayouter_h})
source_group("Resources" FILES ${ReingoldTilfordTreeLayouter_res})

# Add header files to sources to make headers visible in Visual Studio
IF (WIN32)
  set(ReingoldTilfordTreeLayouter_src ${ReingoldTilfordTreeLayouter_src} ${ReingoldTilfordTreeLayouter_h} ${ReingoldTilfordTreeLayouter_res})
ENDIF(WIN32)

# Create static library
add_library(ReingoldTilfordTreeLayouter SHARED ${ReingoldTilfordTreeLayouter_src} ${ReingoldTilfordTreeLayouter_cxx})

# Add library dependencies
target_link_libraries(ReingoldTilfordTreeLayouter
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
install( FILES ${ReingoldTilfordTreeLayouter_res} DESTINATION ${NODE_PLUGIN_INSTALL} )
install( TARGETS ReingoldTilfordTreeLayouter RUNTIME DESTINATION ${NODE_PLUGIN_INSTALL} )
