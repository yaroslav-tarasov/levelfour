project(powerCentrality)
set(CMAKE_CONFIGURATION_TYPES Debug Release CACHE STRING "" FORCE) 
SET(CMAKE_DEBUG_POSTFIX _d)

include_directories(
			${CMAKE_CURRENT_SOURCE_DIR}
			)

# powerCentrality
set( powerCentrality_h ${powerCentrality_h}
			powerCentralityNode.h
			powerCentralityNodePlugin.h
			)

set( powerCentrality_moc ${powerCentrality_moc}	
			powerCentralityNode.h
			powerCentralityNodePlugin.h
			)

set( powerCentrality_src ${powerCentrality_src}	
			powerCentralityNode.cpp
			powerCentralityNodePlugin.cpp
			)

set( powerCentrality_res ${powerCentrality_res}	
			powerCentrality.xml
			)

# Create moc files		   
qt4_wrap_cpp(powerCentrality_cxx ${powerCentrality_moc})

# Create source groups
source_group("Moc Files" FILES ${powerCentrality_cxx})
source_group("Header Files" FILES ${powerCentrality_h})
source_group("Resources" FILES ${powerCentrality_res})

# Add header files to sources to make headers visible in Visual Studio
IF (WIN32)
  set(powerCentrality_src ${powerCentrality_src} ${powerCentrality_h} ${powerCentrality_res})
ENDIF(WIN32)

# Create static library
add_library(powerCentrality SHARED ${powerCentrality_src} ${powerCentrality_cxx})

# Add library dependencies
target_link_libraries(powerCentrality
			optimized QtCore4 debug QtCored4
			optimized QtGui4 debug QtGuid4
			optimized QtXml4 debug QtXmld4
			optimized OgreMain debug OgreMain_d
			optimized frappercore debug frappercore_d
			)

# Install files
install( FILES ${powerCentrality_res} DESTINATION ${NODE_PLUGIN_INSTALL} )
install( TARGETS powerCentrality RUNTIME DESTINATION ${NODE_PLUGIN_INSTALL} )
