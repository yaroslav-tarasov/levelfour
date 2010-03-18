######################################################################
# Generic Component Framework Library from VCreate Logic
# 
# Provides a robust, extensible and re-usable property editor component
######################################################################

include(../Components.pri)

TEMPLATE        = lib
CONFIG         += dll 
LIBS           += $$GCF_LIBS
DEPENDPATH     += . interfaces source
INCLUDEPATH    += . interfaces source \
                  ../ScriptEditor/interfaces

MOC_DIR         = $${PROJECT_BASE}/tmp/PropertyEditor
OBJECTS_DIR     = $${PROJECT_BASE}/tmp/PropertyEditor
UI_HEADERS_DIR  = $${PROJECT_BASE}/tmp/PropertyEditor
RCC_DIR         = $${PROJECT_BASE}/tmp/PropertyEditor
UI_DIR          = $${PROJECT_BASE}/tmp/PropertyEditor

# Input
HEADERS  = interfaces/IDefaultProperty.h \
           interfaces/IDefaultPropertyManager.h \
           interfaces/IEditableProperties.h \
           interfaces/IProperty.h \
           interfaces/IPropertyEditor.h \
           interfaces/IQObjectLoadSave.h \
           source/ColorPickerWidget.h \
           source/Property.h \
           source/PropertyEditor.h \
           source/PropertyEditorComponent.h \
           source/ValueEditorCreators.h \
           source/ValueEditorFactory.h
           
FORMS    = source/ColorPickerWidget.ui

SOURCES  = source/ColorPickerWidget.cpp \
           source/Property.cpp \
           source/PropertyEditor.cpp \
           source/PropertyEditorComponent.cpp \
           source/ValueEditorCreators.cpp \
           source/ValueEditorFactory.cpp

