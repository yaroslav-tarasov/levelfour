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

#include "OsgCapsuleVisNode.h"
#include "OsgCoreComponent.h"
#include "OsgNodeVisNodeIOData.h"
#include <osg/ShapeDrawable>
#include "OsgDrawableVisNodeIOData.h"


DEFINE_VIS_NODE(OsgCapsuleVisNode, CGenericVisNodeBase)
{
    pDesc->setNodeClassCategory("Primitives");
    pDesc->setNodeClassName("Capsule");
    pDesc->setNodeClassDescription("The geometry of a Capsule primitive.");
    pDesc->setNodeIcon( OsgCoreComponent::instance().nodeIcon() );

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

struct OsgCapsuleVisNodeData
{
    OsgCapsuleVisNodeData() : outputDrawable(0) {}
	osg::ShapeDrawable * outputDrawable;
	OsgDrawableVisNodeIOData outputDrawableData;
};

OsgCapsuleVisNode::OsgCapsuleVisNode()
{
    OsgCapsuleVisNode::InitializeNodeDesc();
    d = new OsgCapsuleVisNodeData;

	d->outputDrawableData.setDrawable(d->outputDrawable = new osg::ShapeDrawable(new osg::Capsule));
    
}

OsgCapsuleVisNode::~OsgCapsuleVisNode()
{
    // This would be a good time to delete the backend object. If the backend object is a vtkObject
    // subclass, then you have to delete it now.
    // d->VtkObjectPointer->Delete();

    
    delete d;
}

bool OsgCapsuleVisNode::hasInput(IVisSystemNodeConnectionPath* path)
{
    if(!path)
        return false;

    /*
    If you have added input paths in the description block at the header of this file,
    then you will have to handle inputs here
    */

    return CGenericVisNodeBase::hasInput(path);
}

bool OsgCapsuleVisNode::setInput(IVisSystemNodeConnectionPath* path, IVisSystemNodeIOData* inputData)
{
    if(!path || !inputData)
        return false;

    /*
    If you have added input paths in the description block at the header of this file,
    then you will have to handle inputs here
    */

    return CGenericVisNodeBase::setInput(path, inputData);
}

bool OsgCapsuleVisNode::removeInput(IVisSystemNodeConnectionPath* path, IVisSystemNodeIOData* inputData)
{
    if(!path || !inputData)
        return false;

    /*
    If you have added input paths in the description block at the header of this file,
    then you will have to handle inputs here
    */

    return CGenericVisNodeBase::removeInput(path, inputData);
}

bool OsgCapsuleVisNode::fetchOutput(IVisSystemNodeConnectionPath* path, IVisSystemNodeIOData** outputData)
{
    if (path->pathName() == "OsgDrawable")
	{
		*outputData = &d->outputDrawableData;
		return true;
	}
    return CGenericVisNodeBase::fetchOutput(path, outputData);
}

bool OsgCapsuleVisNode::outputDerefed(IVisSystemNodeConnectionPath* path, IVisSystemNodeIOData* outputData)
{
    if(!path || !outputData)
        return false;

    /*
    If you have added output paths in the description block at the header of this file,
    then you will have to handle outputs here
    */
	if (path->pathName() == "OsgCapsule")
		return true;

    return CGenericVisNodeBase::outputDerefed(path, outputData);
}

#ifdef ENABLE_ADVANCED_PROPERTIES

int OsgCapsuleVisNode::propertyCount()
{
    return 0;
}

QString OsgCapsuleVisNode::propertyName(int index)
{
    Q_UNUSED(index);
    return QString();
}

QString OsgCapsuleVisNode::propertyGroup(int index)
{
    Q_UNUSED(index);
    return QString();
}

QVariant OsgCapsuleVisNode::propertyValue(int index)
{
    Q_UNUSED(index);
    return QVariant();
}

void OsgCapsuleVisNode::setPropertyValue(int index, QVariant value)
{
    Q_UNUSED(index);
    Q_UNUSED(value);
}

bool OsgCapsuleVisNode::hasEditor(int index)
{
    Q_UNUSED(index);
    return false;
}

QWidget* OsgCapsuleVisNode::createEditor(int index)
{
    Q_UNUSED(index);
    return 0;
}

void OsgCapsuleVisNode::setEditorValue(int index, QWidget* widget, QVariant value)
{
    Q_UNUSED(index);
    Q_UNUSED(widget);
    Q_UNUSED(value);
}

QVariant OsgCapsuleVisNode::editorValue(int index, QWidget* widget)
{
    Q_UNUSED(index);
    Q_UNUSED(widget);
}

void OsgCapsuleVisNode::connectEditor(int index, QWidget* widget, QObject* receiver, const char* member)
{
    Q_UNUSED(index);
    Q_UNUSED(widget);
    Q_UNUSED(receiver);
    Q_UNUSED(member);
}

QString OsgCapsuleVisNode::propertyValueString(int index)
{
    Q_UNUSED(index);
    return QString();
}

QVariant OsgCapsuleVisNode::propertyValueFromString(int index, QString valueStr)
{
    Q_UNUSED(index);
    Q_UNUSED(valueStr);

    return QVariant();
}

bool OsgCapsuleVisNode::canLoadSaveProperty(int index)
{
    Q_UNUSED(index);

    return false;
}

#endif





