project(Corona)
set(CMAKE_CONFIGURATION_TYPES Debug Release CACHE STRING "" FORCE) 
SET(CMAKE_DEBUG_POSTFIX _d)

include_directories(
			${CMAKE_CURRENT_SOURCE_DIR}
			)

# Corona
set( Corona_h ${Corona_h}		
			CoronaNode.h
			CoronaNodePlugin.h
			)

set( Corona_moc ${Corona_moc}	
			CoronaNode.h
			CoronaNodePlugin.h
			)

set( Corona_src ${Corona_src}	
			CoronaNode.cpp
			CoronaNodePlugin.cpp
			)

set( Corona_res ${Corona_res}	
			Corona.xml
			)

# Create moc files		   
qt4_wrap_cpp(Corona_cxx ${Corona_moc})

# Create source groups
source_group("Moc Files" FILES ${Corona_cxx})
source_group("Header Files" FILES ${Corona_h})
source_group("Resources" FILES ${Corona_res})

# Add header files to sources to make headers visible in Visual Studio
IF (WIN32)
  set(Corona_src ${Corona_src} ${Corona_h} ${Corona_res})
ENDIF(WIN32)

# Create static library
add_library(Corona SHARED ${Corona_src} ${Corona_cxx})

# Add library dependencies
target_link_libraries(Corona
			optimized QtCore4 debug QtCored4
			optimized QtGui4 debug QtGuid4
			optimized QtXml4 debug QtXmld4
			optimized OgreMain debug OgreMain_d
			optimized frappercore debug frappercore_d
			)

# Install files
install( FILES ${Corona_res} DESTINATION ${NODE_PLUGIN_INSTALL} )
install( TARGETS Corona RUNTIME DESTINATION ${NODE_PLUGIN_INSTALL} )
