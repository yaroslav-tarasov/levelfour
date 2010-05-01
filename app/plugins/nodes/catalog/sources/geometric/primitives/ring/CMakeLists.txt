project(RingSource)
set(CMAKE_CONFIGURATION_TYPES Debug Release CACHE STRING "" FORCE) 
SET(CMAKE_DEBUG_POSTFIX _d)

include_directories(
			${CMAKE_CURRENT_SOURCE_DIR}
			)

# RingSource
set( RingSource_h ${RingSource_h}
			RingSourceNode.h
			RingSourceNodePlugin.h
			)

set( RingSource_moc ${RingSource_moc}	
			RingSourceNode.h
			RingSourceNodePlugin.h
			)

set( RingSource_src ${RingSource_src}	
			RingSourceNode.cpp
			RingSourceNodePlugin.cpp
			)

set( RingSource_res ${RingSource_res}	
			RingSource.xml
			Ring.mesh
			)

# Create moc files		   
qt4_wrap_cpp(RingSource_cxx ${RingSource_moc})

# Create source groups
source_group("Moc Files" FILES ${RingSource_cxx})
source_group("Header Files" FILES ${RingSource_h})
source_group("Resources" FILES ${RingSource_res})

# Add header files to sources to make headers visible in Visual Studio
IF (WIN32)
  set(RingSource_src ${RingSource_src} ${RingSource_h} ${RingSource_res})
ENDIF(WIN32)

# Create static library
add_library(RingSource SHARED ${RingSource_src} ${RingSource_cxx})

# Add library dependencies
target_link_libraries(RingSource
			optimized QtCore4 debug QtCored4
			optimized QtGui4 debug QtGuid4
			optimized QtXml4 debug QtXmld4
			optimized OgreMain debug OgreMain_d
			optimized frappercore debug frappercore_d
			)

# Install files
install( FILES ${RingSource_res} DESTINATION ${NODE_PLUGIN_INSTALL} )
install( TARGETS RingSource RUNTIME DESTINATION ${NODE_PLUGIN_INSTALL} )
