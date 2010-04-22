project(StraightEdgeSource)
set(CMAKE_CONFIGURATION_TYPES Debug Release CACHE STRING "" FORCE) 
SET(CMAKE_DEBUG_POSTFIX _d)

include_directories(
			${CMAKE_CURRENT_SOURCE_DIR}
			)

# StraightEdgeSource
set( StraightEdgeSource_h ${StraightEdgeSource_h}
			StraightEdgeSourceNode.h
			StraightEdgeSourceNodePlugin.h
			)

set( StraightEdgeSource_moc ${StraightEdgeSource_moc}	
			StraightEdgeSourceNode.h
			StraightEdgeSourceNodePlugin.h
			)

set( StraightEdgeSource_src ${StraightEdgeSource_src}	
			StraightEdgeSourceNode.cpp
			StraightEdgeSourceNodePlugin.cpp
			)

set( StraightEdgeSource_res ${StraightEdgeSource_res}	
			StraightEdgeSource.xml
			)

# Create moc files		   
qt4_wrap_cpp(StraightEdgeSource_cxx ${StraightEdgeSource_moc})

# Create source groups
source_group("Moc Files" FILES ${StraightEdgeSource_cxx})
source_group("Header Files" FILES ${StraightEdgeSource_h})
source_group("Resources" FILES ${StraightEdgeSource_res})

# Add header files to sources to make headers visible in Visual Studio
IF (WIN32)
  set(StraightEdgeSource_src ${StraightEdgeSource_src} ${StraightEdgeSource_h} ${StraightEdgeSource_res})
ENDIF(WIN32)

# Create static library
add_library(StraightEdgeSource SHARED ${StraightEdgeSource_src} ${StraightEdgeSource_cxx})

# Add library dependencies
target_link_libraries(StraightEdgeSource
			optimized QtCore4 debug QtCored4
			optimized QtGui4 debug QtGuid4
			optimized QtXml4 debug QtXmld4
			optimized OgreMain debug OgreMain_d
			optimized frappercore debug frappercore_d
			)

# Install files
install( FILES ${StraightEdgeSource_res} DESTINATION ${NODE_PLUGIN_INSTALL} )
install( TARGETS StraightEdgeSource RUNTIME DESTINATION ${NODE_PLUGIN_INSTALL} )
