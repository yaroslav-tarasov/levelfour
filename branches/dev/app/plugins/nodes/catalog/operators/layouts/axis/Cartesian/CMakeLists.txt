project(CartesianLayouter)
set(CMAKE_CONFIGURATION_TYPES Debug Release CACHE STRING "" FORCE) 
SET(CMAKE_DEBUG_POSTFIX _d)

include_directories(
			${CMAKE_CURRENT_SOURCE_DIR}
			)

# CartesianLayouter
set( CartesianLayouter_h ${CartesianLayouter_h}
			CartesianLayouterNode.h
			CartesianLayouterNodePlugin.h
			)

set( CartesianLayouter_moc ${CartesianLayouter_moc}	
			CartesianLayouterNode.h
			CartesianLayouterNodePlugin.h
			)

set( CartesianLayouter_src ${CartesianLayouter_src}	
			CartesianLayouterNode.cpp
			CartesianLayouterNodePlugin.cpp
			)

set( CartesianLayouter_res ${CartesianLayouter_res}	
			CartesianLayouter.xml
			)

# Create moc files		   
qt4_wrap_cpp(CartesianLayouter_cxx ${CartesianLayouter_moc})

# Create source groups
source_group("Moc Files" FILES ${CartesianLayouter_cxx})
source_group("Header Files" FILES ${CartesianLayouter_h})
source_group("Resources" FILES ${CartesianLayouter_res})

# Add header files to sources to make headers visible in Visual Studio
IF (WIN32)
  set(CartesianLayouter_src ${CartesianLayouter_src} ${CartesianLayouter_h} ${CartesianLayouter_res})
ENDIF(WIN32)

# Create static library
add_library(CartesianLayouter SHARED ${CartesianLayouter_src} ${CartesianLayouter_cxx})

# Add library dependencies
target_link_libraries(CartesianLayouter
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
install( FILES ${CartesianLayouter_res} DESTINATION ${NODE_PLUGIN_INSTALL} )
install( TARGETS CartesianLayouter RUNTIME DESTINATION ${NODE_PLUGIN_INSTALL} )
