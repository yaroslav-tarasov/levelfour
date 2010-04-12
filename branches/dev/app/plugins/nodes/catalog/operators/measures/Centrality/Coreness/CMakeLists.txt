project(corenessCentrality)
set(CMAKE_CONFIGURATION_TYPES Debug Release CACHE STRING "" FORCE) 
SET(CMAKE_DEBUG_POSTFIX _d)

include_directories(
			${CMAKE_CURRENT_SOURCE_DIR}
			)

# corenessCentrality
set( corenessCentrality_h ${corenessCentrality_h}
			corenessCentralityNode.h
			corenessCentralityNodePlugin.h
			)

set( corenessCentrality_moc ${corenessCentrality_moc}	
			corenessCentralityNode.h
			corenessCentralityNodePlugin.h
			)

set( corenessCentrality_src ${corenessCentrality_src}	
			corenessCentralityNode.cpp
			corenessCentralityNodePlugin.cpp
			)

set( corenessCentrality_res ${corenessCentrality_res}	
			corenessCentrality.xml
			)

# Create moc files		   
qt4_wrap_cpp(corenessCentrality_cxx ${corenessCentrality_moc})

# Create source groups
source_group("Moc Files" FILES ${corenessCentrality_cxx})
source_group("Header Files" FILES ${corenessCentrality_h})
source_group("Resources" FILES ${corenessCentrality_res})

# Add header files to sources to make headers visible in Visual Studio
IF (WIN32)
  set(corenessCentrality_src ${corenessCentrality_src} ${corenessCentrality_h} ${corenessCentrality_res})
ENDIF(WIN32)

# Create static library
add_library(corenessCentrality SHARED ${corenessCentrality_src} ${corenessCentrality_cxx})

# Add library dependencies
target_link_libraries(corenessCentrality
			optimized QtCore4 debug QtCored4
			optimized QtGui4 debug QtGuid4
			optimized QtXml4 debug QtXmld4
			optimized OgreMain debug OgreMain_d
			optimized frappercore debug frappercore_d
			)

# Install files
install( FILES ${corenessCentrality_res} DESTINATION ${NODE_PLUGIN_INSTALL} )
install( TARGETS corenessCentrality RUNTIME DESTINATION ${NODE_PLUGIN_INSTALL} )
