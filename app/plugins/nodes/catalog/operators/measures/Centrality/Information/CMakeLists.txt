project(informationCentrality)
set(CMAKE_CONFIGURATION_TYPES Debug Release CACHE STRING "" FORCE) 
SET(CMAKE_DEBUG_POSTFIX _d)

include_directories(
			${CMAKE_CURRENT_SOURCE_DIR}
			)

# informationCentrality
set( informationCentrality_h ${informationCentrality_h}
			informationCentralityNode.h
			informationCentralityNodePlugin.h
			)

set( informationCentrality_moc ${informationCentrality_moc}	
			informationCentralityNode.h
			informationCentralityNodePlugin.h
			)

set( informationCentrality_src ${informationCentrality_src}	
			informationCentralityNode.cpp
			informationCentralityNodePlugin.cpp
			)

set( informationCentrality_res ${informationCentrality_res}	
			informationCentrality.xml
			)

# Create moc files		   
qt4_wrap_cpp(informationCentrality_cxx ${informationCentrality_moc})

# Create source groups
source_group("Moc Files" FILES ${informationCentrality_cxx})
source_group("Header Files" FILES ${informationCentrality_h})
source_group("Resources" FILES ${informationCentrality_res})

# Add header files to sources to make headers visible in Visual Studio
IF (WIN32)
  set(informationCentrality_src ${informationCentrality_src} ${informationCentrality_h} ${informationCentrality_res})
ENDIF(WIN32)

# Create static library
add_library(informationCentrality SHARED ${informationCentrality_src} ${informationCentrality_cxx})

# Add library dependencies
target_link_libraries(informationCentrality
			optimized QtCore4 debug QtCored4
			optimized QtGui4 debug QtGuid4
			optimized QtXml4 debug QtXmld4
			optimized OgreMain debug OgreMain_d
			optimized frappercore debug frappercore_d
			)

# Install files
install( FILES ${informationCentrality_res} DESTINATION ${NODE_PLUGIN_INSTALL} )
install( TARGETS informationCentrality RUNTIME DESTINATION ${NODE_PLUGIN_INSTALL} )
