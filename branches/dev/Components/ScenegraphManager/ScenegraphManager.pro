######################################################################
# ScenegraphManager from LevelFour
######################################################################

include(../Components.pri)

TEMPLATE        = lib
CONFIG         += dll 
LIBS           += $${GCF_LIBS}
DEPENDPATH     += . interfaces source
INCLUDEPATH    += . interfaces source 

MOC_DIR         = $${PROJECT_BASE}/tmp/ScenegraphManager
OBJECTS_DIR     = $${PROJECT_BASE}/tmp/ScenegraphManager
UI_HEADERS_DIR  = $${PROJECT_BASE}/tmp/ScenegraphManager
RCC_DIR         = $${PROJECT_BASE}/tmp/ScenegraphManager
UI_DIR          = $${PROJECT_BASE}/tmp/ScenegraphManager

HEADERS         = source/ScenegraphManagerComponent.h
SOURCES         = source/ScenegraphManagerComponent.cpp
