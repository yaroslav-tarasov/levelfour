######################################################################
# VTK Designer 2 - OsgCore component
# 
# Autogenerated using VNFComponentGen
######################################################################

include(../Components.pri)
include(../../VTK.pri)

MOC_DIR         = $${PROJECT_BASE}/tmp/OsgCore
OBJECTS_DIR     = $${PROJECT_BASE}/tmp/OsgCore
UI_HEADERS_DIR  = $${PROJECT_BASE}/tmp/OsgCore
RCC_DIR         = $${PROJECT_BASE}/tmp/OsgCore
UI_DIR          = $${PROJECT_BASE}/tmp/OsgCore

DEPENDPATH      += . \
                   Interfaces 

# Additional includes from the VTK system
INCLUDEPATH     += $${PROJECT_BASE}/Components/VTKSystem/Interfaces \
                   $${PROJECT_BASE}/Components/VTKSystem/IOData \
                   $${PROJECT_BASE}/Common \
                   Interfaces 

# Input
HEADERS += source/OsgCoreComponent.h \
           source/OsgSimpleViewVisNode.h \
           source/OsgCompositeViewVisNode.h \
           source/OsgLightVisNode.h \
			source/OsgLightVisNodeIOData.h \
           source/OsgGroupVisNode.h \
			source/OsgGroupVisNodeIOData.h \
           source/OsgGeodeFromActorVisNode.h \
           source/OsgGeodeFromActorVisNodeIOData.h \
           source/OsgGeodeVisNode.h \
           source/OsgBoxVisNode.h \
           source/OsgSphereVisNode.h \
			source/QOSGWidget.h \
			source/QOSGGraphics.h \
			source/vtkActorToOSG.h \
			source/vtkUpdateCallback.h \
			source/OsgNodeVisNodeIOData.h \
           source/OsgCylinderVisNode.h \
		   source/OsgDrawableVisNodeIOData.h \
				
SOURCES += source/OsgCoreComponent.cpp \
           source/OsgSimpleViewVisNode.cpp \
           source/OsgCompositeViewVisNode.cpp \
           source/OsgLightVisNode.cpp \
           source/OsgGroupVisNode.cpp \
           source/OsgGeodeFromActorVisNode.cpp \
           source/OsgGeodeFromActorVisNodeIOData.cpp \
			source/QOSGWidget.cpp \
			source/QOSGGraphics.cpp \
			source/vtkActorToOSG.cpp \
			source/OsgGroupVisNodeIOData.cpp \
			source/OsgLightVisNodeIOData.cpp \
			source/vtkUpdateCallback.cpp \
           source/OsgGeodeVisNode.cpp \
           source/OsgBoxVisNode.cpp \
           source/OsgSphereVisNode.cpp \
           source/OsgCylinderVisNode.cpp \
			source/OsgNodeVisNodeIOData.cpp \
		   source/OsgDrawableVisNodeIOData.cpp \


LIBS			+= -losgd \
				-losgGAd \
				-losgViewerd \
				-losgDBd \
				-lQtGuid4 \
				-lQtCored4 \
				-lQtOpenGLd4 \

# VNFComponentGen has generated an empty resource file. If you add
# new objects to the resource file, then you can uncomment the line
# below
# RESOURCES += OsgCore.qrc

# Nodes in VTK Designer 2 have to implement one or more node interfaces
# Implementing all of them from scratch would be a tough task. So, we make
# use of ready made implementations and then subclass from them.
HEADERS   +=  $${PROJECT_BASE}/Common/CGenericVisNodeDesc.h \
              $${PROJECT_BASE}/Common/CGenericVisNodeBase.h 

SOURCES   +=  $${PROJECT_BASE}/Common/CGenericVisNodeDesc.cpp \
              $${PROJECT_BASE}/Common/CGenericVisNodeBase.cpp 


