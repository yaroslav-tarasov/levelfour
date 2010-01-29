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

#include "OsgGroupVisNode.h"
#include "OsgCoreComponent.h"
#include "OsgGeodeFromActorVisNodeIOData.h"
#include "OsgLightVisNodeIOData.h"
#include <osgGA/TrackballManipulator>
#include <osg/LightSource>

DEFINE_VIS_NODE(OsgGroupVisNode, CGenericVisNodeBase)
{
    pDesc->setNodeClassCategory("OsgCore");
    pDesc->setNodeClassName("Group");
    pDesc->setNodeClassDescription("Group multiple nodes (geodes or groups) togheter.");
    pDesc->setNodeIcon( OsgCoreComponent::instance().nodeIcon() );

    // Uncomment and use the following code template to add input/output paths
    
    pDesc->addConnectionPath(
        new CGenericVisNodeConnectionPath(
                "OsgGroup",                                 // Name of the path
                IVisSystemNodeConnectionPath::OutputPath,   // Path type can be OutputPath or InputPath
                "osg::ref_ptr<osg::Group>",                                 // Data type of the path
                0,                                          // Path index (don't change)
                false                                       // Allow Multiple Inputs Flag
            )
        );

    pDesc->addConnectionPath(
        new CGenericVisNodeConnectionPath(
                "OsgNodeInput",                                 // Name of the path
                IVisSystemNodeConnectionPath::InputPath,   // Path type can be OutputPath or InputPath
                "osg::ref_ptr<osg::Node>",                                 // Data type of the path
                0,                                          // Path index (don't change)
                true                                       // Allow Multiple Inputs Flag
            )
        );
}

struct OsgGroupVisNodeData
{
	OsgGroupVisNodeData() : outputGroup(0) { }
	osg::ref_ptr<osg::Group> outputGroup;
	OsgGroupVisNodeIOData outputGroupData;
};

OsgGroupVisNode::OsgGroupVisNode()
{
    OsgGroupVisNode::InitializeNodeDesc();
    d = new OsgGroupVisNodeData;

	d->outputGroup = new osg::Group;
}

OsgGroupVisNode::~OsgGroupVisNode()
{
    // This would be a good time to delete the backend object. If the backend object is a vtkObject
    // subclass, then you have to delete it now.
    // d->VtkObjectPointer->Delete();

    delete d;
}


bool OsgGroupVisNode::hasInput(IVisSystemNodeConnectionPath* path)
{
    if(!path)
        return false;

    /*
    If you have added input paths in the description block at the header of this file,
    then you will have to handle inputs here
    */

	if (path->pathName() == "OsgNodeInput")
		return d->outputGroup && d->outputGroup->referenceCount() > 0;

	return CGenericVisNodeBase::hasInput(path);
}

bool OsgGroupVisNode::setInput(IVisSystemNodeConnectionPath* path, IVisSystemNodeIOData* inputData)
{
    if(!path || !inputData)
        return false;

    /*
    If you have added input paths in the description block at the header of this file,
    then you will have to handle inputs here
    */
	if (path->pathName() == "OsgNodeInput")
	{
		OsgGeodeFromActorVisNodeIOData * geodeData = 0;
		OsgGroupVisNodeIOData * groupData = 0;
		bool success = false;

		if (success = inputData->queryInterface("OsgGeodeFromActorVisNodeIOData", (void**)&geodeData)
			&& geodeData)
			d->outputGroup->addChild(geodeData->getOsgGeode().get());
		else if (success = inputData->queryInterface("OsgGroupVisNodeIOData", (void**)&groupData)
			&& groupData)
			d->outputGroup->addChild(groupData->getOsgGroup().get());

		if (success)
				return true;
	}

	return CGenericVisNodeBase::setInput(path, inputData);
}

