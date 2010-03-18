######################################################################
# Generic Component Framework Library from VCreate Logic
# 
# Provides the QtHelp module in a prepackaged component.
######################################################################

include(../Components.pri)

TEMPLATE        = lib
CONFIG         += dll help
LIBS           += $$GCF_LIBS
DEPENDPATH     += . interfaces source
INCLUDEPATH    += . interfaces source 

MOC_DIR         = $${PROJECT_BASE}/tmp/HelpSystem
OBJECTS_DIR     = $${PROJECT_BASE}/tmp/HelpSystem
UI_HEADERS_DIR  = $${PROJECT_BASE}/tmp/HelpSystem
RCC_DIR         = $${PROJECT_BASE}/tmp/HelpSystem
UI_DIR          = $${PROJECT_BASE}/tmp/HelpSystem

HEADERS          = source/HelpSystemComponent.h \
                   source/HelpView.h \
                   source/TopicChooserDialog.h \
                   source/HelpSystemConfigurationPage.h
                   
SOURCES          = source/HelpSystemComponent.cpp \
                   source/HelpView.cpp \
                   source/TopicChooserDialog.cpp \
                   source/HelpSystemConfigurationPage.cpp

FORMS            = source/TopicChooserDialog.ui \
                   source/HelpSystemConfigurationPage.ui

# QT              += webkit

RESOURCES       = HelpSystem.qrc

