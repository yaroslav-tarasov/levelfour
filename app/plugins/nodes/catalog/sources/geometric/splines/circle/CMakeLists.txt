project(CircleSource)
set(CMAKE_CONFIGURATION_TYPES Debug Release CACHE STRING "" FORCE) 
SET(CMAKE_DEBUG_POSTFIX _d)

include_directories(
			${CMAKE_CURRENT_SOURCE_DIR}
			)

# CircleSource
set( CircleSource_h ${CircleSource_h}
			CircleSourceNode.h
			CircleSourceNodePlugin.h
			)

set( CircleSource_moc ${CircleSource_moc}	
			CircleSourceNode.h
			CircleSourceNodePlugin.h
			)

set( CircleSource_src ${CircleSource_src}	
			CircleSourceNode.cpp
			CircleSourceNodePlugin.cpp
			)

set( CircleSource_res ${CircleSource_res}	
			CircleSource.xml
			Circle.mesh
			)

# Create moc files		   
qt4_wrap_cpp(CircleSource_cxx ${CircleSource_moc})

# Create source groups
source_group("Moc Files" FILES ${CircleSource_cxx})
source_group("Header Files" FILES ${CircleSource_h})
source_group("Resources" FILES ${CircleSource_res})

# Add header files to sources to make headers visible in Visual Studio
IF (WIN32)
  set(CircleSource_src ${CircleSource_src} ${CircleSource_h} ${CircleSource_res})
ENDIF(WIN32)

# Create static library
add_library(CircleSource SHARED ${CircleSource_src} ${CircleSource_cxx})

# Add library dependencies
target_link_libraries(CircleSource
			optimized QtCore4 debug QtCored4
			optimized QtGui4 debug QtGuid4
			optimized QtXml4 debug QtXmld4
			optimized OgreMain debug OgreMain_d
			optimized frappercore debug frappercore_d
			)

# Install files
install( FILES ${CircleSource_res} DESTINATION ${NODE_PLUGIN_INSTALL} )
install( TARGETS CircleSource RUNTIME DESTINATION ${NODE_PLUGIN_INSTALL} )
