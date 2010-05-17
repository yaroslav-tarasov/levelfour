project(Sepia)
set(CMAKE_CONFIGURATION_TYPES Debug Release CACHE STRING "" FORCE) 
SET(CMAKE_DEBUG_POSTFIX _d)

include_directories(
			${CMAKE_CURRENT_SOURCE_DIR}
			)

# Sepia
set( Sepia_h ${Sepia_h}		
			SepiaNode.h
			SepiaNodePlugin.h
			)

set( Sepia_moc ${Sepia_moc}	
			SepiaNode.h
			SepiaNodePlugin.h
			)

set( Sepia_src ${Sepia_src}	
			SepiaNode.cpp
			SepiaNodePlugin.cpp
			)

set( Sepia_res ${Sepia_res}	
			Sepia.xml
			)

# Create moc files		   
qt4_wrap_cpp(Sepia_cxx ${Sepia_moc})

# Create source groups
source_group("Moc Files" FILES ${Sepia_cxx})
source_group("Header Files" FILES ${Sepia_h})
source_group("Resources" FILES ${Sepia_res})

# Add header files to sources to make headers visible in Visual Studio
IF (WIN32)
  set(Sepia_src ${Sepia_src} ${Sepia_h} ${Sepia_res})
ENDIF(WIN32)

# Create static library
add_library(Sepia SHARED ${Sepia_src} ${Sepia_cxx})

# Add library dependencies
target_link_libraries(Sepia
			optimized QtCore4 debug QtCored4
			optimized QtGui4 debug QtGuid4
			optimized QtXml4 debug QtXmld4
			optimized OgreMain debug OgreMain_d
			optimized frappercore debug frappercore_d
			)

# Install files
install( FILES ${Sepia_res} DESTINATION ${NODE_PLUGIN_INSTALL} )
install( TARGETS Sepia RUNTIME DESTINATION ${NODE_PLUGIN_INSTALL} )
