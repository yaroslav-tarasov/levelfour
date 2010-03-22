project(PatternExplorerPanel)
set(CMAKE_CONFIGURATION_TYPES Debug Release CACHE STRING "" FORCE) 
SET(CMAKE_DEBUG_POSTFIX _d)

include_directories(
			${CMAKE_CURRENT_SOURCE_DIR}
			)

# patternexplorerpanel
set( patternexplorerpanel_h ${patternexplorerpanel_h}
			patternexplorerpanel.h
			patternexplorerpanelPlugin.h
			)

set( patternexplorerpanel_moc ${patternexplorerpanel_moc}	
			patternexplorerpanel.h
			patternexplorerpanelPlugin.h
			)
			
set( patternexplorerpanel_src ${patternexplorerpanel_src}	
			patternexplorerpanel.cpp
			patternexplorerpanelPlugin.cpp
			)

set( patternexplorerpanel_res ${patternexplorerpanel_res}	
			patternexplorerpanel.xml
			)

# Create moc files		   
qt4_wrap_cpp(patternexplorerpanel_cxx ${patternexplorerpanel_moc})

# Create source groups
source_group("Moc Files" FILES ${patternexplorerpanel_cxx})
source_group("Generated Files" FILES ${patternexplorerpanel_cxx} ${patternexplorerpanel_ui_h})
source_group("Header Files" FILES ${patternexplorerpanel_h})
source_group("Resources" FILES ${patternexplorerpanel_res})

# Add header files to sources to make headers visible in Visual Studio
IF (WIN32)
  set(patternexplorerpanel_src ${patternexplorerpanel_src} ${patternexplorerpanel_h} ${patternexplorerpanel_res})
ENDIF(WIN32)

# Create static library
add_library(patternexplorerpanel SHARED ${patternexplorerpanel_src} ${patternexplorerpanel_cxx} )

# Add library dependencies
target_link_libraries(patternexplorerpanel
			optimized QtCore4 debug QtCored4
			optimized QtGui4 debug QtGuid4
			optimized QtXml4 debug QtXmld4
			optimized OgreMain debug OgreMain_d
			optimized frappercore debug frappercore_d
			optimized frappergui debug frappergui_d
			)

# Install files
install( FILES ${patternexplorerpanel_res} DESTINATION ${PANEL_PLUGIN_INSTALL} )
install( TARGETS patternexplorerpanel RUNTIME DESTINATION ${PANEL_PLUGIN_INSTALL} )
