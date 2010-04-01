project(lightwarping)
set(CMAKE_CONFIGURATION_TYPES Debug Release CACHE STRING "" FORCE) 
SET(CMAKE_DEBUG_POSTFIX _d)

include_directories(
			${CMAKE_CURRENT_SOURCE_DIR}
			)

# lightwarping
set( lightwarping_h ${lightwarping_h}		
			LightWarpingNode.h
			LightWarpingNodePlugin.h
			)

set( lightwarping_moc ${lightwarping_moc}	
			LightWarpingNode.h
			LightWarpingNodePlugin.h
			)

set( lightwarping_src ${lightwarping_src}	
			LightWarpingNode.cpp
			LightWarpingNodePlugin.cpp
			)

set( lightwarping_res ${lightwarping_res}	
			lightwarping.xml
			)

# Create moc files		   
qt4_wrap_cpp(lightwarping_cxx ${lightwarping_moc})

# Create source groups
source_group("Moc Files" FILES ${lightwarping_cxx})
source_group("Header Files" FILES ${lightwarping_h})
source_group("Resources" FILES ${lightwarping_res})

# Add header files to sources to make headers visible in Visual Studio
IF (WIN32)
  set(lightwarping_src ${lightwarping_src} ${lightwarping_h} ${lightwarping_res})
ENDIF(WIN32)

# Create static library
add_library(lightwarping SHARED ${lightwarping_src} ${lightwarping_cxx})

# Add library dependencies
target_link_libraries(lightwarping
			optimized QtCore4 debug QtCored4
			optimized QtGui4 debug QtGuid4
			optimized QtXml4 debug QtXmld4
			optimized OgreMain debug OgreMain_d
			optimized frappercore debug frappercore_d
			)

# Install files
install( FILES ${lightwarping_res} DESTINATION ${NODE_PLUGIN_INSTALL} )
install( TARGETS lightwarping RUNTIME DESTINATION ${NODE_PLUGIN_INSTALL} )
