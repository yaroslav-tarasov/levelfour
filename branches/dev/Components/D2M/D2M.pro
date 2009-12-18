######################################################################
# D2M from LevelFour
######################################################################

include(../Components.pri)

TEMPLATE        = lib
CONFIG         += dll 
LIBS           += $${GCF_LIBS}
DEPENDPATH     += . interfaces source
INCLUDEPATH    += . interfaces source 

MOC_DIR         = $${GCF_TEMP_DIR}/D2M
OBJECTS_DIR     = $${GCF_TEMP_DIR}/D2M
UI_HEADERS_DIR  = $${GCF_TEMP_DIR}/D2M
RCC_DIR         = $${GCF_TEMP_DIR}/D2M
UI_DIR          = $${GCF_TEMP_DIR}/D2M

HEADERS         = source/D2MComponent.h
SOURCES         = source/D2MComponent.cpp
