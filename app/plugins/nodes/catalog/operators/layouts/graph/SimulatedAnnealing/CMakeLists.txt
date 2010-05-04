project(SimulatedAnnealingLayouter)
set(CMAKE_CONFIGURATION_TYPES Debug Release CACHE STRING "" FORCE) 
SET(CMAKE_DEBUG_POSTFIX _d)

include_directories(
			${CMAKE_CURRENT_SOURCE_DIR}
			)

# SimulatedAnnealingLayouter
set( SimulatedAnnealingLayouter_h ${SimulatedAnnealingLayouter_h}
			SimulatedAnnealingLayouterNode.h
			SimulatedAnnealingLayouterNodePlugin.h
			)

set( SimulatedAnnealingLayouter_moc ${SimulatedAnnealingLayouter_moc}	
			SimulatedAnnealingLayouterNode.h
			SimulatedAnnealingLayouterNodePlugin.h
			)

set( SimulatedAnnealingLayouter_src ${SimulatedAnnealingLayouter_src}	
			SimulatedAnnealingLayouterNode.cpp
			SimulatedAnnealingLayouterNodePlugin.cpp
			)

set( SimulatedAnnealingLayouter_res ${SimulatedAnnealingLayouter_res}	
			SimulatedAnnealingLayouter.xml
			)

# Create moc files		   
qt4_wrap_cpp(SimulatedAnnealingLayouter_cxx ${SimulatedAnnealingLayouter_moc})

# Create source groups
source_group("Moc Files" FILES ${SimulatedAnnealingLayouter_cxx})
source_group("Header Files" FILES ${SimulatedAnnealingLayouter_h})
source_group("Resources" FILES ${SimulatedAnnealingLayouter_res})

# Add header files to sources to make headers visible in Visual Studio
IF (WIN32)
  set(SimulatedAnnealingLayouter_src ${SimulatedAnnealingLayouter_src} ${SimulatedAnnealingLayouter_h} ${SimulatedAnnealingLayouter_res})
ENDIF(WIN32)

# Create static library
add_library(SimulatedAnnealingLayouter SHARED ${SimulatedAnnealingLayouter_src} ${SimulatedAnnealingLayouter_cxx})

# Add library dependencies
target_link_libraries(SimulatedAnnealingLayouter
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
install( FILES ${SimulatedAnnealingLayouter_res} DESTINATION ${NODE_PLUGIN_INSTALL} )
install( TARGETS SimulatedAnnealingLayouter RUNTIME DESTINATION ${NODE_PLUGIN_INSTALL} )
