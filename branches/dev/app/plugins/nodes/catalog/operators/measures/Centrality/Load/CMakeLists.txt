project(loadCentrality)
set(CMAKE_CONFIGURATION_TYPES Debug Release CACHE STRING "" FORCE) 
SET(CMAKE_DEBUG_POSTFIX _d)

include_directories(
			${CMAKE_CURRENT_SOURCE_DIR}
			)

# loadCentrality
set( loadCentrality_h ${loadCentrality_h}
			loadCentralityNode.h
			loadCentralityNodePlugin.h
			)

set( loadCentrality_moc ${loadCentrality_moc}	
			loadCentralityNode.h
			loadCentralityNodePlugin.h
			)

set( loadCentrality_src ${loadCentrality_src}	
			loadCentralityNode.cpp
			loadCentralityNodePlugin.cpp
			)

set( loadCentrality_res ${loadCentrality_res}	
			loadCentrality.xml
			)

# Create moc files		   
qt4_wrap_cpp(loadCentrality_cxx ${loadCentrality_moc})

# Create source groups
source_group("Moc Files" FILES ${loadCentrality_cxx})
source_group("Header Files" FILES ${loadCentrality_h})
source_group("Resources" FILES ${loadCentrality_res})

# Add header files to sources to make headers visible in Visual Studio
IF (WIN32)
  set(loadCentrality_src ${loadCentrality_src} ${loadCentrality_h} ${loadCentrality_res})
ENDIF(WIN32)

# Create static library
add_library(loadCentrality SHARED ${loadCentrality_src} ${loadCentrality_cxx})

# Add library dependencies
target_link_libraries(loadCentrality
			optimized QtCore4 debug QtCored4
			optimized QtGui4 debug QtGuid4
			optimized QtXml4 debug QtXmld4
			optimized OgreMain debug OgreMain_d
			optimized frappercore debug frappercore_d
			)

# Install files
install( FILES ${loadCentrality_res} DESTINATION ${NODE_PLUGIN_INSTALL} )
install( TARGETS loadCentrality RUNTIME DESTINATION ${NODE_PLUGIN_INSTALL} )
