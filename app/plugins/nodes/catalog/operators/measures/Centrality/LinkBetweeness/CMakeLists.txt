project(linkBetweenessCentrality)
set(CMAKE_CONFIGURATION_TYPES Debug Release CACHE STRING "" FORCE) 
SET(CMAKE_DEBUG_POSTFIX _d)

include_directories(
			${CMAKE_CURRENT_SOURCE_DIR}
			)

# linkBetweenessCentrality
set( linkBetweenessCentrality_h ${linkBetweenessCentrality_h}
			linkBetweenessCentralityNode.h
			linkBetweenessCentralityNodePlugin.h
			)

set( linkBetweenessCentrality_moc ${linkBetweenessCentrality_moc}	
			linkBetweenessCentralityNode.h
			linkBetweenessCentralityNodePlugin.h
			)

set( linkBetweenessCentrality_src ${linkBetweenessCentrality_src}	
			linkBetweenessCentralityNode.cpp
			linkBetweenessCentralityNodePlugin.cpp
			)

set( linkBetweenessCentrality_res ${linkBetweenessCentrality_res}	
			linkBetweenessCentrality.xml
			)

# Create moc files		   
qt4_wrap_cpp(linkBetweenessCentrality_cxx ${linkBetweenessCentrality_moc})

# Create source groups
source_group("Moc Files" FILES ${linkBetweenessCentrality_cxx})
source_group("Header Files" FILES ${linkBetweenessCentrality_h})
source_group("Resources" FILES ${linkBetweenessCentrality_res})

# Add header files to sources to make headers visible in Visual Studio
IF (WIN32)
  set(linkBetweenessCentrality_src ${linkBetweenessCentrality_src} ${linkBetweenessCentrality_h} ${linkBetweenessCentrality_res})
ENDIF(WIN32)

# Create static library
add_library(linkBetweenessCentrality SHARED ${linkBetweenessCentrality_src} ${linkBetweenessCentrality_cxx})

# Add library dependencies
target_link_libraries(linkBetweenessCentrality
			optimized QtCore4 debug QtCored4
			optimized QtGui4 debug QtGuid4
			optimized QtXml4 debug QtXmld4
			optimized OgreMain debug OgreMain_d
			optimized frappercore debug frappercore_d
			)

# Install files
install( FILES ${linkBetweenessCentrality_res} DESTINATION ${NODE_PLUGIN_INSTALL} )
install( TARGETS linkBetweenessCentrality RUNTIME DESTINATION ${NODE_PLUGIN_INSTALL} )
