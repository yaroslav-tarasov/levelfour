/****************************************************************************
**
** Copyright (C) L4
**
** Use of this file is limited according to the terms specified by
** L4.
**
** Details of those terms are listed in licence.txt included as
** part of the distribution package of this file. This file may not
** be distributed without including the licence.txt file.
**
** This file is provided AS IS with NO WARRANTY OF ANY KIND, INCLUDING THE
** WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
**
****************************************************************************/

#include "OsgInfinitePlaneVisNode.h"
#include "OsgCoreComponent.h"
#include "OsgNodeVisNodeIOData.h"
#include <osg/ShapeDrawable>
#include "OsgDrawableVisNodeIOData.h"


DEFINE_VIS_NODE(OsgInfinitePlaneVisNode, CGenericVisNodeBase)
{
    pDesc->setNodeClassCategory("Primitives");
    pDesc->setNodeClassName("InfinitePlane");
    pDesc->setNodeClassDescription("Create an Infinite Plane primitive.");
    pDesc->setNodeIcon( QIcon(":/OsgCore/InfinitePlane.png") );

    // Uncomment and use the following code template to add input/output paths
    pDesc->addConnectionPath(
        new CGenericVisNodeConnectionPath(
                "OsgDrawable",                                 // Name of the path
                IVisSystemNodeConnectionPath::OutputPath,   // Path type can be OutputPath or InputPath
				"osg::Drawable",                                 // Data type of the path
                0,                                          // Path index (don't change)
                false                                       // Allow Multiple Inputs Flag
            )
        );
}

struct OsgInfinitePlaneVisNodeData
{
    OsgInfinitePlaneVisNodeData() : outputDrawable(0) {}
	osg::ShapeDrawable * outputDrawable;
	OsgDrawableVisNodeIOData outputDrawableData;
};

OsgInfinitePlaneVisNode::OsgInfinitePlaneVisNode()
{
    OsgInfinitePlaneVisNode::InitializeNodeDesc();
    d = new OsgInfinitePlaneVisNodeData;

	d->outputDrawableData.setDrawable(d->outputDrawable = new osg::ShapeDrawable(new osg::InfinitePlane));

    
}

OsgInfinitePlaneVisNode::~OsgInfinitePlaneVisNode()
{
    // This would be a good time to delete the backend object. If the backend object is a vtkObject
    // subclass, then you have to delete it now.
    // d->VtkObjectPointer->Delete();

    
    delete d;
}

bool OsgInfinitePlaneVisNode::hasInput(IVisSystemNodeConnectionPath* path)
{
    if(!path)
        return false;

    /*
    If you have added input paths in the description block at the header of this file,
    then you will have to handle inputs here
    */

    return CGenericVisNodeBase::hasInput(path);
}

bool OsgInfinitePlaneVisNode::setInput(IVisSystemNodeConnectionPath* path, IVisSystemNodeIOData* inputData)
{
    if(!path || !inputData)
        return false;

    /*
    If you have added input paths in the description block at the header of this file,
    then you will have to handle inputs here
    */

    return CGenericVisNodeBase::setInput(path, inputData);
}

bool OsgInfinitePlaneVisNode::removeInput(IVisSystemNodeConnectionPath* path, IVisSystemNodeIOData* inputData)
{
    if(!path || !inputData)
        return false;

    /*
    If you have added input paths in the description block at the header of this file,
    then you will have to handle inputs here
    */

    return CGenericVisNodeBase::removeInput(path, inputData);
}

bool OsgInfinitePlaneVisNode::fetchOutput(IVisSystemNodeConnectionPath* path, IVisSystemNodeIOData** outputData)
{
    if(!path || !outputData)
        return false;

    /*
    If you have added output paths in the description block at the header of this file,
    then you will have to handle outputs here
    */
	if (path->pathName() == "OsgDrawable")
	{
		*outputData = &d->outputDrawableData;
		return true;
	}
    return CGenericVisNodeBase::fetchOutput(path, outputData);
}

bool OsgInfinitePlaneVisNode::outputDerefed(IVisSystemNodeConnectionPath* path, IVisSystemNodeIOData* outputData)
{
    if(!path || !outputData)
        return false;

    /*
    If you have added output paths in the description block at the header of this file,
    then you will have to handle outputs here
    */
	if (path->pathName() == "OsgCone")
		return true;

    return CGenericVisNodeBase::outputDerefed(path, outputData);
}

#ifdef ENABLE_ADVANCED_PROPERTIES

int OsgInfinitePlaneVisNode::propertyCount()
{
    return 0;
}

QString OsgInfinitePlaneVisNode::propertyName(int index)
{
    Q_UNUSED(index);
    return QString();
}

QString OsgInfinitePlaneVisNode::propertyGroup(int index)
{
    Q_UNUSED(index);
    return QString();
}

QVariant OsgInfinitePlaneVisNode::propertyValue(int index)
{
    Q_UNUSED(index);
    return QVariant();
}

void OsgInfinitePlaneVisNode::setPropertyValue(int index, QVariant value)
{
    Q_UNUSED(index);
    Q_UNUSED(value);
}

bool OsgInfinitePlaneVisNode::hasEditor(int index)
{
    Q_UNUSED(index);
    return false;
}

QWidget* OsgInfinitePlaneVisNode::createEditor(int index)
{
    Q_UNUSED(index);
    return 0;
}

void OsgInfinitePlaneVisNode::setEditorValue(int index, QWidget* widget, QVariant value)
{
    Q_UNUSED(index);
    Q_UNUSED(widget);
    Q_UNUSED(value);
}

QVariant OsgInfinitePlaneVisNode::editorValue(int index, QWidget* widget)
{
    Q_UNUSED(index);
    Q_UNUSED(widget);
}

void OsgInfinitePlaneVisNode::connectEditor(int index, QWidget* widget, QObject* receiver, const char* member)
{
    Q_UNUSED(index);
    Q_UNUSED(widget);
    Q_UNUSED(receiver);
    Q_UNUSED(member);
}

QString OsgInfinitePlaneVisNode::propertyValueString(int index)
{
    Q_UNUSED(index);
    return QString();
}

QVariant OsgInfinitePlaneVisNode::propertyValueFromString(int index, QString valueStr)
{
    Q_UNUSED(index);
    Q_UNUSED(valueStr);

    return QVariant();
}

bool OsgInfinitePlaneVisNode::canLoadSaveProperty(int index)
{
    Q_UNUSED(index);

    return false;
}

#endif





