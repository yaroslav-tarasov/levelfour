######################################################################
# VTK Designer 2 - VisSystemNodeClassExplorer component
# 
# Standard node class explorer component for all drag-and-droppable
# nodes in VTK Designer
######################################################################

include(../Components.pri)

MOC_DIR         = $${PROJECT_BASE}/tmp/VisSystemNodeClassExplorer
OBJECTS_DIR     = $${PROJECT_BASE}/tmp/VisSystemNodeClassExplorer
UI_HEADERS_DIR  = $${PROJECT_BASE}/tmp/VisSystemNodeClassExplorer
RCC_DIR         = $${PROJECT_BASE}/tmp/VisSystemNodeClassExplorer
UI_DIR          = $${PROJECT_BASE}/tmp/VisSystemNodeClassExplorer

# Input
HEADERS += Source/CVisSystemNodeClassExplorer.h \
           Source/CVisSystemNodeClassExplorerComponent.h \
           Source/CVisSystemNodeClassExplorerTreeWidget.h
           
SOURCES += Source/CVisSystemNodeClassExplorer.cpp \
           Source/CVisSystemNodeClassExplorerComponent.cpp \
           Source/CVisSystemNodeClassExplorerTreeWidget.cpp

# VNFComponentGen has generated an empty resource file. If you add
# new objects to the resource file, then you can uncomment the line
# below
RESOURCES += VisSystemNodeClassExplorer.qrc
