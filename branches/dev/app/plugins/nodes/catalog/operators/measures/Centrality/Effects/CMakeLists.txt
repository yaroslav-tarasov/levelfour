project(effectsCentrality)
set(CMAKE_CONFIGURATION_TYPES Debug Release CACHE STRING "" FORCE) 
SET(CMAKE_DEBUG_POSTFIX _d)

include_directories(
			${CMAKE_CURRENT_SOURCE_DIR}
			)

# effectsCentrality
set( effectsCentrality_h ${effectsCentrality_h}
			effectsCentralityNode.h
			effectsCentralityNodePlugin.h
			)

set( effectsCentrality_moc ${effectsCentrality_moc}	
			effectsCentralityNode.h
			effectsCentralityNodePlugin.h
			)

set( effectsCentrality_src ${effectsCentrality_src}	
			effectsCentralityNode.cpp
			effectsCentralityNodePlugin.cpp
			)

set( effectsCentrality_res ${effectsCentrality_res}	
			effectsCentrality.xml
			)

# Create moc files		   
qt4_wrap_cpp(effectsCentrality_cxx ${effectsCentrality_moc})

# Create source groups
source_group("Moc Files" FILES ${effectsCentrality_cxx})
source_group("Header Files" FILES ${effectsCentrality_h})
source_group("Resources" FILES ${effectsCentrality_res})

# Add header files to sources to make headers visible in Visual Studio
IF (WIN32)
  set(effectsCentrality_src ${effectsCentrality_src} ${effectsCentrality_h} ${effectsCentrality_res})
ENDIF(WIN32)

# Create static library
add_library(effectsCentrality SHARED ${effectsCentrality_src} ${effectsCentrality_cxx})

# Add library dependencies
target_link_libraries(effectsCentrality
			optimized QtCore4 debug QtCored4
			optimized QtGui4 debug QtGuid4
			optimized QtXml4 debug QtXmld4
			optimized OgreMain debug OgreMain_d
			optimized frappercore debug frappercore_d
			)

# Install files
install( FILES ${effectsCentrality_res} DESTINATION ${NODE_PLUGIN_INSTALL} )
install( TARGETS effectsCentrality RUNTIME DESTINATION ${NODE_PLUGIN_INSTALL} )
