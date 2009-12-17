######################################################################
# VTK Designer Main Window from VCreate Logic
######################################################################

include(../Components.pri)

TEMPLATE    = app
TARGET      = LevelFour
DESTDIR     = $${PROJECT_BASE}/bin
DEPENDPATH  += . source
INCLUDEPATH += . source

LIBS        += $${GCF_LIBS}
win32 {
 LIBS       += $$PROJECT_BASE/lib/MainWindow.lib
} else {
 LIBS       += -lMainWindow -L$$PROJECT_BASE/lib
}

# Input
SOURCES += source/Main.cpp

RESOURCES += Main.qrc

# CONFIG += console

RC_FILE = icons/vtkd.rc


