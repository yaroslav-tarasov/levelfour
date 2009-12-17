######################################################################
# TemplateManager from VCreate Logic Pvt Ltd
######################################################################

include(../Components.pri)

TEMPLATE        = lib
CONFIG         += dll 
LIBS           += $${GCF_LIBS}
DEPENDPATH     += . interfaces source
INCLUDEPATH    += . interfaces source 

MOC_DIR         = $${PROJECT_BASE}/tmp/TemplateManager
OBJECTS_DIR     = $${PROJECT_BASE}/tmp/TemplateManager
UI_HEADERS_DIR  = $${PROJECT_BASE}/tmp/TemplateManager
RCC_DIR         = $${PROJECT_BASE}/tmp/TemplateManager
UI_DIR          = $${PROJECT_BASE}/tmp/TemplateManager

HEADERS         += source/TemplateManagerComponent.h
SOURCES         += source/TemplateManagerComponent.cpp
