project(PolarLayouter)
set(CMAKE_CONFIGURATION_TYPES Debug Release CACHE STRING "" FORCE) 
SET(CMAKE_DEBUG_POSTFIX _d)

include_directories(
			${CMAKE_CURRENT_SOURCE_DIR}
			)

# PolarLayouter
set( PolarLayouter_h ${PolarLayouter_h}
			PolarLayouterNode.h
			PolarLayouterNodePlugin.h
			)

set( PolarLayouter_moc ${PolarLayouter_moc}	
			PolarLayouterNode.h
			PolarLayouterNodePlugin.h
			)

set( PolarLayouter_src ${PolarLayouter_src}	
			PolarLayouterNode.cpp
			PolarLayouterNodePlugin.cpp
			)

set( PolarLayouter_res ${PolarLayouter_res}	
			PolarLayouter.xml
			)

# Create moc files		   
qt4_wrap_cpp(PolarLayouter_cxx ${PolarLayouter_moc})

# Create source groups
source_group("Moc Files" FILES ${PolarLayouter_cxx})
source_group("Header Files" FILES ${PolarLayouter_h})
source_group("Resources" FILES ${PolarLayouter_res})

# Add header files to sources to make headers visible in Visual Studio
IF (WIN32)
  set(PolarLayouter_src ${PolarLayouter_src} ${PolarLayouter_h} ${PolarLayouter_res})
ENDIF(WIN32)

# Create static library
add_library(PolarLayouter SHARED ${PolarLayouter_src} ${PolarLayouter_cxx})

# Add library dependencies
target_link_libraries(PolarLayouter
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
install( FILES ${PolarLayouter_res} DESTINATION ${NODE_PLUGIN_INSTALL} )
install( TARGETS PolarLayouter RUNTIME DESTINATION ${NODE_PLUGIN_INSTALL} )
