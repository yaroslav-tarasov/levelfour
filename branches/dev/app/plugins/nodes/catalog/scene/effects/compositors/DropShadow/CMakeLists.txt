project(DropShadow)
set(CMAKE_CONFIGURATION_TYPES Debug Release CACHE STRING "" FORCE) 
SET(CMAKE_DEBUG_POSTFIX _d)

include_directories(
			${CMAKE_CURRENT_SOURCE_DIR}
			)

# DropShadow
set( DropShadow_h ${DropShadow_h}		
			DropShadowNode.h
			DropShadowNodePlugin.h
			)

set( DropShadow_moc ${DropShadow_moc}	
			DropShadowNode.h
			DropShadowNodePlugin.h
			)

set( DropShadow_src ${DropShadow_src}	
			DropShadowNode.cpp
			DropShadowNodePlugin.cpp
			)

set( DropShadow_res ${DropShadow_res}	
			DropShadow.xml
			)

# Create moc files		   
qt4_wrap_cpp(DropShadow_cxx ${DropShadow_moc})

# Create source groups
source_group("Moc Files" FILES ${DropShadow_cxx})
source_group("Header Files" FILES ${DropShadow_h})
source_group("Resources" FILES ${DropShadow_res})

# Add header files to sources to make headers visible in Visual Studio
IF (WIN32)
  set(DropShadow_src ${DropShadow_src} ${DropShadow_h} ${DropShadow_res})
ENDIF(WIN32)

# Create static library
add_library(DropShadow SHARED ${DropShadow_src} ${DropShadow_cxx})

# Add library dependencies
target_link_libraries(DropShadow
			optimized QtCore4 debug QtCored4
			optimized QtGui4 debug QtGuid4
			optimized QtXml4 debug QtXmld4
			optimized OgreMain debug OgreMain_d
			optimized frappercore debug frappercore_d
			)

# Install files
install( FILES ${DropShadow_res} DESTINATION ${NODE_PLUGIN_INSTALL} )
install( TARGETS DropShadow RUNTIME DESTINATION ${NODE_PLUGIN_INSTALL} )
