project(DrLLayouter)
set(CMAKE_CONFIGURATION_TYPES Debug Release CACHE STRING "" FORCE) 
SET(CMAKE_DEBUG_POSTFIX _d)

include_directories(
			${CMAKE_CURRENT_SOURCE_DIR}
			)

# DrLLayouter
set( DrLLayouter_h ${DrLLayouter_h}
			DrLLayouterNode.h
			DrLLayouterNodePlugin.h
			)

set( DrLLayouter_moc ${DrLLayouter_moc}	
			DrLLayouterNode.h
			DrLLayouterNodePlugin.h
			)

set( DrLLayouter_src ${DrLLayouter_src}	
			DrLLayouterNode.cpp
			DrLLayouterNodePlugin.cpp
			)

set( DrLLayouter_res ${DrLLayouter_res}	
			DrLLayouter.xml
			)

# Create moc files		   
qt4_wrap_cpp(DrLLayouter_cxx ${DrLLayouter_moc})

# Create source groups
source_group("Moc Files" FILES ${DrLLayouter_cxx})
source_group("Header Files" FILES ${DrLLayouter_h})
source_group("Resources" FILES ${DrLLayouter_res})

# Add header files to sources to make headers visible in Visual Studio
IF (WIN32)
  set(DrLLayouter_src ${DrLLayouter_src} ${DrLLayouter_h} ${DrLLayouter_res})
ENDIF(WIN32)

# Create static library
add_library(DrLLayouter SHARED ${DrLLayouter_src} ${DrLLayouter_cxx})

# Add library dependencies
target_link_libraries(DrLLayouter
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
install( FILES ${DrLLayouter_res} DESTINATION ${NODE_PLUGIN_INSTALL} )
install( TARGETS DrLLayouter RUNTIME DESTINATION ${NODE_PLUGIN_INSTALL} )
