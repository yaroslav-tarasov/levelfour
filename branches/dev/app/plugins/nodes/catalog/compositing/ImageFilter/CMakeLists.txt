project(imagefilter)
set(CMAKE_CONFIGURATION_TYPES Debug Release CACHE STRING "" FORCE) 
SET(CMAKE_DEBUG_POSTFIX _d)

include_directories(
			${CMAKE_CURRENT_SOURCE_DIR}
			)

# imagefilter
set( imagefilter_h ${imagefilter_h}		
			ImageFilterNode.h
			ImageFilterNodePlugin.h
			)

set( imagefilter_moc ${imagefilter_moc}	
			ImageFilterNode.h
			ImageFilterNodePlugin.h
			)

set( imagefilter_src ${imagefilter_src}	
			ImageFilterNode.cpp
			ImageFilterNodePlugin.cpp
			)

set( imagefilter_res ${imagefilter_res}	
			imagefilter.xml
			)

# Create moc files		   
qt4_wrap_cpp(imagefilter_cxx ${imagefilter_moc})

# Create source groups
source_group("Moc Files" FILES ${imagefilter_cxx})
source_group("Header Files" FILES ${imagefilter_h})
source_group("Resources" FILES ${imagefilter_res})

# Add header files to sources to make headers visible in Visual Studio
IF (WIN32)
  set(imagefilter_src ${imagefilter_src} ${imagefilter_h} ${imagefilter_res})
ENDIF(WIN32)

# Create static library
add_library(imagefilter SHARED ${imagefilter_src} ${imagefilter_cxx})

# Add library dependencies
target_link_libraries(imagefilter
			optimized QtCore4 debug QtCored4
			optimized QtGui4 debug QtGuid4
			optimized QtXml4 debug QtXmld4
			optimized OgreMain debug OgreMain_d
			optimized frappercore debug frappercore_d
			)

# Install files
install( FILES ${imagefilter_res} DESTINATION ${NODE_PLUGIN_INSTALL} )
install( TARGETS imagefilter RUNTIME DESTINATION ${NODE_PLUGIN_INSTALL} )
