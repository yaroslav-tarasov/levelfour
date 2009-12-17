######################################################################
# M2M from LevelFour
######################################################################

include(../Components.pri)

TEMPLATE        = lib
CONFIG         += dll 
LIBS           += $${GCF_LIBS}
DEPENDPATH     += . interfaces source
INCLUDEPATH    += . interfaces source 

MOC_DIR         = $${PROJECT_BASE}/tmp/M2M
OBJECTS_DIR     = $${PROJECT_BASE}/tmp/M2M
UI_HEADERS_DIR  = $${PROJECT_BASE}/tmp/M2M
RCC_DIR         = $${PROJECT_BASE}/tmp/M2M
UI_DIR          = $${PROJECT_BASE}/tmp/M2M

HEADERS         = source/M2MComponent.h
SOURCES         = source/M2MComponent.cpp
