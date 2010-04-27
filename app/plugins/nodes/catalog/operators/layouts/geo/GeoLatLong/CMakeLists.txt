project(GeoLatLongLayouter)
set(CMAKE_CONFIGURATION_TYPES Debug Release CACHE STRING "" FORCE) 
SET(CMAKE_DEBUG_POSTFIX _d)

include_directories(
			${CMAKE_CURRENT_SOURCE_DIR}
			)

# GeoLatLongLayouter
set( GeoLatLongLayouter_h ${GeoLatLongLayouter_h}
			GeoLatLongLayouterNode.h
			GeoLatLongLayouterNodePlugin.h
			)

set( GeoLatLongLayouter_moc ${GeoLatLongLayouter_moc}	
			GeoLatLongLayouterNode.h
			GeoLatLongLayouterNodePlugin.h
			)

set( GeoLatLongLayouter_src ${GeoLatLongLayouter_src}	
			GeoLatLongLayouterNode.cpp
			GeoLatLongLayouterNodePlugin.cpp
			)

set( GeoLatLongLayouter_res ${GeoLatLongLayouter_res}	
			GeoLatLongLayouter.xml
			)

# Create moc files		   
qt4_wrap_cpp(GeoLatLongLayouter_cxx ${GeoLatLongLayouter_moc})

# Create source groups
source_group("Moc Files" FILES ${GeoLatLongLayouter_cxx})
source_group("Header Files" FILES ${GeoLatLongLayouter_h})
source_group("Resources" FILES ${GeoLatLongLayouter_res})

# Add header files to sources to make headers visible in Visual Studio
IF (WIN32)
  set(GeoLatLongLayouter_src ${GeoLatLongLayouter_src} ${GeoLatLongLayouter_h} ${GeoLatLongLayouter_res})
ENDIF(WIN32)

# Create static library
add_library(GeoLatLongLayouter SHARED ${GeoLatLongLayouter_src} ${GeoLatLongLayouter_cxx})

# Add library dependencies
target_link_libraries(GeoLatLongLayouter
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
install( FILES ${GeoLatLongLayouter_res} DESTINATION ${NODE_PLUGIN_INSTALL} )
install( TARGETS GeoLatLongLayouter RUNTIME DESTINATION ${NODE_PLUGIN_INSTALL} )
