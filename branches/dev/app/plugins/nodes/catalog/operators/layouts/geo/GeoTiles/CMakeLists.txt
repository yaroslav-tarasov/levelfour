project(GeoTilesLayouter)
set(CMAKE_CONFIGURATION_TYPES Debug Release CACHE STRING "" FORCE) 
SET(CMAKE_DEBUG_POSTFIX _d)

include_directories(
			${CMAKE_CURRENT_SOURCE_DIR}
			)

# GeoTilesLayouter
set( GeoTilesLayouter_h ${GeoTilesLayouter_h}
			GeoTilesLayouterNode.h
			GeoTilesLayouterNodePlugin.h
			)

set( GeoTilesLayouter_moc ${GeoTilesLayouter_moc}	
			GeoTilesLayouterNode.h
			GeoTilesLayouterNodePlugin.h
			)

set( GeoTilesLayouter_src ${GeoTilesLayouter_src}	
			GeoTilesLayouterNode.cpp
			GeoTilesLayouterNodePlugin.cpp
			)

set( GeoTilesLayouter_res ${GeoTilesLayouter_res}	
			GeoTilesLayouter.xml
			)

# Create moc files		   
qt4_wrap_cpp(GeoTilesLayouter_cxx ${GeoTilesLayouter_moc})

# Create source groups
source_group("Moc Files" FILES ${GeoTilesLayouter_cxx})
source_group("Header Files" FILES ${GeoTilesLayouter_h})
source_group("Resources" FILES ${GeoTilesLayouter_res})

# Add header files to sources to make headers visible in Visual Studio
IF (WIN32)
  set(GeoTilesLayouter_src ${GeoTilesLayouter_src} ${GeoTilesLayouter_h} ${GeoTilesLayouter_res})
ENDIF(WIN32)

# Create static library
add_library(GeoTilesLayouter SHARED ${GeoTilesLayouter_src} ${GeoTilesLayouter_cxx})

# Add library dependencies
target_link_libraries(GeoTilesLayouter
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
install( FILES ${GeoTilesLayouter_res} DESTINATION ${NODE_PLUGIN_INSTALL} )
install( TARGETS GeoTilesLayouter RUNTIME DESTINATION ${NODE_PLUGIN_INSTALL} )
