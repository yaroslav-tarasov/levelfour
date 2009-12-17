######################################################################
# TreeView from LevelFour
######################################################################

include(../Components.pri)

TEMPLATE        = lib
CONFIG         += dll 
LIBS           += $${GCF_LIBS}
DEPENDPATH     += . interfaces source
INCLUDEPATH    += . interfaces source 

MOC_DIR         = $${PROJECT_BASE}/tmp/TreeView
OBJECTS_DIR     = $${PROJECT_BASE}/tmp/TreeView
UI_HEADERS_DIR  = $${PROJECT_BASE}/tmp/TreeView
RCC_DIR         = $${PROJECT_BASE}/tmp/TreeView
UI_DIR          = $${PROJECT_BASE}/tmp/TreeView

HEADERS         = source/TreeViewComponent.h
SOURCES         = source/TreeViewComponent.cpp
