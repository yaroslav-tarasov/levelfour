project(colorByValue)
set(CMAKE_CONFIGURATION_TYPES Debug Release CACHE STRING "" FORCE) 
SET(CMAKE_DEBUG_POSTFIX _d)

include_directories(
			${CMAKE_CURRENT_SOURCE_DIR}
			)

# colorByValue
set( colorByValue_h ${colorByValue_h}
			colorByValueNode.h
			colorByValueNodePlugin.h
			)

set( colorByValue_moc ${colorByValue_moc}	
			colorByValueNode.h
			colorByValueNodePlugin.h
			)

set( colorByValue_src ${colorByValue_src}	
			colorByValueNode.cpp
			colorByValueNodePlugin.cpp
			)

set( colorByValue_res ${colorByValue_res}	
			colorByValue.xml
			)

# Create moc files		   
qt4_wrap_cpp(colorByValue_cxx ${colorByValue_moc})

# Create source groups
source_group("Moc Files" FILES ${colorByValue_cxx})
source_group("Header Files" FILES ${colorByValue_h})
source_group("Resources" FILES ${colorByValue_res})

# Add header files to sources to make headers visible in Visual Studio
IF (WIN32)
  set(colorByValue_src ${colorByValue_src} ${colorByValue_h} ${colorByValue_res})
ENDIF(WIN32)

# Create static library
add_library(colorByValue SHARED ${colorByValue_src} ${colorByValue_cxx})

# Add library dependencies
target_link_libraries(colorByValue
			optimized QtCore4 debug QtCored4
			optimized QtGui4 debug QtGuid4
			optimized QtXml4 debug QtXmld4
			optimized OgreMain debug OgreMain_d
			optimized frappercore debug frappercore_d
			)

# Install files
install( FILES ${colorByValue_res} DESTINATION ${NODE_PLUGIN_INSTALL} )
install( TARGETS colorByValue RUNTIME DESTINATION ${NODE_PLUGIN_INSTALL} )
