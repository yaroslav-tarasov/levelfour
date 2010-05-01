project(PlaneSource)
set(CMAKE_CONFIGURATION_TYPES Debug Release CACHE STRING "" FORCE) 
SET(CMAKE_DEBUG_POSTFIX _d)

include_directories(
			${CMAKE_CURRENT_SOURCE_DIR}
			)

# PlaneSource
set( PlaneSource_h ${PlaneSource_h}
			PlaneSourceNode.h
			PlaneSourceNodePlugin.h
			)

set( PlaneSource_moc ${PlaneSource_moc}	
			PlaneSourceNode.h
			PlaneSourceNodePlugin.h
			)

set( PlaneSource_src ${PlaneSource_src}	
			PlaneSourceNode.cpp
			PlaneSourceNodePlugin.cpp
			)

set( PlaneSource_res ${PlaneSource_res}	
			PlaneSource.xml
			Plane.mesh
			)

# Create moc files		   
qt4_wrap_cpp(PlaneSource_cxx ${PlaneSource_moc})

# Create source groups
source_group("Moc Files" FILES ${PlaneSource_cxx})
source_group("Header Files" FILES ${PlaneSource_h})
source_group("Resources" FILES ${PlaneSource_res})

# Add header files to sources to make headers visible in Visual Studio
IF (WIN32)
  set(PlaneSource_src ${PlaneSource_src} ${PlaneSource_h} ${PlaneSource_res})
ENDIF(WIN32)

# Create static library
add_library(PlaneSource SHARED ${PlaneSource_src} ${PlaneSource_cxx})

# Add library dependencies
target_link_libraries(PlaneSource
			optimized QtCore4 debug QtCored4
			optimized QtGui4 debug QtGuid4
			optimized QtXml4 debug QtXmld4
			optimized OgreMain debug OgreMain_d
			optimized frappercore debug frappercore_d
			)

# Install files
install( FILES ${PlaneSource_res} DESTINATION ${NODE_PLUGIN_INSTALL} )
install( TARGETS PlaneSource RUNTIME DESTINATION ${NODE_PLUGIN_INSTALL} )
