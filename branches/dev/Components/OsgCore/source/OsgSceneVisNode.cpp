/****************************************************************************
**
** Copyright (C) UNO
**
** Use of this file is limited according to the terms specified by
** UNO.
**
** Details of those terms are listed in licence.txt included as
** part of the distribution package of this file. This file may not
** be distributed without including the licence.txt file.
**
** This file is provided AS IS with NO WARRANTY OF ANY KIND, INCLUDING THE
** WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
**
****************************************************************************/

#include "OsgSceneVisNode.h"
#include "OsgCoreComponent.h"
#include "vtkActor.h"
#include "vtkActorToOSG.h"
#include "CVtkActorData.h"

// includes tests
#include <QtGui/QLabel>
#include <QtGui/QDialog>
#include <QtGui/QWidget>
#include <QtGui/QLayout>
#include <QtGui/QGraphicsScene>
#include <QtGui/QGraphicsProxyWidget>
#include "vtkConeSource.h"
#include "vtkPolyDataMapper.h"
#include "vtkProperty.h"

#include "vtkGlyph3D.h"
#include "vtkSphereSource.h"
#include "vtkPolyDataNormals.h"
#include "vtkGlyph3D.h"
#include "vtkGlyph3D.h"

#include <osg/Geode>
#include <osg/ref_ptr>
#include <osg/MatrixTransform>
#include <osgGA/TrackballManipulator>

DEFINE_VIS_NODE(OsgSceneVisNode, CGenericVisNodeBase)
{
    pDesc->setNodeClassCategory("OsgCore");
    pDesc->setNodeClassName("Scene from Actor");
    pDesc->setNodeClassDescription("Convert an Actor to an Osg scene.");
    pDesc->setNodeIcon( OsgCoreComponent::instance().nodeIcon() );

    // Uncomment and use the following code template to add input/output paths
    
    pDesc->addConnectionPath(
        new CGenericVisNodeConnectionPath(
                "OsgScene",                                 // Name of the path
                IVisSystemNodeConnectionPath::OutputPath,   // Path type can be OutputPath or InputPath
                "osg::QOSGScene",                                 // Data type of the path
                0,                                          // Path index (don't change)
                false                                       // Allow Multiple Inputs Flag
            )
        );

    pDesc->addConnectionPath(
        new CGenericVisNodeConnectionPath(
                "vtkActorInput",                                 // Name of the path
                IVisSystemNodeConnectionPath::InputPath,   // Path type can be OutputPath or InputPath
                "vtkActor",                                 // Data type of the path
                0,                                          // Path index (don't change)
                true                                       // Allow Multiple Inputs Flag
            )
        );
}

struct OsgSceneVisNodeData
{
	OsgSceneVisNodeData() : outputScene(0), inputActor(0) { }
	osg::QOSGScene * outputScene;
	OsgSceneVisNodeIOData outputData;
	vtkActor* inputActor;
};

OsgSceneVisNode::OsgSceneVisNode()
{
    OsgSceneVisNode::InitializeNodeDesc();
    d = new OsgSceneVisNodeData;
}

OsgSceneVisNode::~OsgSceneVisNode()
{
    // This would be a good time to delete the backend object. If the backend object is a vtkObject
    // subclass, then you have to delete it now.
    // d->VtkObjectPointer->Delete();

    
    delete d;
}

void OsgSceneVisNode::update()
{
    command_Update();
}

void OsgSceneVisNode::command_Update()
{
	d->outputData.setOsgScene(createDefaultSceneFromActor(d->inputActor));
	d->outputScene = d->outputData.getOsgScene();

}

bool OsgSceneVisNode::hasInput(IVisSystemNodeConnectionPath* path)
{
    if(!path)
        return false;

    /*
    If you have added input paths in the description block at the header of this file,
    then you will have to handle inputs here
    */

	if (path->pathName() == "vtkActorInput")
		return d->inputActor != 0;

    return CGenericVisNodeBase::hasInput(path);
}

