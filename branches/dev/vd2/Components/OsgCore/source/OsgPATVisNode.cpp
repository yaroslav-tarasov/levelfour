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

#include "OsgPATVisNode.h"
#include "OsgCoreComponent.h"
#include <osg/PositionAttitudeTransform>
#include "OsgPATVisNodeIOData.h"
#include "OsgGeodeFromActorVisNodeIOData.h"

DEFINE_VIS_NODE(OsgPATVisNode, CGenericVisNodeBase)
{
    pDesc->setNodeClassCategory("Actors");
    pDesc->setNodeClassName("PAT");
    pDesc->setNodeClassDescription("PAT");
    pDesc->setNodeIcon( QIcon(":/OsgCore/PAT.png") );

    // Uncomment and use the following code template to add input/output paths
    pDesc->addConnectionPath(
        new CGenericVisNodeConnectionPath(
                "OsgPAT",                                 // Name of the path
                IVisSystemNodeConnectionPath::OutputPath,   // Path type can be OutputPath or InputPath
                "osg::ref_ptr<osg::PositionAttitudeTransform>",                                 // Data type of the path
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

struct OsgPATVisNodeData
{
	OsgPATVisNodeData() : outputPAT(0) { }
	osg::ref_ptr<osg::PositionAttitudeTransform> outputPAT;
	OsgPATVisNodeIOData outputPATData;
};

OsgPATVisNode::OsgPATVisNode()
{
    OsgPATVisNode::InitializeNodeDesc();
    d = new OsgPATVisNodeData;

	d->outputPAT = new osg::PositionAttitudeTransform;
	_visible = true;
}

OsgPATVisNode::~OsgPATVisNode()
{
    // This would be a good time to delete the backend object. If the backend object is a vtkObject
    // subclass, then you have to delete it now.
    // d->VtkObjectPointer->Delete();

    
    delete d;
}

bool OsgPATVisNode::getVisibility() const
{
	return _visible;
}

void OsgPATVisNode::setVisibility(bool visible)
{
	if (_visible != visible)
	{
		_visible = visible;

		if (!d->outputPAT)
			return;

		if (_visible)
			d->outputPAT->setNodeMask(1);
		else 
			d->outputPAT->setNodeMask(0);
	}
}

double OsgPATVisNode::posX() const
{
	osg::Vec3 pos = d->outputPAT->getPosition();
	return pos.x();
}

void OsgPATVisNode::setPosX(double l)
{
	osg::Vec3 pos = d->outputPAT->getPosition();
	d->outputPAT->setPosition(osg::Vec3(l, pos.y(), pos.z()));
}

double OsgPATVisNode::posY() const
{
	osg::Vec3 pos = d->outputPAT->getPosition();
	return pos.y();
}

void OsgPATVisNode::setPosY(double l)
{
	osg::Vec3 pos = d->outputPAT->getPosition();
	d->outputPAT->setPosition(osg::Vec3(pos.x(), l, pos.z()));
}

double OsgPATVisNode::posZ() const
{
	osg::Vec3 pos = d->outputPAT->getPosition();
	return pos.z();
}

void OsgPATVisNode::setPosZ(double l)
{
	osg::Vec3 pos = d->outputPAT->getPosition();
	d->outputPAT->setPosition(osg::Vec3(pos.x(), pos.y(), l));
}

bool OsgPATVisNode::hasInput(IVisSystemNodeConnectionPath* path)
{
    if(!path)
        return false;

    /*
    If you have added input paths in the description block at the header of this file,
    then you will have to handle inputs here
    */
	if (path->pathName() == "OsgNodeInput")
		return d->outputPAT && d->outputPAT->referenceCount() > 0;

    return CGenericVisNodeBase::hasInput(path);
}

bool OsgPATVisNode::setInput(IVisSystemNodeConnectionPath* path, IVisSystemNodeIOData* inputData)
{
    if(!path || !inputData)
        return false;

    /*
    If you have added input paths in the description block at the header of this file,
    then you will have to handle inputs here
    */
	if (path->pathName() == "OsgNodeInput")
	{
		OsgPATVisNodeIOData * patData = 0;
		bool success = false;

		// UNCOMMENT FOLLOWING TO INTEGRATE VTK
/*		OsgGeodeFromActorVisNodeIOData * geodeData = 0;
		if (success = inputData->queryInterface("OsgGeodeFromActorVisNodeIOData", (void**)&geodeData)
			&& geodeData)
			d->outputPAT->addChild(geodeData->getOsgGeode().get());
		else */ 
		
		if (success = inputData->queryInterface("OsgNodeVisNodeIOData", (void**)&patData)
			&& patData)
			d->outputPAT->addChild(patData->getOsgNode());

		if (success)
				return true;
	}

    return CGenericVisNodeBase::setInput(path, inputData);
}

bool OsgPATVisNode::removeInput(IVisSystemNodeConnectionPath* path, IVisSystemNodeIOData* inputData)
{
    if(!path || !inputData)
        return false;

    /*
    If you have added input paths in the description block at the header of this file,
    then you will have to handle inputs here
    */
	if (path->pathName() == "OsgNodeInput")
	{
		OsgPATVisNodeIOData * patData = 0;
		bool success = false;

		// UNCOMMENT FOLLOWING TO INTEGRATE VTK
/*		OsgGeodeFromActorVisNodeIOData * geodeData = 0;
		if (success = inputData->queryInterface("OsgGeodeFromActorVisNodeIOData", (void**)&geodeData)
			&& geodeData)
			d->outputPAT->removeChild((osg::ref_ptr<osg::Node>) geodeData->getOsgGeode().get());
		else */ 
		
		if (success = inputData->queryInterface("OsgNodeVisNodeIOData", (void**)&patData)
			&& patData)
			d->outputPAT->removeChild((osg::Node *) patData->getOsgNode());

		if (success)
		{
				d->outputPATData.setOsgNode(d->outputPAT);
				return true;
		}
	}
    return CGenericVisNodeBase::removeInput(path, inputData);
}

bool OsgPATVisNode::fetchOutput(IVisSystemNodeConnectionPath* path, IVisSystemNodeIOData** outputData)
{
    if(!path || !outputData)
        return false;

    /*
    If you have added output paths in the description block at the header of this file,
    then you will have to handle outputs here
    */
	if (path->pathName() == "OsgPAT")
	{
		d->outputPATData.setOsgNode(d->outputPAT);
		*outputData = &d->outputPATData;
		return true;
	}
    return CGenericVisNodeBase::fetchOutput(path, outputData);
}

bool OsgPATVisNode::outputDerefed(IVisSystemNodeConnectionPath* path, IVisSystemNodeIOData* outputData)
{
    if(!path || !outputData)
        return false;

    /*
    If you have added output paths in the description block at the header of this file,
    then you will have to handle outputs here
    */
	if (path->pathName() == "OsgPAT")
		return true;

    return CGenericVisNodeBase::outputDerefed(path, outputData);
}

#ifdef ENABLE_ADVANCED_PROPERTIES

int OsgPATVisNode::propertyCount()
{
    return 0;
}

QString OsgPATVisNode::propertyName(int index)
{
    Q_UNUSED(index);
    return QString();
}

QString OsgPATVisNode::propertyGroup(int index)
{
    Q_UNUSED(index);
    return QString();
}

QVariant OsgPATVisNode::propertyValue(int index)
{
    Q_UNUSED(index);
    return QVariant();
}

void OsgPATVisNode::setPropertyValue(int index, QVariant value)
{
    Q_UNUSED(index);
    Q_UNUSED(value);
}

bool OsgPATVisNode::hasEditor(int index)
{
    Q_UNUSED(index);
    return false;
}

QWidget* OsgPATVisNode::createEditor(int index)
{
    Q_UNUSED(index);
    return 0;
}

void OsgPATVisNode::setEditorValue(int index, QWidget* widget, QVariant value)
{
    Q_UNUSED(index);
    Q_UNUSED(widget);
    Q_UNUSED(value);
}

QVariant OsgPATVisNode::editorValue(int index, QWidget* widget)
{
    Q_UNUSED(index);
    Q_UNUSED(widget);
}

void OsgPATVisNode::connectEditor(int index, QWidget* widget, QObject* receiver, const char* member)
{
    Q_UNUSED(index);
    Q_UNUSED(widget);
    Q_UNUSED(receiver);
    Q_UNUSED(member);
}

QString OsgPATVisNode::propertyValueString(int index)
{
    Q_UNUSED(index);
    return QString();
}

QVariant OsgPATVisNode::propertyValueFromString(int index, QString valueStr)
{
    Q_UNUSED(index);
    Q_UNUSED(valueStr);

    return QVariant();
}

bool OsgPATVisNode::canLoadSaveProperty(int index)
{
    Q_UNUSED(index);

    return false;
}

#endif





