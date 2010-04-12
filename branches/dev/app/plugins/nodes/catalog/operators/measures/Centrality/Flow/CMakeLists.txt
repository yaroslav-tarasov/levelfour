project(flowCentrality)
set(CMAKE_CONFIGURATION_TYPES Debug Release CACHE STRING "" FORCE) 
SET(CMAKE_DEBUG_POSTFIX _d)

include_directories(
			${CMAKE_CURRENT_SOURCE_DIR}
			)

# flowCentrality
set( flowCentrality_h ${flowCentrality_h}
			flowCentralityNode.h
			flowCentralityNodePlugin.h
			)

set( flowCentrality_moc ${flowCentrality_moc}	
			flowCentralityNode.h
			flowCentralityNodePlugin.h
			)

set( flowCentrality_src ${flowCentrality_src}	
			flowCentralityNode.cpp
			flowCentralityNodePlugin.cpp
			)

set( flowCentrality_res ${flowCentrality_res}	
			flowCentrality.xml
			)

# Create moc files		   
qt4_wrap_cpp(flowCentrality_cxx ${flowCentrality_moc})

# Create source groups
source_group("Moc Files" FILES ${flowCentrality_cxx})
source_group("Header Files" FILES ${flowCentrality_h})
source_group("Resources" FILES ${flowCentrality_res})

# Add header files to sources to make headers visible in Visual Studio
IF (WIN32)
  set(flowCentrality_src ${flowCentrality_src} ${flowCentrality_h} ${flowCentrality_res})
ENDIF(WIN32)

# Create static library
add_library(flowCentrality SHARED ${flowCentrality_src} ${flowCentrality_cxx})

# Add library dependencies
target_link_libraries(flowCentrality
			optimized QtCore4 debug QtCored4
			optimized QtGui4 debug QtGuid4
			optimized QtXml4 debug QtXmld4
			optimized OgreMain debug OgreMain_d
			optimized frappercore debug frappercore_d
			)

# Install files
install( FILES ${flowCentrality_res} DESTINATION ${NODE_PLUGIN_INSTALL} )
install( TARGETS flowCentrality RUNTIME DESTINATION ${NODE_PLUGIN_INSTALL} )
