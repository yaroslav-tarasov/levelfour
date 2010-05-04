project(SphericalLayouter)
set(CMAKE_CONFIGURATION_TYPES Debug Release CACHE STRING "" FORCE) 
SET(CMAKE_DEBUG_POSTFIX _d)

include_directories(
			${CMAKE_CURRENT_SOURCE_DIR}
			)

# SphericalLayouter
set( SphericalLayouter_h ${SphericalLayouter_h}
			SphericalLayouterNode.h
			SphericalLayouterNodePlugin.h
			)

set( SphericalLayouter_moc ${SphericalLayouter_moc}	
			SphericalLayouterNode.h
			SphericalLayouterNodePlugin.h
			)

set( SphericalLayouter_src ${SphericalLayouter_src}	
			SphericalLayouterNode.cpp
			SphericalLayouterNodePlugin.cpp
			)

set( SphericalLayouter_res ${SphericalLayouter_res}	
			SphericalLayouter.xml
			)

# Create moc files		   
qt4_wrap_cpp(SphericalLayouter_cxx ${SphericalLayouter_moc})

# Create source groups
source_group("Moc Files" FILES ${SphericalLayouter_cxx})
source_group("Header Files" FILES ${SphericalLayouter_h})
source_group("Resources" FILES ${SphericalLayouter_res})

# Add header files to sources to make headers visible in Visual Studio
IF (WIN32)
  set(SphericalLayouter_src ${SphericalLayouter_src} ${SphericalLayouter_h} ${SphericalLayouter_res})
ENDIF(WIN32)

# Create static library
add_library(SphericalLayouter SHARED ${SphericalLayouter_src} ${SphericalLayouter_cxx})

# Add library dependencies
target_link_libraries(SphericalLayouter
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
install( FILES ${SphericalLayouter_res} DESTINATION ${NODE_PLUGIN_INSTALL} )
install( TARGETS SphericalLayouter RUNTIME DESTINATION ${NODE_PLUGIN_INSTALL} )
