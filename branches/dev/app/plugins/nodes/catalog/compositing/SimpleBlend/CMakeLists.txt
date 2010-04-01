project(simpleblend)
set(CMAKE_CONFIGURATION_TYPES Debug Release CACHE STRING "" FORCE) 
SET(CMAKE_DEBUG_POSTFIX _d)

include_directories(
			${CMAKE_CURRENT_SOURCE_DIR}
			)

# simpleblend
set( simpleblend_h ${simpleblend_h}		
			SimpleBlendNode.h
			SimpleBlendNodePlugin.h
			)

set( simpleblend_moc ${simpleblend_moc}	
			SimpleBlendNode.h
			SimpleBlendNodePlugin.h
			)

set( simpleblend_src ${simpleblend_src}	
			SimpleBlendNode.cpp
			SimpleBlendNodePlugin.cpp
			)

set( simpleblend_res ${simpleblend_res}	
			simpleblend.xml
			)

# Create moc files		   
qt4_wrap_cpp(simpleblend_cxx ${simpleblend_moc})

# Create source groups
source_group("Moc Files" FILES ${simpleblend_cxx})
source_group("Header Files" FILES ${simpleblend_h})
source_group("Resources" FILES ${simpleblend_res})

# Add header files to sources to make headers visible in Visual Studio
IF (WIN32)
  set(simpleblend_src ${simpleblend_src} ${simpleblend_h} ${simpleblend_res})
ENDIF(WIN32)

# Create static library
add_library(simpleblend SHARED ${simpleblend_src} ${simpleblend_cxx})

# Add library dependencies
target_link_libraries(simpleblend
			optimized QtCore4 debug QtCored4
			optimized QtGui4 debug QtGuid4
			optimized QtXml4 debug QtXmld4
			optimized OgreMain debug OgreMain_d
			optimized frappercore debug frappercore_d
			)

# Install files
install( FILES ${simpleblend_res} DESTINATION ${NODE_PLUGIN_INSTALL} )
install( TARGETS simpleblend RUNTIME DESTINATION ${NODE_PLUGIN_INSTALL} )