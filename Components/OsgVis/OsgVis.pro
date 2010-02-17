######################################################################
# VTK Designer 2 - OsgVis component
# 
# Autogenerated using VNFComponentGen
######################################################################

include(../Components.pri)
include(../../VTK.pri)

MOC_DIR         = $${PROJECT_BASE}/tmp/OsgVis
OBJECTS_DIR     = $${PROJECT_BASE}/tmp/OsgVis
UI_HEADERS_DIR  = $${PROJECT_BASE}/tmp/OsgVis
RCC_DIR         = $${PROJECT_BASE}/tmp/OsgVis
UI_DIR          = $${PROJECT_BASE}/tmp/OsgVis

# Additional includes from the VTK system
INCLUDEPATH     += $${PROJECT_BASE}/Components/OsgCore/source \
                   $${PROJECT_BASE}/Common

# Input
HEADERS += source/OsgVisComponent.h \
           source/LayerVisNode.h \
		   source/LayerVisNodeIOData.h \
           source/MapVisNode.h \
           source/ModelVisNode.h \
		   source/ModelVisNodeIOData.h \
           source/FeaturesVisNode.h \
		   source/FeaturesVisNodeIOData.h \
           ../OsgCore/source/OsgNodeVisNodeIOData.h \
           ../OsgCore/source/OsgGroupVisNodeIOData.h \


SOURCES += source/OsgVisComponent.cpp \
           source/LayerVisNode.cpp \
		   source/LayerVisNodeIOData.cpp \
           source/ModelVisNode.cpp \
		   source/ModelVisNodeIOData.cpp \
           source/FeaturesVisNode.cpp \
		   source/FeaturesVisNodeIOData.cpp \
           source/MapVisNode.cpp \
           ../OsgCore/source/OsgNodeVisNodeIOData.cpp \
           ../OsgCore/source/OsgGroupVisNodeIOData.cpp \

LIBS			+= -losgd \
				-losgGAd \
				-losgDBd \
				-losgEarthd \
				-losgEarthFeaturesd \
				-losgEarthUtild \
				-lopenThreadsd \
				
# VNFComponentGen has generated an empty resource file. If you add
# new objects to the resource file, then you can uncomment the line
# below
# RESOURCES += OsgVis.qrc

# Nodes in VTK Designer 2 have to implement one or more node interfaces
# Implementing all of them from scratch would be a tough task. So, we make
# use of ready made implementations and then subclass from them.
HEADERS   +=  $${PROJECT_BASE}/Common/CGenericVisNodeDesc.h \
              $${PROJECT_BASE}/Common/CGenericVisNodeBase.h 

SOURCES   +=  $${PROJECT_BASE}/Common/CGenericVisNodeDesc.cpp \
              $${PROJECT_BASE}/Common/CGenericVisNodeBase.cpp 

