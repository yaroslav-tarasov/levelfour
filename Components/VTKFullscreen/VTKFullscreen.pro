######################################################################
# VTK Designer 2 - VTKFullscreen component
# 
# Allows viewing of 3D output in full screen.
######################################################################

include(../Components.pri)
include(../../VTK.pri)

MOC_DIR         = $${PROJECT_BASE}/tmp/VTKFullscreen
OBJECTS_DIR     = $${PROJECT_BASE}/tmp/VTKFullscreen
UI_HEADERS_DIR  = $${PROJECT_BASE}/tmp/VTKFullscreen
RCC_DIR         = $${PROJECT_BASE}/tmp/VTKFullscreen
UI_DIR          = $${PROJECT_BASE}/tmp/VTKFullscreen

# Input
HEADERS += source/CFullscreenRenderWindow.h source/CVTKFullscreenComponent.h
FORMS += source/CFullscreenRenderWindowToolbar.ui
SOURCES += source/CFullscreenRenderWindow.cpp source/CVTKFullscreenComponent.cpp
RESOURCES += VTKFullscreen.qrc