# IOData classes. These classes are so small, that they can actually be inline implementations.
# So there is really no need to worry about code size
HEADERS +=  $${PROJECT_BASE}/Components/VTKSystem/IOData/CVtk3DWidgetData.h \
            $${PROJECT_BASE}/Components/VTKSystem/IOData/CVtkAbstractMapper3DData.h \
            $${PROJECT_BASE}/Components/VTKSystem/IOData/CVtkAbstractMapperData.h \
            $${PROJECT_BASE}/Components/VTKSystem/IOData/CVtkAbstractPickerData.h \
            $${PROJECT_BASE}/Components/VTKSystem/IOData/CVtkAbstractTransformData.h \
            $${PROJECT_BASE}/Components/VTKSystem/IOData/CVtkActor2DData.h \
            $${PROJECT_BASE}/Components/VTKSystem/IOData/CVtkActorData.h \
            $${PROJECT_BASE}/Components/VTKSystem/IOData/CVtkAlgorithmData.h \
            $${PROJECT_BASE}/Components/VTKSystem/IOData/CVtkCameraData.h \
            $${PROJECT_BASE}/Components/VTKSystem/IOData/CVtkDataObject.h \
            $${PROJECT_BASE}/Components/VTKSystem/IOData/CVtkDataSet.h \
            $${PROJECT_BASE}/Components/VTKSystem/IOData/CVtkDataSetMapperData.h \
            $${PROJECT_BASE}/Components/VTKSystem/IOData/CVtkImageData.h \
            $${PROJECT_BASE}/Components/VTKSystem/IOData/CVtkImplicitFunctionData.h \
            $${PROJECT_BASE}/Components/VTKSystem/IOData/CVtkImporterData.h \
            $${PROJECT_BASE}/Components/VTKSystem/IOData/CVtkInteractorObserverData.h \
            $${PROJECT_BASE}/Components/VTKSystem/IOData/CVtkLocatorData.h \
            $${PROJECT_BASE}/Components/VTKSystem/IOData/CVtkLookupTableData.h \
            $${PROJECT_BASE}/Components/VTKSystem/IOData/CVtkMapper2DData.h \
            $${PROJECT_BASE}/Components/VTKSystem/IOData/CVtkMapperData.h \
            $${PROJECT_BASE}/Components/VTKSystem/IOData/CVtkObjectIOData.h \
            $${PROJECT_BASE}/Components/VTKSystem/IOData/CVtkPiecewiseFunctionData.h \
            $${PROJECT_BASE}/Components/VTKSystem/IOData/CVtkPointSet.h \
            $${PROJECT_BASE}/Components/VTKSystem/IOData/CVtkPolyData.h \
            $${PROJECT_BASE}/Components/VTKSystem/IOData/CVtkPolyDataMapperData.h \
            $${PROJECT_BASE}/Components/VTKSystem/IOData/CVtkProp3DData.h \
            $${PROJECT_BASE}/Components/VTKSystem/IOData/CVtkPropertyData.h \
            $${PROJECT_BASE}/Components/VTKSystem/IOData/CVtkRectilinearGrid.h \
            $${PROJECT_BASE}/Components/VTKSystem/IOData/CVtkRendererData.h \
            $${PROJECT_BASE}/Components/VTKSystem/IOData/CVtkRenderWindowData.h \
            $${PROJECT_BASE}/Components/VTKSystem/IOData/CVtkRenderWindowInteractorData.h \
            $${PROJECT_BASE}/Components/VTKSystem/IOData/CVtkScalarsToColorsData.h \
            $${PROJECT_BASE}/Components/VTKSystem/IOData/CVtkStructuredGrid.h \
            $${PROJECT_BASE}/Components/VTKSystem/IOData/CVtkStructuredPoints.h \
            $${PROJECT_BASE}/Components/VTKSystem/IOData/CVtkTextureData.h \
            $${PROJECT_BASE}/Components/VTKSystem/IOData/CVtkUniformGrid.h \
            $${PROJECT_BASE}/Components/VTKSystem/IOData/CVtkUnstructuredGrid.h 

