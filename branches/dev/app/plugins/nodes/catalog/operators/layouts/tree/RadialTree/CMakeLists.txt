project(RadialTreeLayouter)
set(CMAKE_CONFIGURATION_TYPES Debug Release CACHE STRING "" FORCE) 
SET(CMAKE_DEBUG_POSTFIX _d)

include_directories(
			${CMAKE_CURRENT_SOURCE_DIR}
			)

# RadialTreeLayouter
set( RadialTreeLayouter_h ${RadialTreeLayouter_h}
			RadialTreeLayouterNode.h
			RadialTreeLayouterNodePlugin.h
			)

set( RadialTreeLayouter_moc ${RadialTreeLayouter_moc}	
			RadialTreeLayouterNode.h
			RadialTreeLayouterNodePlugin.h
			)

set( RadialTreeLayouter_src ${RadialTreeLayouter_src}	
			RadialTreeLayouterNode.cpp
			RadialTreeLayouterNodePlugin.cpp
			)

set( RadialTreeLayouter_res ${RadialTreeLayouter_res}	
			RadialTreeLayouter.xml
			)

# Create moc files		   
qt4_wrap_cpp(RadialTreeLayouter_cxx ${RadialTreeLayouter_moc})

# Create source groups
source_group("Moc Files" FILES ${RadialTreeLayouter_cxx})
source_group("Header Files" FILES ${RadialTreeLayouter_h})
source_group("Resources" FILES ${RadialTreeLayouter_res})

# Add header files to sources to make headers visible in Visual Studio
IF (WIN32)
  set(RadialTreeLayouter_src ${RadialTreeLayouter_src} ${RadialTreeLayouter_h} ${RadialTreeLayouter_res})
ENDIF(WIN32)

# Create static library
add_library(RadialTreeLayouter SHARED ${RadialTreeLayouter_src} ${RadialTreeLayouter_cxx})

# Add library dependencies
target_link_libraries(RadialTreeLayouter
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
install( FILES ${RadialTreeLayouter_res} DESTINATION ${NODE_PLUGIN_INSTALL} )
install( TARGETS RadialTreeLayouter RUNTIME DESTINATION ${NODE_PLUGIN_INSTALL} )
