project(AxisLayouter)
set(CMAKE_CONFIGURATION_TYPES Debug Release CACHE STRING "" FORCE) 
SET(CMAKE_DEBUG_POSTFIX _d)

include_directories(
			${CMAKE_CURRENT_SOURCE_DIR}
			)

# AxisLayouter
set( AxisLayouter_h ${AxisLayouter_h}
			AxisLayouterNode.h
			AxisLayouterNodePlugin.h
			)

set( AxisLayouter_moc ${AxisLayouter_moc}	
			AxisLayouterNode.h
			AxisLayouterNodePlugin.h
			)

set( AxisLayouter_src ${AxisLayouter_src}	
			AxisLayouterNode.cpp
			AxisLayouterNodePlugin.cpp
			)

set( AxisLayouter_res ${AxisLayouter_res}	
			AxisLayouter.xml
			)

# Create moc files		   
qt4_wrap_cpp(AxisLayouter_cxx ${AxisLayouter_moc})

# Create source groups
source_group("Moc Files" FILES ${AxisLayouter_cxx})
source_group("Header Files" FILES ${AxisLayouter_h})
source_group("Resources" FILES ${AxisLayouter_res})

# Add header files to sources to make headers visible in Visual Studio
IF (WIN32)
  set(AxisLayouter_src ${AxisLayouter_src} ${AxisLayouter_h} ${AxisLayouter_res})
ENDIF(WIN32)

# Create static library
add_library(AxisLayouter SHARED ${AxisLayouter_src} ${AxisLayouter_cxx})

# Add library dependencies
target_link_libraries(AxisLayouter
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
install( FILES ${AxisLayouter_res} DESTINATION ${NODE_PLUGIN_INSTALL} )
install( TARGETS AxisLayouter RUNTIME DESTINATION ${NODE_PLUGIN_INSTALL} )
