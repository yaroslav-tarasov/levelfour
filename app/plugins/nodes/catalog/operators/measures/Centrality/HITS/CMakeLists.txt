project(hitsCentrality)
set(CMAKE_CONFIGURATION_TYPES Debug Release CACHE STRING "" FORCE) 
SET(CMAKE_DEBUG_POSTFIX _d)

include_directories(
			${CMAKE_CURRENT_SOURCE_DIR}
			)

# hitsCentrality
set( hitsCentrality_h ${hitsCentrality_h}
			hitsCentralityNode.h
			hitsCentralityNodePlugin.h
			)

set( hitsCentrality_moc ${hitsCentrality_moc}	
			hitsCentralityNode.h
			hitsCentralityNodePlugin.h
			)

set( hitsCentrality_src ${hitsCentrality_src}	
			hitsCentralityNode.cpp
			hitsCentralityNodePlugin.cpp
			)

set( hitsCentrality_res ${hitsCentrality_res}	
			hitsCentrality.xml
			)

# Create moc files		   
qt4_wrap_cpp(hitsCentrality_cxx ${hitsCentrality_moc})

# Create source groups
source_group("Moc Files" FILES ${hitsCentrality_cxx})
source_group("Header Files" FILES ${hitsCentrality_h})
source_group("Resources" FILES ${hitsCentrality_res})

# Add header files to sources to make headers visible in Visual Studio
IF (WIN32)
  set(hitsCentrality_src ${hitsCentrality_src} ${hitsCentrality_h} ${hitsCentrality_res})
ENDIF(WIN32)

# Create static library
add_library(hitsCentrality SHARED ${hitsCentrality_src} ${hitsCentrality_cxx})

# Add library dependencies
target_link_libraries(hitsCentrality
			optimized QtCore4 debug QtCored4
			optimized QtGui4 debug QtGuid4
			optimized QtXml4 debug QtXmld4
			optimized OgreMain debug OgreMain_d
			optimized frappercore debug frappercore_d
			)

# Install files
install( FILES ${hitsCentrality_res} DESTINATION ${NODE_PLUGIN_INSTALL} )
install( TARGETS hitsCentrality RUNTIME DESTINATION ${NODE_PLUGIN_INSTALL} )
