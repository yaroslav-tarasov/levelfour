######################################################################
# Generic Component Framework Library from VCreate Logic
# 
# Provides a multi-context, list based clipboard 
######################################################################

include(../Components.pri)

TEMPLATE        = lib
CONFIG         += dll help
LIBS           += $$GCF_LIBS
DEPENDPATH     += . interfaces source ../Utils/interfaces
INCLUDEPATH    += . interfaces source ../Utils/interfaces

MOC_DIR         = $${PROJECT_BASE}/tmp/Clipboard
OBJECTS_DIR     = $${PROJECT_BASE}/tmp/Clipboard
UI_HEADERS_DIR  = $${PROJECT_BASE}/tmp/Clipboard
RCC_DIR         = $${PROJECT_BASE}/tmp/Clipboard
UI_DIR          = $$${PROJECT_BASE}/tmp/Clipboard

HEADERS         = source/ClipboardComponent.h \
                  source/ClipboardWidget.h \
                  source/ClipboardModel.h 
                   
SOURCES         = source/ClipboardComponent.cpp \
                  source/ClipboardWidget.cpp \
                  source/ClipboardModel.cpp 

FORMS           = source/ClipboardWidget.ui

# QT              += webkit


