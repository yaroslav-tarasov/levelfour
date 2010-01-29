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

#include "OsgGeodeVisNode.h"
#include "OsgCoreComponent.h"
#include <osg/Geode>
#include <osg/Drawable>
#include "OsgDrawableVisNodeIOData.h"
#include "OsgNodeVisNodeIOData.h"

DEFINE_VIS_NODE(OsgGeodeVisNode, CGenericVisNodeBase)
{
    pDesc->setNodeClassCategory("OsgCore");
    pDesc->setNodeClassName("Geode");
    pDesc->setNodeClassDescription("Geode");
    pDesc->setNodeIcon( OsgCoreComponent::instance().nodeIcon() );

    // Uncomment and use the following code template to add input/output paths
    
    pDesc->addConnectionPath(
        new CGenericVisNodeConnectionPath(
                "OsgDrawable",                                 // Name of the path
                IVisSystemNodeConnectionPath::InputPath,   // Path type can be OutputPath or InputPath
				"osg::Drawable",                                 // Data type of the path
                0,                                          // Path index (don't change)
                true                                       // Allow Multiple Inputs Flag
            )
        );
    
    pDesc->addConnectionPath(
        new CGenericVisNodeConnectionPath(
                "OsgGeode",                                 // Name of the path
                IVisSystemNodeConnectionPath::OutputPath,   // Path type can be OutputPath or InputPath
                "osg::ref_ptr<osg::Geode>",                                 // Data type of the path
                0,                                          // Path index (don't change)
                false                                       // Allow Multiple Inputs Flag
            )
        );
}

struct OsgGeodeVisNodeData
{
	OsgGeodeVisNodeData() : inputDrawable(0), outputGeode(0) {}
	osg::Drawable * inputDrawable;
	osg::Geode * outputGeode;
	OsgNodeVisNodeIOData outputGeodeData;
};

OsgGeodeVisNode::OsgGeodeVisNode()
{
    OsgGeodeVisNode::InitializeNodeDesc();
    d = new OsgGeodeVisNodeData;
	
	d->outputGeode = new osg::Geode;
	d->outputGeodeData.setOsgNode(d->outputGeode);
}

OsgGeodeVisNode::~OsgGeodeVisNode()
{
    // This would be a good time to delete the backend object. If the backend object is a vtkObject
    // subclass, then you have to delete it now.
    // d->VtkObjectPointer->Delete();

    
    delete d;
}

bool OsgGeodeVisNode::hasInput(IVisSystemNodeConnectionPath* path)
{
    if(!path)
        return false;

    /*
    If you have added input paths in the description block at the header of this file,
    then you will have to handle inputs here
    */

	if (path->pathName() == "OsgDrawable")
		return d->outputGeode && d->outputGeode->getNumDrawables() > 0;

    return CGenericVisNodeBase::hasInput(path);
}

bool OsgGeodeVisNode::setInput(IVisSystemNodeConnectionPath* path, IVisSystemNodeIOData* inputData)
{
    if(!path || !inputData)
        return false;

    /*
    If you have added input paths in the description block at the header of this file,
    then you will have to handle inputs here
    */
	if (path->pathName() == "OsgDrawable")
	{
		OsgDrawableVisNodeIOData * drawableData = 0;
		bool success = inputData->queryInterface("OsgDrawableVisNodeIOData", (void**)&drawableData);
		if (success && drawableData)
		{
			d->outputGeode->addDrawable(drawableData->getOsgDrawable());
			return true;
		}
	}

    return CGenericVisNodeBase::setInput(path, inputData);
}

bool OsgGeodeVisNode::removeInput(IVisSystemNodeConnectionPath* path, IVisSystemNodeIOData* inputData)
{
    if(!path || !inputData)
        return false;

    /*
    If you have added input paths in the description block at the header of this file,
    then you will have to handle inputs here
    */
	if (path->pathName() == "OsgDrawable")
	{
		OsgDrawableVisNodeIOData * drawableData = 0;
		bool success = inputData->queryInterface("OsgDrawableVisNodeIOData", (void**)&drawableData);
		if (success && drawableData)
		{
			d->outputGeode->removeDrawable(drawableData->getOsgDrawable());
			return true;
		}
	}

    return CGenericVisNodeBase::removeInput(path, inputData);
}

bool OsgGeodeVisNode::fetchOutput(IVisSystemNodeConnectionPath* path, IVisSystemNodeIOData** outputData)
{
    if(!path || !outputData)
        return false;

    /*
    If you have added output paths in the description block at the header of this file,
    then you will have to handle outputs here
    */
	if (path->pathName() == "OsgGeode")
	{
		d->outputGeodeData.setOsgNode(d->outputGeode);
		*outputData = &d->outputGeodeData;
		return true;
	}


    return CGenericVisNodeBase::fetchOutput(path, outputData);
}

bool OsgGeodeVisNode::outputDerefed(IVisSystemNodeConnectionPath* path, IVisSystemNodeIOData* outputData)
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

#ifdef ENABLE_ADVANCED_PROPERTIES

int OsgGeodeVisNode::propertyCount()
{
    return 0;
}

QString OsgGeodeVisNode::propertyName(int index)
{
    Q_UNUSED(index);
    return QString();
}

QString OsgGeodeVisNode::propertyGroup(int index)
{
    Q_UNUSED(index);
    return QString();
}

QVariant OsgGeodeVisNode::propertyValue(int index)
{
    Q_UNUSED(index);
    return QVariant();
}

void OsgGeodeVisNode::setPropertyValue(int index, QVariant value)
{
    Q_UNUSED(index);
    Q_UNUSED(value);
}

bool OsgGeodeVisNode::hasEditor(int index)
{
    Q_UNUSED(index);
    return false;
}

QWidget* OsgGeodeVisNode::createEditor(int index)
{
    Q_UNUSED(index);
    return 0;
}

void OsgGeodeVisNode::setEditorValue(int index, QWidget* widget, QVariant value)
{
    Q_UNUSED(index);
    Q_UNUSED(widget);
    Q_UNUSED(value);
}

QVariant OsgGeodeVisNode::editorValue(int index, QWidget* widget)
{
    Q_UNUSED(index);
    Q_UNUSED(widget);
}

void OsgGeodeVisNode::connectEditor(int index, QWidget* widget, QObject* receiver, const char* member)
{
    Q_UNUSED(index);
    Q_UNUSED(widget);
    Q_UNUSED(receiver);
    Q_UNUSED(member);
}

QString OsgGeodeVisNode::propertyValueString(int index)
{
    Q_UNUSED(index);
    return QString();
}

QVariant OsgGeodeVisNode::propertyValueFromString(int index, QString valueStr)
{
    Q_UNUSED(index);
    Q_UNUSED(valueStr);

    return QVariant();
}

bool OsgGeodeVisNode::canLoadSaveProperty(int index)
{
    Q_UNUSED(index);

    return false;
}

#endif





