######################################################################
# M2D from LevelFour
######################################################################

include(../Components.pri)

TEMPLATE        = lib
CONFIG         += dll 
LIBS           += $${GCF_LIBS}
DEPENDPATH     += . interfaces source
INCLUDEPATH    += . interfaces source 

MOC_DIR         = $${PROJECT_BASE}/tmp/M2D
OBJECTS_DIR     = $${PROJECT_BASE}/tmp/M2D
UI_HEADERS_DIR  = $${PROJECT_BASE}/tmp/M2D
RCC_DIR         = $${PROJECT_BASE}/tmp/M2D
UI_DIR          = $${PROJECT_BASE}/tmp/M2D

HEADERS         = source/M2DComponent.h
SOURCES         = source/M2DComponent.cpp