bool OsgSceneVisNode::setInput(IVisSystemNodeConnectionPath* path, IVisSystemNodeIOData* inputData)
{
    if(!path || !inputData)
        return false;

    /*
    If you have added input paths in the description block at the header of this file,
    then you will have to handle inputs here
    */
	if (path->pathName() == "vtkActorInput")
	{
		CVtkActorData * actorData = 0;
		bool success = inputData->queryInterface("CVtkActorData", (void**)&actorData);
		if (success && actorData)
		{
			d->inputActor = actorData->getVtkActor();
			d->outputData.setOsgScene(createDefaultSceneFromActor(d->inputActor));
			d->outputScene = d->outputData.getOsgScene();

			return true;
		}
	}

    return CGenericVisNodeBase::setInput(path, inputData);
}

bool OsgSceneVisNode::removeInput(IVisSystemNodeConnectionPath* path, IVisSystemNodeIOData* inputData)
{
    if(!path || !inputData)
        return false;

    /*
    If you have added input paths in the description block at the header of this file,
    then you will have to handle inputs here
    */
	if (path->pathName() == "vtkActorInput")
	{
		CVtkActorData * data = 0;
		bool success = inputData->queryInterface("CVtkActorData", (void**)&data);
		if (success && data && data->getVtkActor() == vtkActor::SafeDownCast(d->inputActor))
		{
			d->inputActor = 0;

			return  true;
		}
	}

    return CGenericVisNodeBase::removeInput(path, inputData);
}

bool OsgSceneVisNode::fetchOutput(IVisSystemNodeConnectionPath* path, IVisSystemNodeIOData** outputData)
{
    if(!path || !outputData)
        return false;

    /*
    If you have added output paths in the description block at the header of this file,
    then you will have to handle outputs here
    */

	if (path->pathName() == "OsgScene")
	{
		d->outputData.setOsgScene(d->outputScene);
		*outputData = &d->outputData;
		return true;
	}
    return CGenericVisNodeBase::fetchOutput(path, outputData);
}

bool OsgSceneVisNode::outputDerefed(IVisSystemNodeConnectionPath* path, IVisSystemNodeIOData* outputData)
{
    if(!path || !outputData)
        return false;

    /*
    If you have added output paths in the description block at the header of this file,
    then you will have to handle outputs here
    */

	if (path->pathName() == "OsgScene")
		return true;

    return CGenericVisNodeBase::outputDerefed(path, outputData);
}


#ifdef ENABLE_ADVANCED_PROPERTIES

int OsgSceneVisNode::propertyCount()
{
    return 0;
}

QString OsgSceneVisNode::propertyName(int index)
{
    Q_UNUSED(index);
    return QString();
}

QString OsgSceneVisNode::propertyGroup(int index)
{
    Q_UNUSED(index);
    return QString();
}

QVariant OsgSceneVisNode::propertyValue(int index)
{
    Q_UNUSED(index);
    return QVariant();
}

void OsgSceneVisNode::setPropertyValue(int index, QVariant value)
{
    Q_UNUSED(index);
    Q_UNUSED(value);
}

bool OsgSceneVisNode::hasEditor(int index)
{
    Q_UNUSED(index);
    return false;
}

QWidget* OsgSceneVisNode::createEditor(int index)
{
    Q_UNUSED(index);
    return 0;
}

void OsgSceneVisNode::setEditorValue(int index, QWidget* widget, QVariant value)
{
    Q_UNUSED(index);
    Q_UNUSED(widget);
    Q_UNUSED(value);
}

QVariant OsgSceneVisNode::editorValue(int index, QWidget* widget)
{
    Q_UNUSED(index);
    Q_UNUSED(widget);
}

void OsgSceneVisNode::connectEditor(int index, QWidget* widget, QObject* receiver, const char* member)
{
    Q_UNUSED(index);
    Q_UNUSED(widget);
    Q_UNUSED(receiver);
    Q_UNUSED(member);
}

QString OsgSceneVisNode::propertyValueString(int index)
{
    Q_UNUSED(index);
    return QString();
}

QVariant OsgSceneVisNode::propertyValueFromString(int index, QString valueStr)
{
    Q_UNUSED(index);
    Q_UNUSED(valueStr);

    return QVariant();
}

