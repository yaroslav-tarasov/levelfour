project(TableViewPanel)
set(CMAKE_CONFIGURATION_TYPES Debug Release CACHE STRING "" FORCE) 
SET(CMAKE_DEBUG_POSTFIX _d)

include_directories(
			${CMAKE_CURRENT_SOURCE_DIR}
			)

# TableViewPanel
set( TableViewPanel_h ${TableViewPanel_h}
			TableViewPanel.h
			TableViewPanelPlugin.h
			)

set( TableViewPanel_moc ${TableViewPanel_moc}	
			TableViewPanel.h
			TableViewPanelPlugin.h
			)

set( TableViewPanel_ui ${TableViewPanel_ui}
			TableViewPanel.ui
			)
						
set( TableViewPanel_src ${TableViewPanel_src}	
			TableViewPanel.cpp
			TableViewPanelPlugin.cpp
			)

set( TableViewPanel_res ${TableViewPanel_res}	
			TableViewPanel.xml
			)

# Create moc files		   
qt4_wrap_cpp(TableViewPanel_cxx ${TableViewPanel_moc})
qt4_wrap_ui(TableViewPanel_ui_h ${TableViewPanel_ui})


# Create source groups
source_group("Ui Files" FILES ${TableViewPanel_ui})
source_group("Moc Files" FILES ${TableViewPanel_cxx})
source_group("Generated Files" FILES ${TableViewPanel_cxx} ${TableViewPanel_ui_h})
source_group("Header Files" FILES ${TableViewPanel_h})
source_group("Resources" FILES ${TableViewPanel_res})

# Add header files to sources to make headers visible in Visual Studio
IF (WIN32)
  set(TableViewPanel_src ${TableViewPanel_src} ${TableViewPanel_h} ${TableViewPanel_res})
ENDIF(WIN32)

# Create static library
add_library(TableViewPanel SHARED ${TableViewPanel_src} ${TableViewPanel_cxx} ${TableViewPanel_ui_h} )

# Add library dependencies
target_link_libraries(TableViewPanel
			optimized QtCore4 debug QtCored4
			optimized QtGui4 debug QtGuid4
			optimized QtXml4 debug QtXmld4
			optimized OgreMain debug OgreMain_d
			optimized frappercore debug frappercore_d
			optimized frappergui debug frappergui_d
			optimized vtkInfovis debug vtkInfovis
			optimized QVTK debug QVTK
			)

# Install files
install( FILES ${TableViewPanel_res} DESTINATION ${PANEL_PLUGIN_INSTALL} )
install( TARGETS TableViewPanel RUNTIME DESTINATION ${PANEL_PLUGIN_INSTALL} )
