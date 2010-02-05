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

#include "OsgCameraVisNode.h"
#include "OsgCoreComponent.h"


DEFINE_VIS_NODE(OsgCameraVisNode, CGenericVisNodeBase)
{
    pDesc->setNodeClassCategory("Scene");
    pDesc->setNodeClassName("Camera");
    pDesc->setNodeClassDescription("The settings of a Camera for a group");
    pDesc->setNodeIcon( QIcon(":/OsgCore/Camera.png") );

    // Uncomment and use the following code template to add input/output paths
    /*
    pDesc->addConnectionPath(
        new CGenericVisNodeConnectionPath(
                "PathName",                                 // Name of the path
                IVisSystemNodeConnectionPath::OutputPath,   // Path type can be OutputPath or InputPath
                "PathType",                                 // Data type of the path
                0,                                          // Path index (don't change)
                false                                       // Allow Multiple Inputs Flag
            )
        );
    */
}

struct OsgCameraVisNodeData
{
    
};

OsgCameraVisNode::OsgCameraVisNode()
{
    OsgCameraVisNode::InitializeNodeDesc();
    d = new OsgCameraVisNodeData;

    
}

OsgCameraVisNode::~OsgCameraVisNode()
{
    // This would be a good time to delete the backend object. If the backend object is a vtkObject
    // subclass, then you have to delete it now.
    // d->VtkObjectPointer->Delete();

    
    delete d;
}

bool OsgCameraVisNode::hasInput(IVisSystemNodeConnectionPath* path)
{
    if(!path)
        return false;

    /*
    If you have added input paths in the description block at the header of this file,
    then you will have to handle inputs here
    */

    return CGenericVisNodeBase::hasInput(path);
}

bool OsgCameraVisNode::setInput(IVisSystemNodeConnectionPath* path, IVisSystemNodeIOData* inputData)
{
    if(!path || !inputData)
        return false;

    /*
    If you have added input paths in the description block at the header of this file,
    then you will have to handle inputs here
    */

    return CGenericVisNodeBase::setInput(path, inputData);
}

bool OsgCameraVisNode::removeInput(IVisSystemNodeConnectionPath* path, IVisSystemNodeIOData* inputData)
{
    if(!path || !inputData)
        return false;

    /*
    If you have added input paths in the description block at the header of this file,
    then you will have to handle inputs here
    */

    return CGenericVisNodeBase::removeInput(path, inputData);
}

bool OsgCameraVisNode::fetchOutput(IVisSystemNodeConnectionPath* path, IVisSystemNodeIOData** outputData)
{
    if(!path || !outputData)
        return false;

    /*
    If you have added output paths in the description block at the header of this file,
    then you will have to handle outputs here
    */

    return CGenericVisNodeBase::fetchOutput(path, outputData);
}

bool OsgCameraVisNode::outputDerefed(IVisSystemNodeConnectionPath* path, IVisSystemNodeIOData* outputData)
{
    if(!path || !outputData)
        return false;

    /*
    If you have added output paths in the description block at the header of this file,
    then you will have to handle outputs here
    */

    return CGenericVisNodeBase::outputDerefed(path, outputData);
}

#ifdef ENABLE_ADVANCED_PROPERTIES

int OsgCameraVisNode::propertyCount()
{
    return 0;
}

QString OsgCameraVisNode::propertyName(int index)
{
    Q_UNUSED(index);
    return QString();
}

QString OsgCameraVisNode::propertyGroup(int index)
{
    Q_UNUSED(index);
    return QString();
}

QVariant OsgCameraVisNode::propertyValue(int index)
{
    Q_UNUSED(index);
    return QVariant();
}

void OsgCameraVisNode::setPropertyValue(int index, QVariant value)
{
    Q_UNUSED(index);
    Q_UNUSED(value);
}

bool OsgCameraVisNode::hasEditor(int index)
{
    Q_UNUSED(index);
    return false;
}

QWidget* OsgCameraVisNode::createEditor(int index)
{
    Q_UNUSED(index);
    return 0;
}

void OsgCameraVisNode::setEditorValue(int index, QWidget* widget, QVariant value)
{
    Q_UNUSED(index);
    Q_UNUSED(widget);
    Q_UNUSED(value);
}

QVariant OsgCameraVisNode::editorValue(int index, QWidget* widget)
{
    Q_UNUSED(index);
    Q_UNUSED(widget);
}

void OsgCameraVisNode::connectEditor(int index, QWidget* widget, QObject* receiver, const char* member)
{
    Q_UNUSED(index);
    Q_UNUSED(widget);
    Q_UNUSED(receiver);
    Q_UNUSED(member);
}

QString OsgCameraVisNode::propertyValueString(int index)
{
    Q_UNUSED(index);
    return QString();
}

QVariant OsgCameraVisNode::propertyValueFromString(int index, QString valueStr)
{
    Q_UNUSED(index);
    Q_UNUSED(valueStr);

    return QVariant();
}

bool OsgCameraVisNode::canLoadSaveProperty(int index)
{
    Q_UNUSED(index);

    return false;
}

#endif





