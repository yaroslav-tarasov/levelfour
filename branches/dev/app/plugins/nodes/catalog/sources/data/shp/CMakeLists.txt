project(shpSource)
set(CMAKE_CONFIGURATION_TYPES Debug Release CACHE STRING "" FORCE) 
SET(CMAKE_DEBUG_POSTFIX _d)

include_directories(
			${CMAKE_CURRENT_SOURCE_DIR}
			)

# shpSource
set( shpSource_h ${shpSource_h}
			shpSourceNode.h
			shpSourceNodePlugin.h
			)

set( shpSource_moc ${shpSource_moc}	
			shpSourceNode.h
			shpSourceNodePlugin.h
			)

set( shpSource_src ${shpSource_src}	
			shpSourceNode.cpp
			shpSourceNodePlugin.cpp
			)

set( shpSource_res ${shpSource_res}	
			shpSource.xml
			)

# Create moc files		   
qt4_wrap_cpp(shpSource_cxx ${shpSource_moc})

# Create source groups
source_group("Moc Files" FILES ${shpSource_cxx})
source_group("Header Files" FILES ${shpSource_h})
source_group("Resources" FILES ${shpSource_res})

# Add header files to sources to make headers visible in Visual Studio
IF (WIN32)
  set(shpSource_src ${shpSource_src} ${shpSource_h} ${shpSource_res})
ENDIF(WIN32)

# Create static library
add_library(shpSource SHARED ${shpSource_src} ${shpSource_cxx})

# Add library dependencies
target_link_libraries(shpSource
			optimized QtCore4 debug QtCored4
			optimized QtGui4 debug QtGuid4
			optimized QtXml4 debug QtXmld4
			optimized OgreMain debug OgreMain_d
			optimized frappercore debug frappercore_d
			)

# Install files
install( FILES ${shpSource_res} DESTINATION ${NODE_PLUGIN_INSTALL} )
install( TARGETS shpSource RUNTIME DESTINATION ${NODE_PLUGIN_INSTALL} )
