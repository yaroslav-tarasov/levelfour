project(TreeViewPanel)
set(CMAKE_CONFIGURATION_TYPES Debug Release CACHE STRING "" FORCE) 
SET(CMAKE_DEBUG_POSTFIX _d)

include_directories(
			${CMAKE_CURRENT_BINARY_DIR}
			${CMAKE_CURRENT_SOURCE_DIR}
			)

# TreeViewPanel
set( TreeViewPanel_h ${TreeViewPanel_h}
			TreeViewPanel.h
			TreeViewPanelPlugin.h
			)

set( TreeViewPanel_moc ${TreeViewPanel_moc}	
			TreeViewPanel.h
			TreeViewPanelPlugin.h
			)

set( TreeViewPanel_ui ${TreeViewPanel_ui}
			TreeViewPanel.ui
			)
			
set( TreeViewPanel_src ${TreeViewPanel_src}	
			TreeViewPanel.cpp
			TreeViewPanelPlugin.cpp
			)

set( TreeViewPanel_res ${TreeViewPanel_res}	
			TreeViewPanel.xml
			)

# Create moc files		   
qt4_wrap_cpp(TreeViewPanel_cxx ${TreeViewPanel_moc})
qt4_wrap_ui(TreeViewPanel_ui_h ${TreeViewPanel_ui})

# Create source groups
source_group("Ui Files" FILES ${TreeViewPanel_ui})
source_group("Moc Files" FILES ${TreeViewPanel_cxx})
source_group("Generated Files" FILES ${TreeViewPanel_cxx} ${TreeViewPanel_ui_h})
source_group("Header Files" FILES ${TreeViewPanel_h})
source_group("Resources" FILES ${TreeViewPanel_res})

# Add header files to sources to make headers visible in Visual Studio
IF (WIN32)
  set(TreeViewPanel_src ${TreeViewPanel_src} ${TreeViewPanel_h} ${TreeViewPanel_res})
ENDIF(WIN32)

# Create static library
add_library(TreeViewPanel SHARED ${TreeViewPanel_src} ${TreeViewPanel_cxx} ${TreeViewPanel_ui_h} )

# Add library dependencies
target_link_libraries(TreeViewPanel
			optimized QtCore4 debug QtCored4
			optimized QtGui4 debug QtGuid4
			optimized QtXml4 debug QtXmld4
			optimized OgreMain debug OgreMain_d
			optimized frappercore debug frappercore_d
			optimized frappergui debug frappergui_d
			)

# Install files
install( FILES ${TreeViewPanel_res} DESTINATION ${PANEL_PLUGIN_INSTALL} )
install( TARGETS TreeViewPanel RUNTIME DESTINATION ${PANEL_PLUGIN_INSTALL} )
