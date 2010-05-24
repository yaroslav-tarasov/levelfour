project(BleachBypass)
set(CMAKE_CONFIGURATION_TYPES Debug Release CACHE STRING "" FORCE) 
SET(CMAKE_DEBUG_POSTFIX _d)

include_directories(
			${CMAKE_CURRENT_SOURCE_DIR}
			)

# BleachBypass
set( BleachBypass_h ${BleachBypass_h}		
			BleachBypassNode.h
			BleachBypassNodePlugin.h
			)

set( BleachBypass_moc ${BleachBypass_moc}	
			BleachBypassNode.h
			BleachBypassNodePlugin.h
			)

set( BleachBypass_src ${BleachBypass_src}	
			BleachBypassNode.cpp
			BleachBypassNodePlugin.cpp
			)

set( BleachBypass_res ${BleachBypass_res}	
			BleachBypass.xml
			)

# Create moc files		   
qt4_wrap_cpp(BleachBypass_cxx ${BleachBypass_moc})

# Create source groups
source_group("Moc Files" FILES ${BleachBypass_cxx})
source_group("Header Files" FILES ${BleachBypass_h})
source_group("Resources" FILES ${BleachBypass_res})

# Add header files to sources to make headers visible in Visual Studio
IF (WIN32)
  set(BleachBypass_src ${BleachBypass_src} ${BleachBypass_h} ${BleachBypass_res})
ENDIF(WIN32)

# Create static library
add_library(BleachBypass SHARED ${BleachBypass_src} ${BleachBypass_cxx})

# Add library dependencies
target_link_libraries(BleachBypass
			optimized QtCore4 debug QtCored4
			optimized QtGui4 debug QtGuid4
			optimized QtXml4 debug QtXmld4
			optimized OgreMain debug OgreMain_d
			optimized frappercore debug frappercore_d
			)

# Install files
install( FILES ${BleachBypass_res} DESTINATION ${NODE_PLUGIN_INSTALL} )
install( TARGETS BleachBypass RUNTIME DESTINATION ${NODE_PLUGIN_INSTALL} )
