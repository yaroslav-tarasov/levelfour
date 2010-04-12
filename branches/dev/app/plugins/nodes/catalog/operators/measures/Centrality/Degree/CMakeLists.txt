project(degreeCentrality)
set(CMAKE_CONFIGURATION_TYPES Debug Release CACHE STRING "" FORCE) 
SET(CMAKE_DEBUG_POSTFIX _d)

include_directories(
			${CMAKE_CURRENT_SOURCE_DIR}
			)

# degreeCentrality
set( degreeCentrality_h ${degreeCentrality_h}
			degreeCentralityNode.h
			degreeCentralityNodePlugin.h
			)

set( degreeCentrality_moc ${degreeCentrality_moc}	
			degreeCentralityNode.h
			degreeCentralityNodePlugin.h
			)

set( degreeCentrality_src ${degreeCentrality_src}	
			degreeCentralityNode.cpp
			degreeCentralityNodePlugin.cpp
			)

set( degreeCentrality_res ${degreeCentrality_res}	
			degreeCentrality.xml
			)

# Create moc files		   
qt4_wrap_cpp(degreeCentrality_cxx ${degreeCentrality_moc})

# Create source groups
source_group("Moc Files" FILES ${degreeCentrality_cxx})
source_group("Header Files" FILES ${degreeCentrality_h})
source_group("Resources" FILES ${degreeCentrality_res})

# Add header files to sources to make headers visible in Visual Studio
IF (WIN32)
  set(degreeCentrality_src ${degreeCentrality_src} ${degreeCentrality_h} ${degreeCentrality_res})
ENDIF(WIN32)

# Create static library
add_library(degreeCentrality SHARED ${degreeCentrality_src} ${degreeCentrality_cxx})

# Add library dependencies
target_link_libraries(degreeCentrality
			optimized QtCore4 debug QtCored4
			optimized QtGui4 debug QtGuid4
			optimized QtXml4 debug QtXmld4
			optimized OgreMain debug OgreMain_d
			optimized frappercore debug frappercore_d
			)

# Install files
install( FILES ${degreeCentrality_res} DESTINATION ${NODE_PLUGIN_INSTALL} )
install( TARGETS degreeCentrality RUNTIME DESTINATION ${NODE_PLUGIN_INSTALL} )
