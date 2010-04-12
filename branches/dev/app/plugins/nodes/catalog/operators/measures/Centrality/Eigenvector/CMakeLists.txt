project(eigenvectorCentrality)
set(CMAKE_CONFIGURATION_TYPES Debug Release CACHE STRING "" FORCE) 
SET(CMAKE_DEBUG_POSTFIX _d)

include_directories(
			${CMAKE_CURRENT_SOURCE_DIR}
			)

# eigenvectorCentrality
set( eigenvectorCentrality_h ${eigenvectorCentrality_h}
			eigenvectorCentralityNode.h
			eigenvectorCentralityNodePlugin.h
			)

set( eigenvectorCentrality_moc ${eigenvectorCentrality_moc}	
			eigenvectorCentralityNode.h
			eigenvectorCentralityNodePlugin.h
			)

set( eigenvectorCentrality_src ${eigenvectorCentrality_src}	
			eigenvectorCentralityNode.cpp
			eigenvectorCentralityNodePlugin.cpp
			)

set( eigenvectorCentrality_res ${eigenvectorCentrality_res}	
			eigenvectorCentrality.xml
			)

# Create moc files		   
qt4_wrap_cpp(eigenvectorCentrality_cxx ${eigenvectorCentrality_moc})

# Create source groups
source_group("Moc Files" FILES ${eigenvectorCentrality_cxx})
source_group("Header Files" FILES ${eigenvectorCentrality_h})
source_group("Resources" FILES ${eigenvectorCentrality_res})

# Add header files to sources to make headers visible in Visual Studio
IF (WIN32)
  set(eigenvectorCentrality_src ${eigenvectorCentrality_src} ${eigenvectorCentrality_h} ${eigenvectorCentrality_res})
ENDIF(WIN32)

# Create static library
add_library(eigenvectorCentrality SHARED ${eigenvectorCentrality_src} ${eigenvectorCentrality_cxx})

# Add library dependencies
target_link_libraries(eigenvectorCentrality
			optimized QtCore4 debug QtCored4
			optimized QtGui4 debug QtGuid4
			optimized QtXml4 debug QtXmld4
			optimized OgreMain debug OgreMain_d
			optimized frappercore debug frappercore_d
			)

# Install files
install( FILES ${eigenvectorCentrality_res} DESTINATION ${NODE_PLUGIN_INSTALL} )
install( TARGETS eigenvectorCentrality RUNTIME DESTINATION ${NODE_PLUGIN_INSTALL} )
