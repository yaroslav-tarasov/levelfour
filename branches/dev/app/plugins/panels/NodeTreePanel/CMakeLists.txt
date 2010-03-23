project(NodeTreePanel)
set(CMAKE_CONFIGURATION_TYPES Debug Release CACHE STRING "" FORCE) 
SET(CMAKE_DEBUG_POSTFIX _d)

include_directories(
			${CMAKE_CURRENT_BINARY_DIR}			
			${CMAKE_CURRENT_SOURCE_DIR}
			)

# nodetreepanel
set( nodetreepanel_h ${nodetreepanel_h}
			nodetreepanel.h
			nodetreepanelPlugin.h
			)

set( nodetreepanel_moc ${nodetreepanel_moc}	
			nodetreepanel.h
			nodetreepanelPlugin.h
			)

set( nodetreepanel_ui ${nodetreepanel_ui}
			NodeTreePanel.ui
			)
				
set( nodetreepanel_src ${nodetreepanel_src}	
			nodetreepanel.cpp
			nodetreepanelPlugin.cpp
			)

set( nodetreepanel_res ${nodetreepanel_res}	
			nodetreepanel.xml
			)

# Create moc files		   
qt4_wrap_cpp(nodetreepanel_cxx ${nodetreepanel_moc})
qt4_wrap_ui(nodetreepanel_ui_h ${nodetreepanel_ui})

# Create source groups
source_group("Ui Files" FILES ${nodetreepanel_ui})
source_group("Moc Files" FILES ${nodetreepanel_cxx})
source_group("Generated Files" FILES ${nodetreepanel_cxx} ${nodetreepanel_ui_h})
source_group("Header Files" FILES ${nodetreepanel_h})
source_group("Resources" FILES ${nodetreepanel_res})

# Add header files to sources to make headers visible in Visual Studio
IF (WIN32)
  set(nodetreepanel_src ${nodetreepanel_src} ${nodetreepanel_h} ${nodetreepanel_res})
ENDIF(WIN32)

# Create static library
add_library(nodetreepanel SHARED ${nodetreepanel_src} ${nodetreepanel_cxx} ${nodetreepanel_ui_h} )

# Add library dependencies
target_link_libraries(nodetreepanel
			optimized QtCore4 debug QtCored4
			optimized QtGui4 debug QtGuid4
			optimized QtXml4 debug QtXmld4
			optimized OgreMain debug OgreMain_d
			optimized frappercore debug frappercore_d
			optimized frappergui debug frappergui_d
			)

# Install files
install( FILES ${nodetreepanel_res} DESTINATION ${PANEL_PLUGIN_INSTALL} )
install( TARGETS nodetreepanel RUNTIME DESTINATION ${PANEL_PLUGIN_INSTALL} )
