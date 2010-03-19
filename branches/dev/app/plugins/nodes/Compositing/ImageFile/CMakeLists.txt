project(imagefile)
set(CMAKE_CONFIGURATION_TYPES Debug Release CACHE STRING "" FORCE) 
SET(CMAKE_DEBUG_POSTFIX _d)

include_directories(
			${CMAKE_CURRENT_SOURCE_DIR}
			)

# imagefile
set( imagefile_h ${imagefile_h}		
			ImageFileNode.h
			ImageFileNodePlugin.h
			)

set( imagefile_moc ${imagefile_moc}	
			ImageFileNode.h
			ImageFileNodePlugin.h
			)

set( imagefile_src ${imagefile_src}	
			ImageFileNode.cpp
			ImageFileNodePlugin.cpp
			)
			
set( imagefile_res ${imagefile_res}	
			imagefile.xml
			)

# Create moc files		   
qt4_wrap_cpp(imagefile_cxx ${imagefile_moc})

# Create source groups
source_group("Moc Files" FILES ${imagefile_cxx})
source_group("Header Files" FILES ${imagefile_h})
source_group("Resources" FILES ${imagefile_res})

# Add header files to sources to make headers visible in Visual Studio
IF (WIN32)
  set(imagefile_src ${imagefile_src} ${imagefile_h} ${imagefile_res})
ENDIF(WIN32)

# Create static library
add_library(imagefile SHARED ${imagefile_src} ${imagefile_cxx})

# Add library dependencies
target_link_libraries(imagefile
			optimized QtCore4 debug QtCored4
			optimized QtGui4 debug QtGuid4
			optimized QtXml4 debug QtXmld4
			optimized OgreMain debug OgreMain_d
			optimized frappercore debug frappercore_d
			)

# Install files
install( FILES ${imagefile_res} DESTINATION ${NODE_PLUGIN_INSTALL} )
install( TARGETS imagefile RUNTIME DESTINATION ${NODE_PLUGIN_INSTALL} )