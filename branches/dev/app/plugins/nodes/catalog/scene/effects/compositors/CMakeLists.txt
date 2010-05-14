project(NightVision)
set(CMAKE_CONFIGURATION_TYPES Debug Release CACHE STRING "" FORCE) 
SET(CMAKE_DEBUG_POSTFIX _d)

include_directories(
			${CMAKE_CURRENT_SOURCE_DIR}
			)

# NightVision
set( NightVision_h ${NightVision_h}		
			NightVisionNode.h
			NightVisionNodePlugin.h
			)

set( NightVision_moc ${NightVision_moc}	
			NightVisionNode.h
			NightVisionNodePlugin.h
			)

set( NightVision_src ${NightVision_src}	
			NightVisionNode.cpp
			NightVisionNodePlugin.cpp
			)

set( NightVision_res ${NightVision_res}	
			NightVision.xml
			)

# Create moc files		   
qt4_wrap_cpp(NightVision_cxx ${NightVision_moc})

# Create source groups
source_group("Moc Files" FILES ${NightVision_cxx})
source_group("Header Files" FILES ${NightVision_h})
source_group("Resources" FILES ${NightVision_res})

# Add header files to sources to make headers visible in Visual Studio
IF (WIN32)
  set(NightVision_src ${NightVision_src} ${NightVision_h} ${NightVision_res})
ENDIF(WIN32)

# Create static library
add_library(NightVision SHARED ${NightVision_src} ${NightVision_cxx})

# Add library dependencies
target_link_libraries(NightVision
			optimized QtCore4 debug QtCored4
			optimized QtGui4 debug QtGuid4
			optimized QtXml4 debug QtXmld4
			optimized OgreMain debug OgreMain_d
			optimized frappercore debug frappercore_d
			)

# Install files
install( FILES ${NightVision_res} DESTINATION ${NODE_PLUGIN_INSTALL} )
install( TARGETS NightVision RUNTIME DESTINATION ${NODE_PLUGIN_INSTALL} )
