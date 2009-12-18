######################################################################
# PropertyInspector from LevelFour
######################################################################

include(../Components.pri)

TEMPLATE        = lib
CONFIG         += dll 
LIBS           += $${GCF_LIBS}
DEPENDPATH     += . interfaces source
INCLUDEPATH    += . interfaces source 

MOC_DIR         = $${GCF_TEMP_DIR}/PropertyInspector
OBJECTS_DIR     = $${GCF_TEMP_DIR}/PropertyInspector
UI_HEADERS_DIR  = $${GCF_TEMP_DIR}/PropertyInspector
RCC_DIR         = $${GCF_TEMP_DIR}/PropertyInspector
UI_DIR          = $${GCF_TEMP_DIR}/PropertyInspector

HEADERS         = source/PropertyInspectorComponent.h
SOURCES         = source/PropertyInspectorComponent.cpp
