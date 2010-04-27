project(frappergui)
set(CMAKE_CONFIGURATION_TYPES Debug Release CACHE STRING "" FORCE) 
SET(CMAKE_DEBUG_POSTFIX _d)

include_directories(
			${CMAKE_CURRENT_BINARY_DIR}
			${CMAKE_CURRENT_SOURCE_DIR}
			)

# frappergui
set( frappergui_h ${frappergui_h}		
		   BaseGraphicsView.h
		   Controller.h
		   CopyHandler.h
		   CurveEditorGraphicsView.h
		   CurveEditorPanel.h
		   DocumentationPanel.h
		   DoubleSlider.h
		   GrabberWidget.h
		   GraphicsView.h
		   HierarchyPanel.h
		   HistoryPanel.h
		   KeyGraphicsItem.h
		   LogPanel.h
		   NetworkGraphicsView.h
		   NetworkPanel.h
		   Panel.h
		   PanelFactory.h
		   PanelFrame.h
		   ParameterPanel.h
		   ParameterTabPage.h
		   PanelTypeInterface.h
		   SegmentGraphicsItem.h
		   TimelineGraphicsItem.h
		   TimelinePanel.h
		   TimelineWidget.h
		   ViewPanel.h
		   ViewportPanel.h
		   ViewportWidget.h
		   WidgetFactory.h
		   WidgetPlugin.h
		   Window.h
		   WindowAction.h
		   WindowItem.h
		   WindowManager.h
		   WindowsDialog.h
		   )

set( frappergui_moc ${frappergui_moc}		
		   BaseGraphicsView.h
		   CopyHandler.h
		   CurveEditorGraphicsView.h
		   CurveEditorPanel.h
		   DocumentationPanel.h
		   DoubleSlider.h
		   GrabberWidget.h
		   GraphicsView.h
		   HierarchyPanel.h
		   HistoryPanel.h
		   LogPanel.h
		   NetworkGraphicsView.h
		   NetworkPanel.h
		   Panel.h
		   PanelFrame.h
		   ParameterPanel.h
		   ParameterTabPage.h
		   PanelTypeInterface.h
		   TimelinePanel.h
		   TimelineWidget.h
		   ViewPanel.h
		   ViewportPanel.h
		   ViewportWidget.h
		   WidgetPlugin.h
		   WidgetFactory.h
		   WidgetTypeInterface.h
		   Window.h
		   WindowAction.h
		   WindowsDialog.h
		   )

set( frappergui_ui ${frappergui_ui}
		   CurveEditorPanel.ui
		   DocumentationPanel.ui	
		   HierarchyPanel.ui
		   HistoryPanel.ui
		   LogPanel.ui
		   NetworkPanel.ui
		   PanelFrame.ui
		   ParameterPanel.ui
		   TimelinePanel.ui
		   ViewportPanel.ui
		   Window.ui
		   WindowsDialog.ui
		   )

set( frappergui_src ${frappergui_src}		
		   BaseGraphicsView.cpp
		   Controller.cpp
		   CurveEditorGraphicsView.cpp
		   CurveEditorPanel.cpp
		   DocumentationPanel.cpp
		   DoubleSlider.cpp
		   GrabberWidget.cpp
		   GraphicsView.cpp
		   HierarchyPanel.cpp
		   HistoryPanel.cpp
		   KeyGraphicsItem.cpp
		   LogPanel.cpp
		   NetworkGraphicsView.cpp
		   NetworkPanel.cpp
		   Panel.cpp
		   PanelFactory.cpp
		   PanelFrame.cpp
		   ParameterPanel.cpp
		   ParameterTabPage.cpp
		   SegmentGraphicsItem.cpp
		   TimelineGraphicsItem.cpp
		   TimelinePanel.cpp
		   TimelineWidget.cpp
		   ViewPanel.cpp
		   ViewportPanel.cpp
		   ViewportWidget.cpp
		   WidgetPlugin.cpp
		   WidgetFactory.cpp
		   Window.cpp
		   WindowAction.cpp
		   WindowItem.cpp
		   WindowManager.cpp
		   WindowsDialog.cpp
		   )
		   
set ( frappergui_qrc ${frappergui_qrc}
		   main.qrc )		   
		   
# Create moc and ui files			   
qt4_wrap_cpp(frappergui_cxx ${frappergui_moc})
qt4_wrap_ui(frappergui_ui_h ${frappergui_ui})
qt4_add_resources(frappergui_rcc ${frappergui_qrc})

# Create source groups
source_group("Ui Files" FILES ${frappergui_ui})
source_group("Resource Files" FILES ${frappergui_qrc})
source_group("Generated Files" FILES ${frappergui_cxx} ${frappergui_ui_h} ${frappergui_rcc})
source_group("Header Files" FILES ${frappergui_h})

# Add header files to sources to make headers visible in Visual Studio
set(frappergui_src ${frappergui_src} ${frappergui_h})

# Create static library
add_library(frappergui SHARED ${frappergui_src} ${frappergui_cxx} ${frappergui_ui_h} ${frappergui_rcc})

# Add library dependencies
target_link_libraries(frappergui
			optimized QtCore4 debug QtCored4
			optimized QtGui4 debug QtGuid4
			optimized QtXml4 debug QtXmld4
			optimized OgreMain debug OgreMain_d
			optimized frappercore debug frappercore_d
			optimized QVTK debug QVTK
			)

# Install files
install( TARGETS frappergui RUNTIME DESTINATION ${CMAKE_INSTALL_PREFIX} )
