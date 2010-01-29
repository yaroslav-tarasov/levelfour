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

HEADERS         = source/SceneViewComponent.h \
				source/vtkActorToOSG.h \
				source/QOSGWidget.h \
				source/QOSGGraphics.h
				
SOURCES         = source/SceneViewComponent.cpp \
				source/vtkActorToOSG.cpp \
				source/QOSGWidget.cpp \
				source/QOSGGraphics.cpp

LIBS			+= -lvtkCommon \
-lvtkFiltering \
-lvtkGraphics \
-lvtkRendering \
-lvtksys \
-losgd \
-losgDBd \
-lQtGuid4 \
-lQtCored4 \
-lQtOpenGLd4 \
-lOpenThreadsd \
-losgUtild \
-losgGAd \
-losgViewerd \
-losgTextd \
-lopengl32 \