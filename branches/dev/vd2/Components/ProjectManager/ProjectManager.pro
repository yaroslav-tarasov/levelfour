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
INCLUDEPATH    += . interfaces source ../Utils/interfaces

MOC_DIR         = $${PROJECT_BASE}/tmp/ProjectManager
OBJECTS_DIR     = $${PROJECT_BASE}/tmp/ProjectManager
UI_HEADERS_DIR  = $${PROJECT_BASE}/tmp/ProjectManager
RCC_DIR         = $${PROJECT_BASE}/tmp/ProjectManager
UI_DIR          = $${PROJECT_BASE}/tmp/ProjectManager

# Input
HEADERS  =  source/Project.h \
		source/ProjectManagerComponent.h

SOURCES  =  source/Project.cpp \
		source/ProjectManagerComponent.cpp

RESOURCES = ProjectManager.qrc