bool OsgSceneVisNode::canLoadSaveProperty(int index)
{
    Q_UNUSED(index);

    return false;
}

#endif

osg::QOSGScene* OsgSceneVisNode::createDefaultSceneFromActor(vtkActor* actor)
{
	osg::ref_ptr<osg::Geode> geode = vtkActorToOSG(actor, 0, 0).get();

	osg::ref_ptr<osg::Light> light = new osg::Light;
	light->setLightNum(2);
	light->setAmbient(osg::Vec4(.1f, .1f, .1f, .1f));
	light->setDiffuse(osg::Vec4(.8f, .8f, .8f, .1f));
	light->setSpecular(osg::Vec4(.8f, .8f, .8f, .1f));
	light->setPosition(osg::Vec4(.0f, .0f, .0f, .0f));
	light->setDirection(osg::Vec3(.1f, .0f, .0f));
	light->setSpotCutoff(25.f);
	osg::LightSource * lightSource = new osg::LightSource;
	lightSource->setLight(light.get());

	osg::ref_ptr<osg::Group> group = new osg::Group;
	group->addChild(geode);

	osg::QOSGScene *pScene = new osg::QOSGScene;

	pScene->setCameraManipulator(new osgGA::TrackballManipulator);
	pScene->setSceneData(group.get());
	pScene->setLight(light);
	return pScene;
}

osg::QOSGScene * OsgSceneVisNode::createTestScene()
{
	vtkConeSource *cone;
	vtkSphereSource *sphere;
	vtkActor *sphereActor, *spikeActor;
	vtkGlyph3D *glyph;
	vtkPolyDataMapper *sphereMapper, *spikeMapper;

	sphere = vtkSphereSource::New();
	sphereMapper = vtkPolyDataMapper::New();
	sphereMapper->SetInput(sphere->GetOutput());
	sphereActor = vtkActor::New();
	sphereActor->SetMapper(sphereMapper);
	sphereActor->GetProperty()->SetColor(0.407263, 1, 0.278904);

	sphereActor->GetProperty()->SetOpacity(1.0);

	cone = vtkConeSource::New();

	glyph = vtkGlyph3D::New();

	glyph->SetInput((vtkDataSet *) sphere->GetOutput());

	glyph->SetSource(cone->GetOutput());
	glyph->SetVectorModeToUseNormal();
	glyph->SetScaleModeToScaleByVector();
	glyph->SetScaleFactor(0.25);

	vtkPolyDataNormals *spikeNormals = vtkPolyDataNormals::New();
	spikeNormals->SetInput(glyph->GetOutput());

	spikeMapper = vtkPolyDataMapper::New();
	spikeMapper->SetInput(spikeNormals->GetOutput());

	spikeActor = vtkActor::New();
	spikeActor->SetMapper(spikeMapper);
	spikeActor->GetProperty()->SetColor(0.7, 0.2, 0.2);

	osg::ref_ptr<osg::Geode> loadedModel = vtkActorToOSG(sphereActor, 0, 0).get();
	osg::ref_ptr<osg::Geode> loadedModel1 = vtkActorToOSG(spikeActor, 0, 0).get();

	osg::ref_ptr<osg::Light> light = new osg::Light;
	light->setLightNum(2);
	light->setAmbient(osg::Vec4(.1f, .1f, .1f, .1f));
	light->setDiffuse(osg::Vec4(.8f, .8f, .8f, .1f));
	light->setSpecular(osg::Vec4(.8f, .8f, .8f, .1f));
	light->setPosition(osg::Vec4(.0f, .0f, .0f, .0f));
	light->setDirection(osg::Vec3(.1f, .0f, .0f));
	light->setSpotCutoff(25.f);
	osg::LightSource * lightSource = new osg::LightSource;
	lightSource->setLight(light.get());

	osg::ref_ptr<osg::Group> group = new osg::Group;
	group->addChild(loadedModel);
	group->addChild(loadedModel1);

	osg::QOSGScene *pScene = new osg::QOSGScene;

	pScene->setCameraManipulator(new osgGA::TrackballManipulator);
	pScene->setSceneData(group.get());
	pScene->setLight(light);
	return pScene;
}

