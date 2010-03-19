project(randomblink)
set(CMAKE_CONFIGURATION_TYPES Debug Release CACHE STRING "" FORCE) 
SET(CMAKE_DEBUG_POSTFIX _d)

include_directories(
			${CMAKE_CURRENT_SOURCE_DIR}
			)

# randomblink
set( randomblink_h ${randomblink_h}		
			RandomblinkNode.h
			RandomblinkNodePlugin.h
			)

set( randomblink_moc ${randomblink_moc}	
			RandomblinkNode.h
			RandomblinkNodePlugin.h
			)

set( randomblink_src ${randomblink_src}	
			RandomblinkNode.cpp
			RandomblinkNodePlugin.cpp
			)

set( randomblink_res ${randomblink_res}	
			randomblink.xml
			)

# Create moc files		   
qt4_wrap_cpp(randomblink_cxx ${randomblink_moc})

# Create source groups
source_group("Moc Files" FILES ${randomblink_cxx})
source_group("Header Files" FILES ${randomblink_h})
source_group("Resources" FILES ${randomblink_res})

# Add header files to sources to make headers visible in Visual Studio
IF (WIN32)
  set(randomblink_src ${randomblink_src} ${randomblink_h} ${randomblink_res})
ENDIF(WIN32)

# Create static library
add_library(randomblink SHARED ${randomblink_src} ${randomblink_cxx})

# Add library dependencies
target_link_libraries(randomblink
			optimized QtCore4 debug QtCored4
			optimized QtGui4 debug QtGuid4
			optimized QtXml4 debug QtXmld4
			optimized OgreMain debug OgreMain_d
			optimized frappercore debug frappercore_d
			)

# Install files
install( FILES ${randomblink_res} DESTINATION ${NODE_PLUGIN_INSTALL} )
install( TARGETS randomblink RUNTIME DESTINATION ${NODE_PLUGIN_INSTALL} )