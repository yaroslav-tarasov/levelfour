project(delimitedSource)
set(CMAKE_CONFIGURATION_TYPES Debug Release CACHE STRING "" FORCE) 
SET(CMAKE_DEBUG_POSTFIX _d)

include_directories(
			${CMAKE_CURRENT_SOURCE_DIR}
			)

# delimitedSource
set( delimitedSource_h ${delimitedSource_h}
			delimitedSourceNode.h
			delimitedSourceNodePlugin.h
			)

set( delimitedSource_moc ${delimitedSource_moc}	
			delimitedSourceNode.h
			delimitedSourceNodePlugin.h
			)

set( delimitedSource_src ${delimitedSource_src}	
			delimitedSourceNode.cpp
			delimitedSourceNodePlugin.cpp
			)

set( delimitedSource_res ${delimitedSource_res}	
			delimitedSource.xml
			)

# Create moc files		   
qt4_wrap_cpp(delimitedSource_cxx ${delimitedSource_moc})

# Create source groups
source_group("Moc Files" FILES ${delimitedSource_cxx})
source_group("Header Files" FILES ${delimitedSource_h})
source_group("Resources" FILES ${delimitedSource_res})

# Add header files to sources to make headers visible in Visual Studio
IF (WIN32)
  set(delimitedSource_src ${delimitedSource_src} ${delimitedSource_h} ${delimitedSource_res})
ENDIF(WIN32)

# Create static library
add_library(delimitedSource SHARED ${delimitedSource_src} ${delimitedSource_cxx})

# Add library dependencies
target_link_libraries(delimitedSource
			optimized QtCore4 debug QtCored4
			optimized QtGui4 debug QtGuid4
			optimized QtXml4 debug QtXmld4
			optimized OgreMain debug OgreMain_d
			optimized frappercore debug frappercore_d
			)

# Install files
install( FILES ${delimitedSource_res} DESTINATION ${NODE_PLUGIN_INSTALL} )
install( TARGETS delimitedSource RUNTIME DESTINATION ${NODE_PLUGIN_INSTALL} )
