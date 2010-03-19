project(testpanel)
set(CMAKE_CONFIGURATION_TYPES Debug Release CACHE STRING "" FORCE) 
SET(CMAKE_DEBUG_POSTFIX _d)

include_directories(
			${CMAKE_CURRENT_SOURCE_DIR}
			)

# testpanel
set( testpanel_h ${testpanel_h}
			testpanel.h
			testpanelPlugin.h
			)

set( testpanel_moc ${testpanel_moc}	
			testpanel.h
			testpanelPlugin.h
			)
			
set( testpanel_src ${testpanel_src}	
			testpanel.cpp
			testpanelPlugin.cpp
			)

set( testpanel_res ${testpanel_res}	
			testpanel.xml
			)

# Create moc files		   
qt4_wrap_cpp(testpanel_cxx ${testpanel_moc})

# Create source groups
source_group("Moc Files" FILES ${testpanel_cxx})
source_group("Generated Files" FILES ${testpanel_cxx} ${testpanel_ui_h})
source_group("Header Files" FILES ${testpanel_h})
source_group("Resources" FILES ${testpanel_res})

# Add header files to sources to make headers visible in Visual Studio
IF (WIN32)
  set(testpanel_src ${testpanel_src} ${testpanel_h} ${testpanel_res})
ENDIF(WIN32)

# Create static library
add_library(testpanel SHARED ${testpanel_src} ${testpanel_cxx} )

# Add library dependencies
target_link_libraries(testpanel
			optimized QtCore4 debug QtCored4
			optimized QtGui4 debug QtGuid4
			optimized QtXml4 debug QtXmld4
			optimized OgreMain debug OgreMain_d
			optimized frappercore debug frappercore_d
			optimized frappergui debug frappergui_d
			)

# Install files
install( FILES ${testpanel_res} DESTINATION ${PANEL_PLUGIN_INSTALL} )
install( TARGETS testpanel RUNTIME DESTINATION ${PANEL_PLUGIN_INSTALL} )
