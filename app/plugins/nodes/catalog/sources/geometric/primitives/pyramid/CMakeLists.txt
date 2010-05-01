project(PyramidSource)
set(CMAKE_CONFIGURATION_TYPES Debug Release CACHE STRING "" FORCE) 
SET(CMAKE_DEBUG_POSTFIX _d)

include_directories(
			${CMAKE_CURRENT_SOURCE_DIR}
			)

# PyramidSource
set( PyramidSource_h ${PyramidSource_h}
			PyramidSourceNode.h
			PyramidSourceNodePlugin.h
			)

set( PyramidSource_moc ${PyramidSource_moc}	
			PyramidSourceNode.h
			PyramidSourceNodePlugin.h
			)

set( PyramidSource_src ${PyramidSource_src}	
			PyramidSourceNode.cpp
			PyramidSourceNodePlugin.cpp
			)

set( PyramidSource_res ${PyramidSource_res}	
			PyramidSource.xml
			Pyramid.mesh
			)

# Create moc files		   
qt4_wrap_cpp(PyramidSource_cxx ${PyramidSource_moc})

# Create source groups
source_group("Moc Files" FILES ${PyramidSource_cxx})
source_group("Header Files" FILES ${PyramidSource_h})
source_group("Resources" FILES ${PyramidSource_res})

# Add header files to sources to make headers visible in Visual Studio
IF (WIN32)
  set(PyramidSource_src ${PyramidSource_src} ${PyramidSource_h} ${PyramidSource_res})
ENDIF(WIN32)

# Create static library
add_library(PyramidSource SHARED ${PyramidSource_src} ${PyramidSource_cxx})

# Add library dependencies
target_link_libraries(PyramidSource
			optimized QtCore4 debug QtCored4
			optimized QtGui4 debug QtGuid4
			optimized QtXml4 debug QtXmld4
			optimized OgreMain debug OgreMain_d
			optimized frappercore debug frappercore_d
			)

# Install files
install( FILES ${PyramidSource_res} DESTINATION ${NODE_PLUGIN_INSTALL} )
install( TARGETS PyramidSource RUNTIME DESTINATION ${NODE_PLUGIN_INSTALL} )
