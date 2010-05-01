project(SpindleSource)
set(CMAKE_CONFIGURATION_TYPES Debug Release CACHE STRING "" FORCE) 
SET(CMAKE_DEBUG_POSTFIX _d)

include_directories(
			${CMAKE_CURRENT_SOURCE_DIR}
			)

# SpindleSource
set( SpindleSource_h ${SpindleSource_h}
			SpindleSourceNode.h
			SpindleSourceNodePlugin.h
			)

set( SpindleSource_moc ${SpindleSource_moc}	
			SpindleSourceNode.h
			SpindleSourceNodePlugin.h
			)

set( SpindleSource_src ${SpindleSource_src}	
			SpindleSourceNode.cpp
			SpindleSourceNodePlugin.cpp
			)

set( SpindleSource_res ${SpindleSource_res}	
			SpindleSource.xml
			Spindle.mesh
			)

# Create moc files		   
qt4_wrap_cpp(SpindleSource_cxx ${SpindleSource_moc})

# Create source groups
source_group("Moc Files" FILES ${SpindleSource_cxx})
source_group("Header Files" FILES ${SpindleSource_h})
source_group("Resources" FILES ${SpindleSource_res})

# Add header files to sources to make headers visible in Visual Studio
IF (WIN32)
  set(SpindleSource_src ${SpindleSource_src} ${SpindleSource_h} ${SpindleSource_res})
ENDIF(WIN32)

# Create static library
add_library(SpindleSource SHARED ${SpindleSource_src} ${SpindleSource_cxx})

# Add library dependencies
target_link_libraries(SpindleSource
			optimized QtCore4 debug QtCored4
			optimized QtGui4 debug QtGuid4
			optimized QtXml4 debug QtXmld4
			optimized OgreMain debug OgreMain_d
			optimized frappercore debug frappercore_d
			)

# Install files
install( FILES ${SpindleSource_res} DESTINATION ${NODE_PLUGIN_INSTALL} )
install( TARGETS SpindleSource RUNTIME DESTINATION ${NODE_PLUGIN_INSTALL} )
