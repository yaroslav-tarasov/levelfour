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

#include "OsgCartoonVisNode.h"
#include "OsgFXComponent.h"


DEFINE_VIS_NODE(OsgCartoonVisNode, CGenericVisNodeBase)
{
    pDesc->setNodeClassCategory("OsgFX");
    pDesc->setNodeClassName("OsgCartoon");
    pDesc->setNodeClassDescription("OsgCartoon");
    pDesc->setNodeIcon( OsgFXComponent::instance().nodeIcon() );

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

struct OsgCartoonVisNodeData
{
    
};

OsgCartoonVisNode::OsgCartoonVisNode()
{
    OsgCartoonVisNode::InitializeNodeDesc();
    d = new OsgCartoonVisNodeData;

    
}

OsgCartoonVisNode::~OsgCartoonVisNode()
{
    // This would be a good time to delete the backend object. If the backend object is a vtkObject
    // subclass, then you have to delete it now.
    // d->VtkObjectPointer->Delete();

    
    delete d;
}

bool OsgCartoonVisNode::hasInput(IVisSystemNodeConnectionPath* path)
{
    if(!path)
        return false;

    /*
    If you have added input paths in the description block at the header of this file,
    then you will have to handle inputs here
    */

    return CGenericVisNodeBase::hasInput(path);
}

bool OsgCartoonVisNode::setInput(IVisSystemNodeConnectionPath* path, IVisSystemNodeIOData* inputData)
{
    if(!path || !inputData)
        return false;

    /*
    If you have added input paths in the description block at the header of this file,
    then you will have to handle inputs here
    */

    return CGenericVisNodeBase::setInput(path, inputData);
}

bool OsgCartoonVisNode::removeInput(IVisSystemNodeConnectionPath* path, IVisSystemNodeIOData* inputData)
{
    if(!path || !inputData)
        return false;

    /*
    If you have added input paths in the description block at the header of this file,
    then you will have to handle inputs here
    */

    return CGenericVisNodeBase::removeInput(path, inputData);
}

bool OsgCartoonVisNode::fetchOutput(IVisSystemNodeConnectionPath* path, IVisSystemNodeIOData** outputData)
{
    if(!path || !outputData)
        return false;

    /*
    If you have added output paths in the description block at the header of this file,
    then you will have to handle outputs here
    */

    return CGenericVisNodeBase::fetchOutput(path, outputData);
}

bool OsgCartoonVisNode::outputDerefed(IVisSystemNodeConnectionPath* path, IVisSystemNodeIOData* outputData)
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

int OsgCartoonVisNode::propertyCount()
{
    return 0;
}

QString OsgCartoonVisNode::propertyName(int index)
{
    Q_UNUSED(index);
    return QString();
}

QString OsgCartoonVisNode::propertyGroup(int index)
{
    Q_UNUSED(index);
    return QString();
}

QVariant OsgCartoonVisNode::propertyValue(int index)
{
    Q_UNUSED(index);
    return QVariant();
}

void OsgCartoonVisNode::setPropertyValue(int index, QVariant value)
{
    Q_UNUSED(index);
    Q_UNUSED(value);
}

bool OsgCartoonVisNode::hasEditor(int index)
{
    Q_UNUSED(index);
    return false;
}

QWidget* OsgCartoonVisNode::createEditor(int index)
{
    Q_UNUSED(index);
    return 0;
}

void OsgCartoonVisNode::setEditorValue(int index, QWidget* widget, QVariant value)
{
    Q_UNUSED(index);
    Q_UNUSED(widget);
    Q_UNUSED(value);
}

QVariant OsgCartoonVisNode::editorValue(int index, QWidget* widget)
{
    Q_UNUSED(index);
    Q_UNUSED(widget);
}

void OsgCartoonVisNode::connectEditor(int index, QWidget* widget, QObject* receiver, const char* member)
{
    Q_UNUSED(index);
    Q_UNUSED(widget);
    Q_UNUSED(receiver);
    Q_UNUSED(member);
}

QString OsgCartoonVisNode::propertyValueString(int index)
{
    Q_UNUSED(index);
    return QString();
}

QVariant OsgCartoonVisNode::propertyValueFromString(int index, QString valueStr)
{
    Q_UNUSED(index);
    Q_UNUSED(valueStr);

    return QVariant();
}

bool OsgCartoonVisNode::canLoadSaveProperty(int index)
{
    Q_UNUSED(index);

    return false;
}

#endif





