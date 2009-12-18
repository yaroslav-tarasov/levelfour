######################################################################
# SceneExporter from My Organization
######################################################################

include(../Components.pri)
include(../../VTK.pri)

TEMPLATE        = lib
CONFIG         += dll 
LIBS           += $${GCF_LIBS}
DEPENDPATH     += . interfaces source
INCLUDEPATH    += . interfaces source 

MOC_DIR         = $${PROJECT_BASE}/tmp/SceneExporter
OBJECTS_DIR     = $${PROJECT_BASE}/tmp/SceneExporter
UI_HEADERS_DIR  = $${PROJECT_BASE}/tmp/SceneExporter
RCC_DIR         = $${PROJECT_BASE}/tmp/SceneExporter
UI_DIR          = $${PROJECT_BASE}/tmp/SceneExporter

HEADERS         += source/SceneExporterComponent.h
SOURCES         += source/SceneExporterComponent.cpp
