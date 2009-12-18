######################################################################
# M2R from LevelFour
######################################################################

include(../Components.pri)

TEMPLATE        = lib
CONFIG         += dll 
LIBS           += $${GCF_LIBS}
DEPENDPATH     += . interfaces source
INCLUDEPATH    += . interfaces source 

MOC_DIR         = $${GCF_TEMP_DIR}/M2R
OBJECTS_DIR     = $${GCF_TEMP_DIR}/M2R
UI_HEADERS_DIR  = $${GCF_TEMP_DIR}/M2R
RCC_DIR         = $${GCF_TEMP_DIR}/M2R
UI_DIR          = $${GCF_TEMP_DIR}/M2R

HEADERS         = source/M2RComponent.h
SOURCES         = source/M2RComponent.cpp
