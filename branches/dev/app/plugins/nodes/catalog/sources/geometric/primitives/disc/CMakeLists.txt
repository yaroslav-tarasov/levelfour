project(DiscSource)
set(CMAKE_CONFIGURATION_TYPES Debug Release CACHE STRING "" FORCE) 
SET(CMAKE_DEBUG_POSTFIX _d)

include_directories(
			${CMAKE_CURRENT_SOURCE_DIR}
			)

# DiscSource
set( DiscSource_h ${DiscSource_h}
			DiscSourceNode.h
			DiscSourceNodePlugin.h
			)

set( DiscSource_moc ${DiscSource_moc}	
			DiscSourceNode.h
			DiscSourceNodePlugin.h
			)

set( DiscSource_src ${DiscSource_src}	
			DiscSourceNode.cpp
			DiscSourceNodePlugin.cpp
			)

set( DiscSource_res ${DiscSource_res}	
			DiscSource.xml
			Disc.mesh
			)

# Create moc files		   
qt4_wrap_cpp(DiscSource_cxx ${DiscSource_moc})

# Create source groups
source_group("Moc Files" FILES ${DiscSource_cxx})
source_group("Header Files" FILES ${DiscSource_h})
source_group("Resources" FILES ${DiscSource_res})

# Add header files to sources to make headers visible in Visual Studio
IF (WIN32)
  set(DiscSource_src ${DiscSource_src} ${DiscSource_h} ${DiscSource_res})
ENDIF(WIN32)

# Create static library
add_library(DiscSource SHARED ${DiscSource_src} ${DiscSource_cxx})

# Add library dependencies
target_link_libraries(DiscSource
			optimized QtCore4 debug QtCored4
			optimized QtGui4 debug QtGuid4
			optimized QtXml4 debug QtXmld4
			optimized OgreMain debug OgreMain_d
			optimized frappercore debug frappercore_d
			)

# Install files
install( FILES ${DiscSource_res} DESTINATION ${NODE_PLUGIN_INSTALL} )
install( TARGETS DiscSource RUNTIME DESTINATION ${NODE_PLUGIN_INSTALL} )
