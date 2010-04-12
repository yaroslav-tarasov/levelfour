project(randomWalkCentrality)
set(CMAKE_CONFIGURATION_TYPES Debug Release CACHE STRING "" FORCE) 
SET(CMAKE_DEBUG_POSTFIX _d)

include_directories(
			${CMAKE_CURRENT_SOURCE_DIR}
			)

# randomWalkCentrality
set( randomWalkCentrality_h ${randomWalkCentrality_h}
			randomWalkCentralityNode.h
			randomWalkCentralityNodePlugin.h
			)

set( randomWalkCentrality_moc ${randomWalkCentrality_moc}	
			randomWalkCentralityNode.h
			randomWalkCentralityNodePlugin.h
			)

set( randomWalkCentrality_src ${randomWalkCentrality_src}	
			randomWalkCentralityNode.cpp
			randomWalkCentralityNodePlugin.cpp
			)

set( randomWalkCentrality_res ${randomWalkCentrality_res}	
			randomWalkCentrality.xml
			)

# Create moc files		   
qt4_wrap_cpp(randomWalkCentrality_cxx ${randomWalkCentrality_moc})

# Create source groups
source_group("Moc Files" FILES ${randomWalkCentrality_cxx})
source_group("Header Files" FILES ${randomWalkCentrality_h})
source_group("Resources" FILES ${randomWalkCentrality_res})

# Add header files to sources to make headers visible in Visual Studio
IF (WIN32)
  set(randomWalkCentrality_src ${randomWalkCentrality_src} ${randomWalkCentrality_h} ${randomWalkCentrality_res})
ENDIF(WIN32)

# Create static library
add_library(randomWalkCentrality SHARED ${randomWalkCentrality_src} ${randomWalkCentrality_cxx})

# Add library dependencies
target_link_libraries(randomWalkCentrality
			optimized QtCore4 debug QtCored4
			optimized QtGui4 debug QtGuid4
			optimized QtXml4 debug QtXmld4
			optimized OgreMain debug OgreMain_d
			optimized frappercore debug frappercore_d
			)

# Install files
install( FILES ${randomWalkCentrality_res} DESTINATION ${NODE_PLUGIN_INSTALL} )
install( TARGETS randomWalkCentrality RUNTIME DESTINATION ${NODE_PLUGIN_INSTALL} )
