project(shapeByValue)
set(CMAKE_CONFIGURATION_TYPES Debug Release CACHE STRING "" FORCE) 
SET(CMAKE_DEBUG_POSTFIX _d)

include_directories(
			${CMAKE_CURRENT_SOURCE_DIR}
			)

# shapeByValue
set( shapeByValue_h ${shapeByValue_h}
			shapeByValueNode.h
			shapeByValueNodePlugin.h
			)

set( shapeByValue_moc ${shapeByValue_moc}	
			shapeByValueNode.h
			shapeByValueNodePlugin.h
			)

set( shapeByValue_src ${shapeByValue_src}	
			shapeByValueNode.cpp
			shapeByValueNodePlugin.cpp
			)

set( shapeByValue_res ${shapeByValue_res}	
			shapeByValue.xml
			)

# Create moc files		   
qt4_wrap_cpp(shapeByValue_cxx ${shapeByValue_moc})

# Create source groups
source_group("Moc Files" FILES ${shapeByValue_cxx})
source_group("Header Files" FILES ${shapeByValue_h})
source_group("Resources" FILES ${shapeByValue_res})

# Add header files to sources to make headers visible in Visual Studio
IF (WIN32)
  set(shapeByValue_src ${shapeByValue_src} ${shapeByValue_h} ${shapeByValue_res})
ENDIF(WIN32)

# Create static library
add_library(shapeByValue SHARED ${shapeByValue_src} ${shapeByValue_cxx})

# Add library dependencies
target_link_libraries(shapeByValue
			optimized QtCore4 debug QtCored4
			optimized QtGui4 debug QtGuid4
			optimized QtXml4 debug QtXmld4
			optimized OgreMain debug OgreMain_d
			optimized frappercore debug frappercore_d
			)

# Install files
install( FILES ${shapeByValue_res} DESTINATION ${NODE_PLUGIN_INSTALL} )
install( TARGETS shapeByValue RUNTIME DESTINATION ${NODE_PLUGIN_INSTALL} )
