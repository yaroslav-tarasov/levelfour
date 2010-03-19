project(colorimage)
set(CMAKE_CONFIGURATION_TYPES Debug Release CACHE STRING "" FORCE) 
SET(CMAKE_DEBUG_POSTFIX _d)

include_directories(
			${CMAKE_CURRENT_SOURCE_DIR}
			)

# colorimage
set( colorimage_h ${colorimage_h}		
			ColorImageNode.h
			ColorImageNodePlugin.h
			)

set( colorimage_moc ${colorimage_moc}	
			ColorImageNode.h
			ColorImageNodePlugin.h
			)

set( colorimage_src ${colorimage_src}	
			ColorImageNode.cpp
			ColorImageNodePlugin.cpp
			)

set( colorimage_res ${colorimage_res}	
			colorimage.xml
			)

# Create moc files		   
qt4_wrap_cpp(colorimage_cxx ${colorimage_moc})

# Create source groups
source_group("Moc Files" FILES ${colorimage_cxx})
source_group("Header Files" FILES ${colorimage_h})
source_group("Resources" FILES ${colorimage_res})

# Add header files to sources to make headers visible in Visual Studio
IF (WIN32)
  set(colorimage_src ${colorimage_src} ${colorimage_h})
ENDIF(WIN32)

# Create static library
add_library(colorimage SHARED ${colorimage_src} ${colorimage_cxx})

# Add library dependencies
target_link_libraries(colorimage
			optimized QtCore4 debug QtCored4
			optimized QtGui4 debug QtGuid4
			optimized QtXml4 debug QtXmld4
			optimized OgreMain debug OgreMain_d
			optimized frappercore debug frappercore_d
			)

# Install files
install( FILES ${colorimage_res} DESTINATION ${NODE_PLUGIN_INSTALL} )
install( TARGETS colorimage RUNTIME DESTINATION ${NODE_PLUGIN_INSTALL} )