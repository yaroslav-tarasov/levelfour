project(positionByValue)
set(CMAKE_CONFIGURATION_TYPES Debug Release CACHE STRING "" FORCE) 
SET(CMAKE_DEBUG_POSTFIX _d)

include_directories(
			${CMAKE_CURRENT_SOURCE_DIR}
			)

# positionByValue
set( positionByValue_h ${positionByValue_h}
			positionByValueNode.h
			positionByValueNodePlugin.h
			)

set( positionByValue_moc ${positionByValue_moc}	
			positionByValueNode.h
			positionByValueNodePlugin.h
			)

set( positionByValue_src ${positionByValue_src}	
			positionByValueNode.cpp
			positionByValueNodePlugin.cpp
			)

set( positionByValue_res ${positionByValue_res}	
			positionByValue.xml
			)

# Create moc files		   
qt4_wrap_cpp(positionByValue_cxx ${positionByValue_moc})

# Create source groups
source_group("Moc Files" FILES ${positionByValue_cxx})
source_group("Header Files" FILES ${positionByValue_h})
source_group("Resources" FILES ${positionByValue_res})

# Add header files to sources to make headers visible in Visual Studio
IF (WIN32)
  set(positionByValue_src ${positionByValue_src} ${positionByValue_h} ${positionByValue_res})
ENDIF(WIN32)

# Create static library
add_library(positionByValue SHARED ${positionByValue_src} ${positionByValue_cxx})

# Add library dependencies
target_link_libraries(positionByValue
			optimized QtCore4 debug QtCored4
			optimized QtGui4 debug QtGuid4
			optimized QtXml4 debug QtXmld4
			optimized OgreMain debug OgreMain_d
			optimized frappercore debug frappercore_d
			)

# Install files
install( FILES ${positionByValue_res} DESTINATION ${NODE_PLUGIN_INSTALL} )
install( TARGETS positionByValue RUNTIME DESTINATION ${NODE_PLUGIN_INSTALL} )
