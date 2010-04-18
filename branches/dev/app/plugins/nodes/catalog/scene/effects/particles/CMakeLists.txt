project(particleSystem)
set(CMAKE_CONFIGURATION_TYPES Debug Release CACHE STRING "" FORCE) 
SET(CMAKE_DEBUG_POSTFIX _d)

include_directories(
			${CMAKE_CURRENT_SOURCE_DIR}
			)

# particleSystem
set( particleSystem_h ${particleSystem_h}
			particleSystemNode.h
			particleSystemNodePlugin.h
			)

set( particleSystem_moc ${particleSystem_moc}	
			particleSystemNode.h
			particleSystemNodePlugin.h
			)

set( particleSystem_src ${particleSystem_src}	
			particleSystemNode.cpp
			particleSystemNodePlugin.cpp
			)

set( particleSystem_res ${particleSystem_res}	
			particleSystem.xml
			)

# Create moc files		   
qt4_wrap_cpp(particleSystem_cxx ${particleSystem_moc})

# Create source groups
source_group("Moc Files" FILES ${particleSystem_cxx})
source_group("Header Files" FILES ${particleSystem_h})
source_group("Resources" FILES ${particleSystem_res})

# Add header files to sources to make headers visible in Visual Studio
IF (WIN32)
  set(particleSystem_src ${particleSystem_src} ${particleSystem_h} ${particleSystem_res})
ENDIF(WIN32)

# Create static library
add_library(particleSystem SHARED ${particleSystem_src} ${particleSystem_cxx})

# Add library dependencies
target_link_libraries(particleSystem
			optimized QtCore4 debug QtCored4
			optimized QtGui4 debug QtGuid4
			optimized QtXml4 debug QtXmld4
			optimized OgreMain debug OgreMain_d
			optimized frappercore debug frappercore_d
			)

# Install files
install( FILES ${particleSystem_res} DESTINATION ${NODE_PLUGIN_INSTALL} )
install( TARGETS particleSystem RUNTIME DESTINATION ${NODE_PLUGIN_INSTALL} )
