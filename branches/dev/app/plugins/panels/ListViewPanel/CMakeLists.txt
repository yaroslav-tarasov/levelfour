project(listviewpanel)
set(CMAKE_CONFIGURATION_TYPES Debug Release CACHE STRING "" FORCE) 
SET(CMAKE_DEBUG_POSTFIX _d)

include_directories(
			${CMAKE_CURRENT_SOURCE_DIR}
			)

# listviewpanel
set( listviewpanel_h ${listviewpanel_h}
			listviewpanel.h
			listviewpanelPlugin.h
			)

set( listviewpanel_moc ${listviewpanel_moc}	
			listviewpanel.h
			listviewpanelPlugin.h
			)
			
set( listviewpanel_src ${listviewpanel_src}	
			listviewpanel.cpp
			listviewpanelPlugin.cpp
			)

set( listviewpanel_res ${listviewpanel_res}	
			listviewpanel.xml
			)

# Create moc files		   
qt4_wrap_cpp(listviewpanel_cxx ${listviewpanel_moc})

# Create source groups
source_group("Moc Files" FILES ${listviewpanel_cxx})
source_group("Generated Files" FILES ${listviewpanel_cxx} ${listviewpanel_ui_h})
source_group("Header Files" FILES ${listviewpanel_h})
source_group("Resources" FILES ${listviewpanel_res})

# Add header files to sources to make headers visible in Visual Studio
IF (WIN32)
  set(listviewpanel_src ${listviewpanel_src} ${listviewpanel_h} ${listviewpanel_res})
ENDIF(WIN32)

# Create static library
add_library(listviewpanel SHARED ${listviewpanel_src} ${listviewpanel_cxx} )

# Add library dependencies
target_link_libraries(listviewpanel
			optimized QtCore4 debug QtCored4
			optimized QtGui4 debug QtGuid4
			optimized QtXml4 debug QtXmld4
			optimized OgreMain debug OgreMain_d
			optimized frappercore debug frappercore_d
			optimized frappergui debug frappergui_d
			)

# Install files
install( FILES ${listviewpanel_res} DESTINATION ${PANEL_PLUGIN_INSTALL} )
install( TARGETS listviewpanel RUNTIME DESTINATION ${PANEL_PLUGIN_INSTALL} )
