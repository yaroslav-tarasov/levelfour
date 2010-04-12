project(nodeBetweenessCentrality)
set(CMAKE_CONFIGURATION_TYPES Debug Release CACHE STRING "" FORCE) 
SET(CMAKE_DEBUG_POSTFIX _d)

include_directories(
			${CMAKE_CURRENT_SOURCE_DIR}
			)

# nodeBetweenessCentrality
set( nodeBetweenessCentrality_h ${nodeBetweenessCentrality_h}
			nodeBetweenessCentralityNode.h
			nodeBetweenessCentralityNodePlugin.h
			)

set( nodeBetweenessCentrality_moc ${nodeBetweenessCentrality_moc}	
			nodeBetweenessCentralityNode.h
			nodeBetweenessCentralityNodePlugin.h
			)

set( nodeBetweenessCentrality_src ${nodeBetweenessCentrality_src}	
			nodeBetweenessCentralityNode.cpp
			nodeBetweenessCentralityNodePlugin.cpp
			)

set( nodeBetweenessCentrality_res ${nodeBetweenessCentrality_res}	
			nodeBetweenessCentrality.xml
			)

# Create moc files		   
qt4_wrap_cpp(nodeBetweenessCentrality_cxx ${nodeBetweenessCentrality_moc})

# Create source groups
source_group("Moc Files" FILES ${nodeBetweenessCentrality_cxx})
source_group("Header Files" FILES ${nodeBetweenessCentrality_h})
source_group("Resources" FILES ${nodeBetweenessCentrality_res})

# Add header files to sources to make headers visible in Visual Studio
IF (WIN32)
  set(nodeBetweenessCentrality_src ${nodeBetweenessCentrality_src} ${nodeBetweenessCentrality_h} ${nodeBetweenessCentrality_res})
ENDIF(WIN32)

# Create static library
add_library(nodeBetweenessCentrality SHARED ${nodeBetweenessCentrality_src} ${nodeBetweenessCentrality_cxx})

# Add library dependencies
target_link_libraries(nodeBetweenessCentrality
			optimized QtCore4 debug QtCored4
			optimized QtGui4 debug QtGuid4
			optimized QtXml4 debug QtXmld4
			optimized OgreMain debug OgreMain_d
			optimized frappercore debug frappercore_d
			)

# Install files
install( FILES ${nodeBetweenessCentrality_res} DESTINATION ${NODE_PLUGIN_INSTALL} )
install( TARGETS nodeBetweenessCentrality RUNTIME DESTINATION ${NODE_PLUGIN_INSTALL} )
