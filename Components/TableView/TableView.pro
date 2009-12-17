######################################################################
# TableView from LevelFour
######################################################################

include(../Components.pri)

TEMPLATE        = lib
CONFIG         += dll 
LIBS           += $${GCF_LIBS}
DEPENDPATH     += . interfaces source
INCLUDEPATH    += . interfaces source 

MOC_DIR         = $${PROJECT_BASE}/tmp/TableView
OBJECTS_DIR     = $${PROJECT_BASE}/tmp/TableView
UI_HEADERS_DIR  = $${PROJECT_BASE}/tmp/TableView
RCC_DIR         = $${PROJECT_BASE}/tmp/TableView
UI_DIR          = $${PROJECT_BASE}/tmp/TableView

HEADERS         = source/TableViewComponent.h
SOURCES         = source/TableViewComponent.cpp
