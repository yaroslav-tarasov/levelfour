project(aqtwatercolor)
set(CMAKE_CONFIGURATION_TYPES Debug Release CACHE STRING "" FORCE) 
SET(CMAKE_DEBUG_POSTFIX _d)

include_directories(
			${CMAKE_CURRENT_SOURCE_DIR}
			)

# aqtwatercolor
set( aqtwatercolor_h ${aqtwatercolor_h}		
			AqtWatercolorNode.h
			AqtWatercolorNodePlugin.h
			)

set( aqtwatercolor_moc ${aqtwatercolor_moc}	
			AqtWatercolorNode.h
			AqtWatercolorNodePlugin.h
			)

set( aqtwatercolor_src ${aqtwatercolor_src}	
			AqtWatercolorNode.cpp
			AqtWatercolorNodePlugin.cpp
			)

set( aqtwatercolor_res ${aqtwatercolor_res}	
			aqtwatercolor.xml
			)

# Create moc files		   
qt4_wrap_cpp(aqtwatercolor_cxx ${aqtwatercolor_moc})

# Create source groups
source_group("Moc Files" FILES ${aqtwatercolor_cxx})
source_group("Header Files" FILES ${aqtwatercolor_h})
source_group("Resources" FILES ${aqtwatercolor_res})

# Add header files to sources to make headers visible in Visual Studio
IF (WIN32)
  set(aqtwatercolor_src ${aqtwatercolor_src} ${aqtwatercolor_h} ${aqtwatercolor_res})
ENDIF(WIN32)

# Create static library
add_library(aqtwatercolor SHARED ${aqtwatercolor_src} ${aqtwatercolor_cxx})

# Add library dependencies
target_link_libraries(aqtwatercolor
			optimized QtCore4 debug QtCored4
			optimized QtGui4 debug QtGuid4
			optimized QtXml4 debug QtXmld4
			optimized OgreMain debug OgreMain_d
			optimized frappercore debug frappercore_d
			)

# Install files
install( FILES ${aqtwatercolor_res} DESTINATION ${NODE_PLUGIN_INSTALL} )
install( TARGETS aqtwatercolor RUNTIME DESTINATION ${NODE_PLUGIN_INSTALL} )
