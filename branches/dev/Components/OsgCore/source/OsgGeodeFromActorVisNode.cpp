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

#include "OsgGeodeFromActorVisNode.h"
#include "OsgCoreComponent.h"
#include <osg/Geode>
#include <osg/ref_ptr>
#include "CVtkActorData.h"
#include "vtkActorToOSG.h"

DEFINE_VIS_NODE(OsgGeodeFromActorVisNode, CGenericVisNodeBase)
{
    pDesc->setNodeClassCategory("OsgCore");
    pDesc->setNodeClassName("Geode from Actor");
    pDesc->setNodeClassDescription("Convert a vtkActor to an Osg geode.");
    pDesc->setNodeIcon( OsgCoreComponent::instance().nodeIcon() );

	pDesc->addConnectionPath(
        new CGenericVisNodeConnectionPath(
                "OsgGeode",                                 // Name of the path
                IVisSystemNodeConnectionPath::OutputPath,   // Path type can be OutputPath or InputPath
                "osg::ref_ptr<osg::Geode>",                                 // Data type of the path
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
                false                                       // Allow Multiple Inputs Flag
            )
        );
}

struct OsgGeodeFromActorVisNodeData
{
	OsgGeodeFromActorVisNodeData() : outputGeode(0), inputActor(0) { }
	osg::ref_ptr<osg::Geode> outputGeode;
	OsgGeodeFromActorVisNodeIOData outputData;
	vtkActor* inputActor;
};

OsgGeodeFromActorVisNode::OsgGeodeFromActorVisNode()
{
    OsgGeodeFromActorVisNode::InitializeNodeDesc();
    d = new OsgGeodeFromActorVisNodeData;

    
}

OsgGeodeFromActorVisNode::~OsgGeodeFromActorVisNode()
{
    // This would be a good time to delete the backend object. If the backend object is a vtkObject
    // subclass, then you have to delete it now.
    // d->VtkObjectPointer->Delete();

    
    delete d;
}

void OsgGeodeFromActorVisNode::update()
{
    command_Update();
}

void OsgGeodeFromActorVisNode::command_Update()
{
	d->outputData.setOsgGeode(createGeodeFromActor(d->inputActor));
	d->outputGeode = d->outputData.getOsgGeode();

}

bool OsgGeodeFromActorVisNode::hasInput(IVisSystemNodeConnectionPath* path)
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

bool OsgGeodeFromActorVisNode::setInput(IVisSystemNodeConnectionPath* path, IVisSystemNodeIOData* inputData)
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
			d->outputData.setOsgGeode(createGeodeFromActor(d->inputActor));
			d->outputGeode = d->outputData.getOsgGeode();

			return true;
		}
	}

    return CGenericVisNodeBase::setInput(path, inputData);
}

bool OsgGeodeFromActorVisNode::removeInput(IVisSystemNodeConnectionPath* path, IVisSystemNodeIOData* inputData)
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
		if (success && actorData && actorData->getVtkActor() == vtkActor::SafeDownCast(d->inputActor))
		{
			d->inputActor = 0;

			return  true;
		}
	}

    return CGenericVisNodeBase::removeInput(path, inputData);
}

bool OsgGeodeFromActorVisNode::fetchOutput(IVisSystemNodeConnectionPath* path, IVisSystemNodeIOData** outputData)
{
    if(!path || !outputData)
        return false;

    /*
    If you have added output paths in the description block at the header of this file,
    then you will have to handle outputs here
    */
	if (path->pathName() == "OsgGeode")
	{
		d->outputData.setOsgGeode(d->outputGeode);
		*outputData = &d->outputData;
		return true;
	}

    return CGenericVisNodeBase::fetchOutput(path, outputData);
}

bool OsgGeodeFromActorVisNode::outputDerefed(IVisSystemNodeConnectionPath* path, IVisSystemNodeIOData* outputData)
{
    if(!path || !outputData)
        return false;

    /*
    If you have added output paths in the description block at the header of this file,
    then you will have to handle outputs here
    */
	if (path->pathName() == "OsgGeode")
		return true;

    return CGenericVisNodeBase::outputDerefed(path, outputData);
}

osg::ref_ptr<osg::Geode> OsgGeodeFromActorVisNode::createGeodeFromActor(vtkActor* actor)
{
	osg::ref_ptr<osg::Geode> geode = vtkActorToOSG(actor, 0, 0).get();
	if (!geode)
		geode = new osg::Geode;

	return geode;
}

#ifdef ENABLE_ADVANCED_PROPERTIES

int OsgGeodeFromActorVisNode::propertyCount()
{
    return 0;
}

QString OsgGeodeFromActorVisNode::propertyName(int index)
{
    Q_UNUSED(index);
    return QString();
}

QString OsgGeodeFromActorVisNode::propertyGroup(int index)
{
    Q_UNUSED(index);
    return QString();
}

QVariant OsgGeodeFromActorVisNode::propertyValue(int index)
{
    Q_UNUSED(index);
    return QVariant();
}

void OsgGeodeFromActorVisNode::setPropertyValue(int index, QVariant value)
{
    Q_UNUSED(index);
    Q_UNUSED(value);
}

bool OsgGeodeFromActorVisNode::hasEditor(int index)
{
    Q_UNUSED(index);
    return false;
}

QWidget* OsgGeodeFromActorVisNode::createEditor(int index)
{
    Q_UNUSED(index);
    return 0;
}

void OsgGeodeFromActorVisNode::setEditorValue(int index, QWidget* widget, QVariant value)
{
    Q_UNUSED(index);
    Q_UNUSED(widget);
    Q_UNUSED(value);
}

QVariant OsgGeodeFromActorVisNode::editorValue(int index, QWidget* widget)
{
    Q_UNUSED(index);
    Q_UNUSED(widget);
}

void OsgGeodeFromActorVisNode::connectEditor(int index, QWidget* widget, QObject* receiver, const char* member)
{
    Q_UNUSED(index);
    Q_UNUSED(widget);
    Q_UNUSED(receiver);
    Q_UNUSED(member);
}

QString OsgGeodeFromActorVisNode::propertyValueString(int index)
{
    Q_UNUSED(index);
    return QString();
}

QVariant OsgGeodeFromActorVisNode::propertyValueFromString(int index, QString valueStr)
{
    Q_UNUSED(index);
    Q_UNUSED(valueStr);

    return QVariant();
}

bool OsgGeodeFromActorVisNode::canLoadSaveProperty(int index)
{
    Q_UNUSED(index);

    return false;
}

#endif





