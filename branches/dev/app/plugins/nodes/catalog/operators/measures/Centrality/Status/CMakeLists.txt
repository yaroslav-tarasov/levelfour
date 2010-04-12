project(statusCentrality)
set(CMAKE_CONFIGURATION_TYPES Debug Release CACHE STRING "" FORCE) 
SET(CMAKE_DEBUG_POSTFIX _d)

include_directories(
			${CMAKE_CURRENT_SOURCE_DIR}
			)

# statusCentrality
set( statusCentrality_h ${statusCentrality_h}
			statusCentralityNode.h
			statusCentralityNodePlugin.h
			)

set( statusCentrality_moc ${statusCentrality_moc}	
			statusCentralityNode.h
			statusCentralityNodePlugin.h
			)

set( statusCentrality_src ${statusCentrality_src}	
			statusCentralityNode.cpp
			statusCentralityNodePlugin.cpp
			)

set( statusCentrality_res ${statusCentrality_res}	
			statusCentrality.xml
			)

# Create moc files		   
qt4_wrap_cpp(statusCentrality_cxx ${statusCentrality_moc})

# Create source groups
source_group("Moc Files" FILES ${statusCentrality_cxx})
source_group("Header Files" FILES ${statusCentrality_h})
source_group("Resources" FILES ${statusCentrality_res})

# Add header files to sources to make headers visible in Visual Studio
IF (WIN32)
  set(statusCentrality_src ${statusCentrality_src} ${statusCentrality_h} ${statusCentrality_res})
ENDIF(WIN32)

# Create static library
add_library(statusCentrality SHARED ${statusCentrality_src} ${statusCentrality_cxx})

# Add library dependencies
target_link_libraries(statusCentrality
			optimized QtCore4 debug QtCored4
			optimized QtGui4 debug QtGuid4
			optimized QtXml4 debug QtXmld4
			optimized OgreMain debug OgreMain_d
			optimized frappercore debug frappercore_d
			)

# Install files
install( FILES ${statusCentrality_res} DESTINATION ${NODE_PLUGIN_INSTALL} )
install( TARGETS statusCentrality RUNTIME DESTINATION ${NODE_PLUGIN_INSTALL} )
