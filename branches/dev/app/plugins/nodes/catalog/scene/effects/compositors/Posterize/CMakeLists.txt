project(Posterize)
set(CMAKE_CONFIGURATION_TYPES Debug Release CACHE STRING "" FORCE) 
SET(CMAKE_DEBUG_POSTFIX _d)

include_directories(
			${CMAKE_CURRENT_SOURCE_DIR}
			)

# Posterize
set( Posterize_h ${Posterize_h}		
			PosterizeNode.h
			PosterizeNodePlugin.h
			)

set( Posterize_moc ${Posterize_moc}	
			PosterizeNode.h
			PosterizeNodePlugin.h
			)

set( Posterize_src ${Posterize_src}	
			PosterizeNode.cpp
			PosterizeNodePlugin.cpp
			)

set( Posterize_res ${Posterize_res}	
			Posterize.xml
			)

# Create moc files		   
qt4_wrap_cpp(Posterize_cxx ${Posterize_moc})

# Create source groups
source_group("Moc Files" FILES ${Posterize_cxx})
source_group("Header Files" FILES ${Posterize_h})
source_group("Resources" FILES ${Posterize_res})

# Add header files to sources to make headers visible in Visual Studio
IF (WIN32)
  set(Posterize_src ${Posterize_src} ${Posterize_h} ${Posterize_res})
ENDIF(WIN32)

# Create static library
add_library(Posterize SHARED ${Posterize_src} ${Posterize_cxx})

# Add library dependencies
target_link_libraries(Posterize
			optimized QtCore4 debug QtCored4
			optimized QtGui4 debug QtGuid4
			optimized QtXml4 debug QtXmld4
			optimized OgreMain debug OgreMain_d
			optimized frappercore debug frappercore_d
			)

# Install files
install( FILES ${Posterize_res} DESTINATION ${NODE_PLUGIN_INSTALL} )
install( TARGETS Posterize RUNTIME DESTINATION ${NODE_PLUGIN_INSTALL} )
