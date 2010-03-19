project(wiitracker)
set(CMAKE_CONFIGURATION_TYPES Debug Release CACHE STRING "" FORCE) 
SET(CMAKE_DEBUG_POSTFIX _d)

include_directories(
			${CMAKE_CURRENT_SOURCE_DIR}
			${CMAKE_SOURCE_DIR}/thirdparty_libraries/wiiuse/include	
		    )

link_directories(
			${CMAKE_SOURCE_DIR}/thirdparty_libraries/wiiuse/lib
			)

# wiitracker
set( wiitracker_h ${wiitracker_h}		
			WiiTrackerNode.h
			WiiTrackerNodePlugin.h
			)

set( wiitracker_moc ${wiitracker_moc}	
			WiiTrackerNode.h
			WiiTrackerNodePlugin.h
			)

set( wiitracker_src ${wiitracker_src}	
			WiiTrackerNode.cpp
			WiiTrackerNodePlugin.cpp
			)

set( wiitracker_res ${wiitracker_res}	
			wiitracker.xml
			)

# Create moc files		   
qt4_wrap_cpp(wiitracker_cxx ${wiitracker_moc})

# Create source groups
source_group("Moc Files" FILES ${wiitracker_cxx})
source_group("Header Files" FILES ${wiitracker_h})
source_group("Resources" FILES ${wiitracker_res})

# Add header files to sources to make headers visible in Visual Studio
IF (WIN32)
  set(wiitracker_src ${wiitracker_src} ${wiitracker_h} ${wiitracker_res})
ENDIF(WIN32)

# Create static library
add_library(wiitracker SHARED ${wiitracker_src} ${wiitracker_cxx})

# Add library dependencies
target_link_libraries(wiitracker
			optimized QtCore4 debug QtCored4
			optimized QtGui4 debug QtGuid4
			optimized QtXml4 debug QtXmld4
			optimized OgreMain debug OgreMain_d
			optimized wiiuse debug wiiuse
			optimized frappercore debug frappercore_d
			)

# Install files
install( FILES ${wiitracker_res} DESTINATION ${NODE_PLUGIN_INSTALL} )
install( TARGETS wiitracker RUNTIME DESTINATION ${NODE_PLUGIN_INSTALL} )