project(closenessCentrality)
set(CMAKE_CONFIGURATION_TYPES Debug Release CACHE STRING "" FORCE) 
SET(CMAKE_DEBUG_POSTFIX _d)

include_directories(
			${CMAKE_CURRENT_SOURCE_DIR}
			)

# closenessCentrality
set( closenessCentrality_h ${closenessCentrality_h}
			closenessCentralityNode.h
			closenessCentralityNodePlugin.h
			)

set( closenessCentrality_moc ${closenessCentrality_moc}	
			closenessCentralityNode.h
			closenessCentralityNodePlugin.h
			)

set( closenessCentrality_src ${closenessCentrality_src}	
			closenessCentralityNode.cpp
			closenessCentralityNodePlugin.cpp
			)

set( closenessCentrality_res ${closenessCentrality_res}	
			closenessCentrality.xml
			)

# Create moc files		   
qt4_wrap_cpp(closenessCentrality_cxx ${closenessCentrality_moc})

# Create source groups
source_group("Moc Files" FILES ${closenessCentrality_cxx})
source_group("Header Files" FILES ${closenessCentrality_h})
source_group("Resources" FILES ${closenessCentrality_res})

# Add header files to sources to make headers visible in Visual Studio
IF (WIN32)
  set(closenessCentrality_src ${closenessCentrality_src} ${closenessCentrality_h} ${closenessCentrality_res})
ENDIF(WIN32)

# Create static library
add_library(closenessCentrality SHARED ${closenessCentrality_src} ${closenessCentrality_cxx})

# Add library dependencies
target_link_libraries(closenessCentrality
			optimized QtCore4 debug QtCored4
			optimized QtGui4 debug QtGuid4
			optimized QtXml4 debug QtXmld4
			optimized OgreMain debug OgreMain_d
			optimized frappercore debug frappercore_d
			)

# Install files
install( FILES ${closenessCentrality_res} DESTINATION ${NODE_PLUGIN_INSTALL} )
install( TARGETS closenessCentrality RUNTIME DESTINATION ${NODE_PLUGIN_INSTALL} )
