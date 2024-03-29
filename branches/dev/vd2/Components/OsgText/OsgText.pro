######################################################################
# VTK Designer 2 - OsgText component
# 
# Autogenerated using VNFComponentGen
######################################################################

include(../Components.pri)

MOC_DIR         = $${PROJECT_BASE}/tmp/OsgText
OBJECTS_DIR     = $${PROJECT_BASE}/tmp/OsgText
UI_HEADERS_DIR  = $${PROJECT_BASE}/tmp/OsgText
RCC_DIR         = $${PROJECT_BASE}/tmp/OsgText
UI_DIR          = $${PROJECT_BASE}/tmp/OsgText

# Additional includes from the VTK system
INCLUDEPATH     += $${PROJECT_BASE}/Common

# Input
HEADERS += source/OsgTextComponent.h \
           source/OsgFadeTextVisNode.h \
           source/OsgStringVisNode.h \
           source/OsgTextVisNode.h \
           source/OsgText3DVisNode.h \
 

SOURCES += source/OsgTextComponent.cpp \
           source/OsgFadeTextVisNode.cpp \
           source/OsgStringVisNode.cpp \
           source/OsgTextVisNode.cpp \
           source/OsgText3DVisNode.cpp \


# VNFComponentGen has generated an empty resource file. If you add
# new objects to the resource file, then you can uncomment the line
# below
RESOURCES += OsgText.qrc

# Nodes in VTK Designer 2 have to implement one or more node interfaces
# Implementing all of them from scratch would be a tough task. So, we make
# use of ready made implementations and then subclass from them.
HEADERS   +=  $${PROJECT_BASE}/Common/CGenericVisNodeDesc.h \
              $${PROJECT_BASE}/Common/CGenericVisNodeBase.h 

SOURCES   +=  $${PROJECT_BASE}/Common/CGenericVisNodeDesc.cpp \
              $${PROJECT_BASE}/Common/CGenericVisNodeBase.cpp 


