project(tableviewpanel)
set(CMAKE_CONFIGURATION_TYPES Debug Release CACHE STRING "" FORCE) 
SET(CMAKE_DEBUG_POSTFIX _d)

include_directories(
			${CMAKE_CURRENT_SOURCE_DIR}
			)

# tableviewpanel
set( tableviewpanel_h ${tableviewpanel_h}
			tableviewpanel.h
			tableviewpanelPlugin.h
			)

set( tableviewpanel_moc ${tableviewpanel_moc}	
			tableviewpanel.h
			tableviewpanelPlugin.h
			)

set( tableviewpanel_ui ${tableviewpanel_ui}
			TableViewPanel.ui
			)
						
set( tableviewpanel_src ${tableviewpanel_src}	
			tableviewpanel.cpp
			tableviewpanelPlugin.cpp
			)

set( tableviewpanel_res ${tableviewpanel_res}	
			tableviewpanel.xml
			)

# Create moc files		   
qt4_wrap_cpp(tableviewpanel_cxx ${tableviewpanel_moc})
qt4_wrap_ui(tableviewpanel_ui_h ${tableviewpanel_ui})


# Create source groups
source_group("Ui Files" FILES ${tableviewpanel_ui})
source_group("Moc Files" FILES ${tableviewpanel_cxx})
source_group("Generated Files" FILES ${tableviewpanel_cxx} ${tableviewpanel_ui_h})
source_group("Header Files" FILES ${tableviewpanel_h})
source_group("Resources" FILES ${tableviewpanel_res})

# Add header files to sources to make headers visible in Visual Studio
IF (WIN32)
  set(tableviewpanel_src ${tableviewpanel_src} ${tableviewpanel_h} ${tableviewpanel_res})
ENDIF(WIN32)

# Create static library
add_library(tableviewpanel SHARED ${tableviewpanel_src} ${tableviewpanel_cxx} ${tableviewpanel_ui_h} )

# Add library dependencies
target_link_libraries(tableviewpanel
			optimized QtCore4 debug QtCored4
			optimized QtGui4 debug QtGuid4
			optimized QtXml4 debug QtXmld4
			optimized OgreMain debug OgreMain_d
			optimized frappercore debug frappercore_d
			optimized vtkInfovis debug vtkInfovis
			optimized QVTK debug QVTK
			optimized frappergui debug frappergui_d
			)

# Install files
install( FILES ${tableviewpanel_res} DESTINATION ${PANEL_PLUGIN_INSTALL} )
install( TARGETS tableviewpanel RUNTIME DESTINATION ${PANEL_PLUGIN_INSTALL} )
