project(GeoRasterLayouter)
set(CMAKE_CONFIGURATION_TYPES Debug Release CACHE STRING "" FORCE) 
SET(CMAKE_DEBUG_POSTFIX _d)

include_directories(
			${CMAKE_CURRENT_SOURCE_DIR}
			)

# GeoRasterLayouter
set( GeoRasterLayouter_h ${GeoRasterLayouter_h}
			GeoRasterLayouterNode.h
			GeoRasterLayouterNodePlugin.h
			)

set( GeoRasterLayouter_moc ${GeoRasterLayouter_moc}	
			GeoRasterLayouterNode.h
			GeoRasterLayouterNodePlugin.h
			)

set( GeoRasterLayouter_src ${GeoRasterLayouter_src}	
			GeoRasterLayouterNode.cpp
			GeoRasterLayouterNodePlugin.cpp
			)

set( GeoRasterLayouter_res ${GeoRasterLayouter_res}	
			GeoRasterLayouter.xml
			)

# Create moc files		   
qt4_wrap_cpp(GeoRasterLayouter_cxx ${GeoRasterLayouter_moc})

# Create source groups
source_group("Moc Files" FILES ${GeoRasterLayouter_cxx})
source_group("Header Files" FILES ${GeoRasterLayouter_h})
source_group("Resources" FILES ${GeoRasterLayouter_res})

# Add header files to sources to make headers visible in Visual Studio
IF (WIN32)
  set(GeoRasterLayouter_src ${GeoRasterLayouter_src} ${GeoRasterLayouter_h} ${GeoRasterLayouter_res})
ENDIF(WIN32)

# Create static library
add_library(GeoRasterLayouter SHARED ${GeoRasterLayouter_src} ${GeoRasterLayouter_cxx})

# Add library dependencies
target_link_libraries(GeoRasterLayouter
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
install( FILES ${GeoRasterLayouter_res} DESTINATION ${NODE_PLUGIN_INSTALL} )
install( TARGETS GeoRasterLayouter RUNTIME DESTINATION ${NODE_PLUGIN_INSTALL} )
