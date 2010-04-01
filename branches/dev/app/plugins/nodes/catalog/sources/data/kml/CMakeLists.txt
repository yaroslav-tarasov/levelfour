project(kmlSource)
set(CMAKE_CONFIGURATION_TYPES Debug Release CACHE STRING "" FORCE) 
SET(CMAKE_DEBUG_POSTFIX _d)

include_directories(
			${CMAKE_CURRENT_SOURCE_DIR}
			)

# kmlSource
set( kmlSource_h ${kmlSource_h}
			kmlSourceNode.h
			kmlSourceNodePlugin.h
			)

set( kmlSource_moc ${kmlSource_moc}	
			kmlSourceNode.h
			kmlSourceNodePlugin.h
			)

set( kmlSource_src ${kmlSource_src}	
			kmlSourceNode.cpp
			kmlSourceNodePlugin.cpp
			)

set( kmlSource_res ${kmlSource_res}	
			kmlSource.xml
			)

# Create moc files		   
qt4_wrap_cpp(kmlSource_cxx ${kmlSource_moc})

# Create source groups
source_group("Moc Files" FILES ${kmlSource_cxx})
source_group("Header Files" FILES ${kmlSource_h})
source_group("Resources" FILES ${kmlSource_res})

# Add header files to sources to make headers visible in Visual Studio
IF (WIN32)
  set(kmlSource_src ${kmlSource_src} ${kmlSource_h} ${kmlSource_res})
ENDIF(WIN32)

# Create static library
add_library(kmlSource SHARED ${kmlSource_src} ${kmlSource_cxx})

# Add library dependencies
target_link_libraries(kmlSource
			optimized QtCore4 debug QtCored4
			optimized QtGui4 debug QtGuid4
			optimized QtXml4 debug QtXmld4
			optimized OgreMain debug OgreMain_d
			optimized frappercore debug frappercore_d
			)

# Install files
install( FILES ${kmlSource_res} DESTINATION ${NODE_PLUGIN_INSTALL} )
install( TARGETS kmlSource RUNTIME DESTINATION ${NODE_PLUGIN_INSTALL} )
