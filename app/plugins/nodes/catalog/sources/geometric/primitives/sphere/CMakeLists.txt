project(SphereSource)
set(CMAKE_CONFIGURATION_TYPES Debug Release CACHE STRING "" FORCE) 
SET(CMAKE_DEBUG_POSTFIX _d)

include_directories(
			${CMAKE_CURRENT_SOURCE_DIR}
			)

# SphereSource
set( SphereSource_h ${SphereSource_h}
			SphereSourceNode.h
			SphereSourceNodePlugin.h
			)

set( SphereSource_moc ${SphereSource_moc}	
			SphereSourceNode.h
			SphereSourceNodePlugin.h
			)

set( SphereSource_src ${SphereSource_src}	
			SphereSourceNode.cpp
			SphereSourceNodePlugin.cpp
			)

set( SphereSource_res ${SphereSource_res}	
			SphereSource.xml
			)

# Create moc files		   
qt4_wrap_cpp(SphereSource_cxx ${SphereSource_moc})

# Create source groups
source_group("Moc Files" FILES ${SphereSource_cxx})
source_group("Header Files" FILES ${SphereSource_h})
source_group("Resources" FILES ${SphereSource_res})

# Add header files to sources to make headers visible in Visual Studio
IF (WIN32)
  set(SphereSource_src ${SphereSource_src} ${SphereSource_h} ${SphereSource_res})
ENDIF(WIN32)

# Create static library
add_library(SphereSource SHARED ${SphereSource_src} ${SphereSource_cxx})

# Add library dependencies
target_link_libraries(SphereSource
			optimized QtCore4 debug QtCored4
			optimized QtGui4 debug QtGuid4
			optimized QtXml4 debug QtXmld4
			optimized OgreMain debug OgreMain_d
			optimized frappercore debug frappercore_d
			)

# Install files
install( FILES ${SphereSource_res} DESTINATION ${NODE_PLUGIN_INSTALL} )
install( TARGETS SphereSource RUNTIME DESTINATION ${NODE_PLUGIN_INSTALL} )
