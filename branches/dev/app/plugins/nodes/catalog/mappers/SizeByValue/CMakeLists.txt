project(sizeByValue)
set(CMAKE_CONFIGURATION_TYPES Debug Release CACHE STRING "" FORCE) 
SET(CMAKE_DEBUG_POSTFIX _d)

include_directories(
			${CMAKE_CURRENT_SOURCE_DIR}
			)

# sizeByValue
set( sizeByValue_h ${sizeByValue_h}
			sizeByValueNode.h
			sizeByValueNodePlugin.h
			)

set( sizeByValue_moc ${sizeByValue_moc}	
			sizeByValueNode.h
			sizeByValueNodePlugin.h
			)

set( sizeByValue_src ${sizeByValue_src}	
			sizeByValueNode.cpp
			sizeByValueNodePlugin.cpp
			)

set( sizeByValue_res ${sizeByValue_res}	
			sizeByValue.xml
			)

# Create moc files		   
qt4_wrap_cpp(sizeByValue_cxx ${sizeByValue_moc})

# Create source groups
source_group("Moc Files" FILES ${sizeByValue_cxx})
source_group("Header Files" FILES ${sizeByValue_h})
source_group("Resources" FILES ${sizeByValue_res})

# Add header files to sources to make headers visible in Visual Studio
IF (WIN32)
  set(sizeByValue_src ${sizeByValue_src} ${sizeByValue_h} ${sizeByValue_res})
ENDIF(WIN32)

# Create static library
add_library(sizeByValue SHARED ${sizeByValue_src} ${sizeByValue_cxx})

# Add library dependencies
target_link_libraries(sizeByValue
			optimized QtCore4 debug QtCored4
			optimized QtGui4 debug QtGuid4
			optimized QtXml4 debug QtXmld4
			optimized OgreMain debug OgreMain_d
			optimized frappercore debug frappercore_d
			)

# Install files
install( FILES ${sizeByValue_res} DESTINATION ${NODE_PLUGIN_INSTALL} )
install( TARGETS sizeByValue RUNTIME DESTINATION ${NODE_PLUGIN_INSTALL} )