SOURCES +=  $${PROJECT_BASE}/Components/VTKSystem/IOData/CVtk3DWidgetData.cpp \
            $${PROJECT_BASE}/Components/VTKSystem/IOData/CVtkAbstractMapper3DData.cpp \
            $${PROJECT_BASE}/Components/VTKSystem/IOData/CVtkAbstractMapperData.cpp \
            $${PROJECT_BASE}/Components/VTKSystem/IOData/CVtkAbstractPickerData.cpp \
            $${PROJECT_BASE}/Components/VTKSystem/IOData/CVtkAbstractTransformData.cpp \
            $${PROJECT_BASE}/Components/VTKSystem/IOData/CVtkActor2DData.cpp \
            $${PROJECT_BASE}/Components/VTKSystem/IOData/CVtkActorData.cpp \
            $${PROJECT_BASE}/Components/VTKSystem/IOData/CVtkAlgorithmData.cpp \
            $${PROJECT_BASE}/Components/VTKSystem/IOData/CVtkCameraData.cpp \
            $${PROJECT_BASE}/Components/VTKSystem/IOData/CVtkDataObject.cpp \
            $${PROJECT_BASE}/Components/VTKSystem/IOData/CVtkDataSet.cpp \
            $${PROJECT_BASE}/Components/VTKSystem/IOData/CVtkDataSetMapperData.cpp \
            $${PROJECT_BASE}/Components/VTKSystem/IOData/CVtkImageData.cpp \
            $${PROJECT_BASE}/Components/VTKSystem/IOData/CVtkImplicitFunctionData.cpp \
            $${PROJECT_BASE}/Components/VTKSystem/IOData/CVtkImporterData.cpp \
            $${PROJECT_BASE}/Components/VTKSystem/IOData/CVtkInteractorObserverData.cpp \
            $${PROJECT_BASE}/Components/VTKSystem/IOData/CVtkLocatorData.cpp \
            $${PROJECT_BASE}/Components/VTKSystem/IOData/CVtkLookupTableData.cpp \
            $${PROJECT_BASE}/Components/VTKSystem/IOData/CVtkMapper2DData.cpp \
            $${PROJECT_BASE}/Components/VTKSystem/IOData/CVtkMapperData.cpp \
            $${PROJECT_BASE}/Components/VTKSystem/IOData/CVtkObjectIOData.cpp \
            $${PROJECT_BASE}/Components/VTKSystem/IOData/CVtkPiecewiseFunctionData.cpp \
            $${PROJECT_BASE}/Components/VTKSystem/IOData/CVtkPointSet.cpp \
            $${PROJECT_BASE}/Components/VTKSystem/IOData/CVtkPolyData.cpp \
            $${PROJECT_BASE}/Components/VTKSystem/IOData/CVtkPolyDataMapperData.cpp \
            $${PROJECT_BASE}/Components/VTKSystem/IOData/CVtkProp3DData.cpp \
            $${PROJECT_BASE}/Components/VTKSystem/IOData/CVtkPropertyData.cpp \
            $${PROJECT_BASE}/Components/VTKSystem/IOData/CVtkRectilinearGrid.cpp \
            $${PROJECT_BASE}/Components/VTKSystem/IOData/CVtkRendererData.cpp \
            $${PROJECT_BASE}/Components/VTKSystem/IOData/CVtkRenderWindowData.cpp \
            $${PROJECT_BASE}/Components/VTKSystem/IOData/CVtkRenderWindowInteractorData.cpp \
            $${PROJECT_BASE}/Components/VTKSystem/IOData/CVtkScalarsToColorsData.cpp \
            $${PROJECT_BASE}/Components/VTKSystem/IOData/CVtkStructuredGrid.cpp \
            $${PROJECT_BASE}/Components/VTKSystem/IOData/CVtkStructuredPoints.cpp \
            $${PROJECT_BASE}/Components/VTKSystem/IOData/CVtkTextureData.cpp \
            $${PROJECT_BASE}/Components/VTKSystem/IOData/CVtkUniformGrid.cpp \
            $${PROJECT_BASE}/Components/VTKSystem/IOData/CVtkUnstructuredGrid.cpp 
            
