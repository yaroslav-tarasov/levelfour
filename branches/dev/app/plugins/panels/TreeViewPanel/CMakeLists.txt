project(treeviewpanel)
set(CMAKE_CONFIGURATION_TYPES Debug Release CACHE STRING "" FORCE) 
SET(CMAKE_DEBUG_POSTFIX _d)

include_directories(
			${CMAKE_CURRENT_BINARY_DIR}
			${CMAKE_CURRENT_SOURCE_DIR}
			)

# treeviewpanel
set( treeviewpanel_h ${treeviewpanel_h}
			treeviewpanel.h
			treeviewpanelPlugin.h
			)

set( treeviewpanel_moc ${treeviewpanel_moc}	
			treeviewpanel.h
			treeviewpanelPlugin.h
			)

set( treeviewpanel_ui ${treeviewpanel_ui}
			TreeViewPanel.ui
			)
			
set( treeviewpanel_src ${treeviewpanel_src}	
			treeviewpanel.cpp
			treeviewpanelPlugin.cpp
			)

set( treeviewpanel_res ${treeviewpanel_res}	
			treeviewpanel.xml
			)

# Create moc files		   
qt4_wrap_cpp(treeviewpanel_cxx ${treeviewpanel_moc})
qt4_wrap_ui(treeviewpanel_ui_h ${treeviewpanel_ui})

# Create source groups
source_group("Ui Files" FILES ${treeviewpanel_ui})
source_group("Moc Files" FILES ${treeviewpanel_cxx})
source_group("Generated Files" FILES ${treeviewpanel_cxx} ${treeviewpanel_ui_h})
source_group("Header Files" FILES ${treeviewpanel_h})
source_group("Resources" FILES ${treeviewpanel_res})

# Add header files to sources to make headers visible in Visual Studio
IF (WIN32)
  set(treeviewpanel_src ${treeviewpanel_src} ${treeviewpanel_h} ${treeviewpanel_res})
ENDIF(WIN32)

# Create static library
add_library(treeviewpanel SHARED ${treeviewpanel_src} ${treeviewpanel_cxx} ${treeviewpanel_ui_h} )

# Add library dependencies
target_link_libraries(treeviewpanel
			optimized QtCore4 debug QtCored4
			optimized QtGui4 debug QtGuid4
			optimized QtXml4 debug QtXmld4
			optimized OgreMain debug OgreMain_d
			optimized frappercore debug frappercore_d
			optimized frappergui debug frappergui_d
			)

# Install files
install( FILES ${treeviewpanel_res} DESTINATION ${PANEL_PLUGIN_INSTALL} )
install( TARGETS treeviewpanel RUNTIME DESTINATION ${PANEL_PLUGIN_INSTALL} )
