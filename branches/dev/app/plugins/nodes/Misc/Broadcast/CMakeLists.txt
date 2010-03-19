project(broadcast)
set(CMAKE_CONFIGURATION_TYPES Debug Release CACHE STRING "" FORCE) 
SET(CMAKE_DEBUG_POSTFIX _d)

include_directories(
			${CMAKE_CURRENT_SOURCE_DIR}
			)

# broadcast
set( broadcast_h ${broadcast_h}		
			BroadcastNode.h
			BroadcastNodePlugin.h
			)

set( broadcast_moc ${broadcast_moc}	
			BroadcastNode.h
			BroadcastNodePlugin.h
			)

set( broadcast_src ${broadcast_src}	
			BroadcastNode.cpp
			BroadcastNodePlugin.cpp
			)

set( broadcast_res ${broadcast_res}	
			broadcast.xml
			)

# Create moc files		   
qt4_wrap_cpp(broadcast_cxx ${broadcast_moc})

# Create source groups
source_group("Moc Files" FILES ${broadcast_cxx})
source_group("Header Files" FILES ${broadcast_h})
source_group("Resources" FILES ${broadcast_res})

# Add header files to sources to make headers visible in Visual Studio
IF (WIN32)
  set(broadcast_src ${broadcast_src} ${broadcast_h} ${broadcast_res})
ENDIF(WIN32)

# Create static library
add_library(broadcast SHARED ${broadcast_src} ${broadcast_cxx})

# Add library dependencies
target_link_libraries(broadcast
			optimized QtCore4 debug QtCored4
			optimized QtGui4 debug QtGuid4
			optimized QtXml4 debug QtXmld4
			optimized QtNetwork4 debug QtNetworkd4
			optimized OgreMain debug OgreMain_d
			optimized frappercore debug frappercore_d
			)

# Install files
install( FILES ${broadcast_res} DESTINATION ${NODE_PLUGIN_INSTALL} )
install( TARGETS broadcast RUNTIME DESTINATION ${NODE_PLUGIN_INSTALL} )
