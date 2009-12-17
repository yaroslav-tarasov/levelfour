######################################################################
# Generic Component Framework Library from VCreate Logic
# 
# Provides a simple, reusable, script editor component. Using this
# component you can quickly make your applications scriptable. 
# 
######################################################################

include(../Components.pri)

TEMPLATE        = lib
LIBS           += $$GCF_LIBS
DEPENDPATH     += . interfaces source
INCLUDEPATH    += . interfaces source 

MOC_DIR         = $${PROJECT_BASE}/tmp/StdMainWindow
OBJECTS_DIR     = $${PROJECT_BASE}/tmp/StdMainWindow
UI_HEADERS_DIR  = $${PROJECT_BASE}/tmp/StdMainWindow
RCC_DIR         = $${PROJECT_BASE}/tmp/StdMainWindow
UI_DIR          = $${PROJECT_BASE}/tmp/StdMainWindow

DEFINES         += VCL_MAKE_DLL2

# Input
HEADERS  = source/AboutBox.h \
           source/MainWindow.h \
           source/MainWindowComponent.h \
           source/SplashScreen.h
           
FORMS    = source/AboutBox.ui

SOURCES  = source/AboutBox.cpp \
           source/MainWindow.cpp \
           source/MainWindowComponent.cpp \
           source/SplashScreen.cpp

RESOURCES = MainWindow.qrc
