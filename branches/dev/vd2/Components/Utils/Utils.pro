######################################################################
# Generic Component Framework Library from VCreate Logic
# 
# Provides a Math and XML utils implementation
######################################################################

include(../Components.pri)

TEMPLATE        = lib
CONFIG         += dll 
LIBS           += $$GCF_LIBS
DEPENDPATH     += . interfaces source
INCLUDEPATH    += . interfaces source

MOC_DIR         = $${PROJECT_BASE}/tmp/Utils
OBJECTS_DIR     = $${PROJECT_BASE}/tmp/Utils
UI_HEADERS_DIR  = $${PROJECT_BASE}/tmp/Utils
RCC_DIR         = $${PROJECT_BASE}/tmp/Utils
UI_DIR          = $${PROJECT_BASE}/tmp/Utils

# Input
HEADERS  = source/UtilsComponent.h
SOURCES  = source/UtilsComponent.cpp



