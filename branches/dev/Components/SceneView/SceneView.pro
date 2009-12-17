######################################################################
# SceneView from LevelFour
######################################################################

include(../Components.pri)

TEMPLATE        = lib
CONFIG         += dll 
LIBS           += $${GCF_LIBS}
DEPENDPATH     += . interfaces source
INCLUDEPATH    += . interfaces source 

MOC_DIR         = $${PROJECT_BASE}/tmp/SceneView
OBJECTS_DIR     = $${PROJECT_BASE}/tmp/SceneView
UI_HEADERS_DIR  = $${PROJECT_BASE}/tmp/SceneView
RCC_DIR         = $${PROJECT_BASE}/tmp/SceneView
UI_DIR          = $${PROJECT_BASE}/tmp/SceneView

HEADERS         = source/SceneViewComponent.h
SOURCES         = source/SceneViewComponent.cpp
