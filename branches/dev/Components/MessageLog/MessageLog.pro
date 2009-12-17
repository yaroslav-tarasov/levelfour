######################################################################
# Generic Component Framework Library from VCreate Logic
# 
# Provides a project manager component. Most useful in applications
# that save/load data from XML files. Different aspects of the 
# project will be stored under different XML elements
######################################################################

include(../Components.pri)

TEMPLATE        = lib
CONFIG         += dll 
LIBS           += $$GCF_LIBS
DEPENDPATH     += . interfaces source
INCLUDEPATH    += . interfaces source 

MOC_DIR         = $${PROJECT_BASE}/tmp/MessageLog
OBJECTS_DIR     = $${PROJECT_BASE}/tmp/MessageLog
UI_HEADERS_DIR  = $${PROJECT_BASE}/tmp/MessageLog
RCC_DIR         = $${PROJECT_BASE}/tmp/MessageLog
UI_DIR          = $${PROJECT_BASE}/tmp/MessageLog

HEADERS         = source/MessageLogComponent.h
SOURCES         = source/MessageLogComponent.cpp
RESOURCES       = MessageLog.qrc