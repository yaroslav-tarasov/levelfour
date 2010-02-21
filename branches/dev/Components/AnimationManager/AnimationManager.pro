######################################################################
# AnimationManager from L4
######################################################################

include(../Components.pri)

TEMPLATE        = lib
CONFIG         += dll 
LIBS           += $${GCF_LIBS}
DEPENDPATH     += . interfaces source
INCLUDEPATH    += . interfaces source 

MOC_DIR         = $${GCF_TEMP_DIR}/AnimationManager
OBJECTS_DIR     = $${GCF_TEMP_DIR}/AnimationManager
UI_HEADERS_DIR  = $${GCF_TEMP_DIR}/AnimationManager
RCC_DIR         = $${GCF_TEMP_DIR}/AnimationManager
UI_DIR          = $${GCF_TEMP_DIR}/AnimationManager

HEADERS         = source/AnimationManagerComponent.h \
			source/GrabberWidget.h \
			source/TimelineGraphicsItem.h \
			source/TimelineWidget.h \
			

SOURCES         = source/AnimationManagerComponent.cpp \
			source/GrabberWidget.cpp \
			source/TimelineGraphicsItem.cpp \
			source/TimelineWidget.cpp \
			

RESOURCES = AnimationManager.qrc