bool OsgGroupVisNode::removeInput(IVisSystemNodeConnectionPath* path, IVisSystemNodeIOData* inputData)
{
    if(!path || !inputData)
        return false;

    /*
    If you have added input paths in the description block at the header of this file,
    then you will have to handle inputs here
    */
	if (path->pathName() == "OsgNodeInput")
	{
		OsgGeodeFromActorVisNodeIOData * geodeData = 0;
		OsgGroupVisNodeIOData * groupData = 0;
		bool success = false;

		if (success = inputData->queryInterface("OsgGeodeFromActorVisNodeIOData", (void**)&geodeData)
			&& geodeData)
			d->outputGroup->removeChild((osg::ref_ptr<osg::Node>) geodeData->getOsgGeode().get());
		else if (success = inputData->queryInterface("OsgGroupVisNodeIOData", (void**)&groupData)
			&& groupData)
			d->outputGroup->removeChild((osg::ref_ptr<osg::Node>) groupData->getOsgGroup().get());

		if (success)
		{
				d->outputGroupData.setOsgGroup(d->outputGroup);
				return true;
		}
	}

    return CGenericVisNodeBase::removeInput(path, inputData);
}

bool OsgGroupVisNode::fetchOutput(IVisSystemNodeConnectionPath* path, IVisSystemNodeIOData** outputData)
{
    if(!path || !outputData)
        return false;

    /*
    If you have added output paths in the description block at the header of this file,
    then you will have to handle outputs here
    */
	if (path->pathName() == "OsgGroup")
	{
		d->outputGroupData.setOsgGroup(d->outputGroup);
		*outputData = &d->outputGroupData;
		return true;
	}

	return CGenericVisNodeBase::fetchOutput(path, outputData);
}

bool OsgGroupVisNode::outputDerefed(IVisSystemNodeConnectionPath* path, IVisSystemNodeIOData* outputData)
{
    if(!path || !outputData)
        return false;

    /*
    If you have added output paths in the description block at the header of this file,
    then you will have to handle outputs here
    */
	if (path->pathName() == "OsgGroup")
		return true;

    return CGenericVisNodeBase::outputDerefed(path, outputData);
}

#ifdef ENABLE_ADVANCED_PROPERTIES

int OsgGroupVisNode::propertyCount()
{
    return 0;
}

QString OsgGroupVisNode::propertyName(int index)
{
    Q_UNUSED(index);
    return QString();
}

QString OsgGroupVisNode::propertyGroup(int index)
{
    Q_UNUSED(index);
    return QString();
}

QVariant OsgGroupVisNode::propertyValue(int index)
{
    Q_UNUSED(index);
    return QVariant();
}

void OsgGroupVisNode::setPropertyValue(int index, QVariant value)
{
    Q_UNUSED(index);
    Q_UNUSED(value);
}

bool OsgGroupVisNode::hasEditor(int index)
{
    Q_UNUSED(index);
    return false;
}

QWidget* OsgGroupVisNode::createEditor(int index)
{
    Q_UNUSED(index);
    return 0;
}

void OsgGroupVisNode::setEditorValue(int index, QWidget* widget, QVariant value)
{
    Q_UNUSED(index);
    Q_UNUSED(widget);
    Q_UNUSED(value);
}

QVariant OsgGroupVisNode::editorValue(int index, QWidget* widget)
{
    Q_UNUSED(index);
    Q_UNUSED(widget);
}

void OsgGroupVisNode::connectEditor(int index, QWidget* widget, QObject* receiver, const char* member)
{
    Q_UNUSED(index);
    Q_UNUSED(widget);
    Q_UNUSED(receiver);
    Q_UNUSED(member);
}

QString OsgGroupVisNode::propertyValueString(int index)
{
    Q_UNUSED(index);
    return QString();
}

QVariant OsgGroupVisNode::propertyValueFromString(int index, QString valueStr)
{
    Q_UNUSED(index);
    Q_UNUSED(valueStr);

    return QVariant();
}

bool OsgGroupVisNode::canLoadSaveProperty(int index)
{
    Q_UNUSED(index);

    return false;
}

#endif





