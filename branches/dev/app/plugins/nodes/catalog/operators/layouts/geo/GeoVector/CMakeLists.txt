project(GeoVectorLayouter)
set(CMAKE_CONFIGURATION_TYPES Debug Release CACHE STRING "" FORCE) 
SET(CMAKE_DEBUG_POSTFIX _d)

include_directories(
			${CMAKE_CURRENT_SOURCE_DIR}
			)

# GeoVectorLayouter
set( GeoVectorLayouter_h ${GeoVectorLayouter_h}
			GeoVectorLayouterNode.h
			GeoVectorLayouterNodePlugin.h
			)

set( GeoVectorLayouter_moc ${GeoVectorLayouter_moc}	
			GeoVectorLayouterNode.h
			GeoVectorLayouterNodePlugin.h
			)

set( GeoVectorLayouter_src ${GeoVectorLayouter_src}	
			GeoVectorLayouterNode.cpp
			GeoVectorLayouterNodePlugin.cpp
			)

set( GeoVectorLayouter_res ${GeoVectorLayouter_res}	
			GeoVectorLayouter.xml
			)

# Create moc files		   
qt4_wrap_cpp(GeoVectorLayouter_cxx ${GeoVectorLayouter_moc})

# Create source groups
source_group("Moc Files" FILES ${GeoVectorLayouter_cxx})
source_group("Header Files" FILES ${GeoVectorLayouter_h})
source_group("Resources" FILES ${GeoVectorLayouter_res})

# Add header files to sources to make headers visible in Visual Studio
IF (WIN32)
  set(GeoVectorLayouter_src ${GeoVectorLayouter_src} ${GeoVectorLayouter_h} ${GeoVectorLayouter_res})
ENDIF(WIN32)

# Create static library
add_library(GeoVectorLayouter SHARED ${GeoVectorLayouter_src} ${GeoVectorLayouter_cxx})

# Add library dependencies
target_link_libraries(GeoVectorLayouter
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
install( FILES ${GeoVectorLayouter_res} DESTINATION ${NODE_PLUGIN_INSTALL} )
install( TARGETS GeoVectorLayouter RUNTIME DESTINATION ${NODE_PLUGIN_INSTALL} )
