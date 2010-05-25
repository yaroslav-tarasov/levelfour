project(Tiles)
set(CMAKE_CONFIGURATION_TYPES Debug Release CACHE STRING "" FORCE) 
SET(CMAKE_DEBUG_POSTFIX _d)

include_directories(
			${CMAKE_CURRENT_SOURCE_DIR}
			)

# Tiles
set( Tiles_h ${Tiles_h}		
			TilesNode.h
			TilesNodePlugin.h
			)

set( Tiles_moc ${Tiles_moc}	
			TilesNode.h
			TilesNodePlugin.h
			)

set( Tiles_src ${Tiles_src}	
			TilesNode.cpp
			TilesNodePlugin.cpp
			)

set( Tiles_res ${Tiles_res}	
			Tiles.xml
			)

# Create moc files		   
qt4_wrap_cpp(Tiles_cxx ${Tiles_moc})

# Create source groups
source_group("Moc Files" FILES ${Tiles_cxx})
source_group("Header Files" FILES ${Tiles_h})
source_group("Resources" FILES ${Tiles_res})

# Add header files to sources to make headers visible in Visual Studio
IF (WIN32)
  set(Tiles_src ${Tiles_src} ${Tiles_h} ${Tiles_res})
ENDIF(WIN32)

# Create static library
add_library(Tiles SHARED ${Tiles_src} ${Tiles_cxx})

# Add library dependencies
target_link_libraries(Tiles
			optimized QtCore4 debug QtCored4
			optimized QtGui4 debug QtGuid4
			optimized QtXml4 debug QtXmld4
			optimized OgreMain debug OgreMain_d
			optimized frappercore debug frappercore_d
			)

# Install files
install( FILES ${Tiles_res} DESTINATION ${NODE_PLUGIN_INSTALL} )
install( TARGETS Tiles RUNTIME DESTINATION ${NODE_PLUGIN_INSTALL} )
