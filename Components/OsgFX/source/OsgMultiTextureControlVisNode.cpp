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

#include "OsgMultiTextureControlVisNode.h"
#include "OsgFXComponent.h"


DEFINE_VIS_NODE(OsgMultiTextureControlVisNode, CGenericVisNodeBase)
{
    pDesc->setNodeClassCategory("OsgFX");
    pDesc->setNodeClassName("OsgMultiTextureControl");
    pDesc->setNodeClassDescription("OsgMultiTextureControl");
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

struct OsgMultiTextureControlVisNodeData
{
    
};

OsgMultiTextureControlVisNode::OsgMultiTextureControlVisNode()
{
    OsgMultiTextureControlVisNode::InitializeNodeDesc();
    d = new OsgMultiTextureControlVisNodeData;

    
}

OsgMultiTextureControlVisNode::~OsgMultiTextureControlVisNode()
{
    // This would be a good time to delete the backend object. If the backend object is a vtkObject
    // subclass, then you have to delete it now.
    // d->VtkObjectPointer->Delete();

    
    delete d;
}

bool OsgMultiTextureControlVisNode::hasInput(IVisSystemNodeConnectionPath* path)
{
    if(!path)
        return false;

    /*
    If you have added input paths in the description block at the header of this file,
    then you will have to handle inputs here
    */

    return CGenericVisNodeBase::hasInput(path);
}

bool OsgMultiTextureControlVisNode::setInput(IVisSystemNodeConnectionPath* path, IVisSystemNodeIOData* inputData)
{
    if(!path || !inputData)
        return false;

    /*
    If you have added input paths in the description block at the header of this file,
    then you will have to handle inputs here
    */

    return CGenericVisNodeBase::setInput(path, inputData);
}

bool OsgMultiTextureControlVisNode::removeInput(IVisSystemNodeConnectionPath* path, IVisSystemNodeIOData* inputData)
{
    if(!path || !inputData)
        return false;

    /*
    If you have added input paths in the description block at the header of this file,
    then you will have to handle inputs here
    */

    return CGenericVisNodeBase::removeInput(path, inputData);
}

bool OsgMultiTextureControlVisNode::fetchOutput(IVisSystemNodeConnectionPath* path, IVisSystemNodeIOData** outputData)
{
    if(!path || !outputData)
        return false;

    /*
    If you have added output paths in the description block at the header of this file,
    then you will have to handle outputs here
    */

    return CGenericVisNodeBase::fetchOutput(path, outputData);
}

bool OsgMultiTextureControlVisNode::outputDerefed(IVisSystemNodeConnectionPath* path, IVisSystemNodeIOData* outputData)
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

int OsgMultiTextureControlVisNode::propertyCount()
{
    return 0;
}

QString OsgMultiTextureControlVisNode::propertyName(int index)
{
    Q_UNUSED(index);
    return QString();
}

QString OsgMultiTextureControlVisNode::propertyGroup(int index)
{
    Q_UNUSED(index);
    return QString();
}

QVariant OsgMultiTextureControlVisNode::propertyValue(int index)
{
    Q_UNUSED(index);
    return QVariant();
}

void OsgMultiTextureControlVisNode::setPropertyValue(int index, QVariant value)
{
    Q_UNUSED(index);
    Q_UNUSED(value);
}

bool OsgMultiTextureControlVisNode::hasEditor(int index)
{
    Q_UNUSED(index);
    return false;
}

QWidget* OsgMultiTextureControlVisNode::createEditor(int index)
{
    Q_UNUSED(index);
    return 0;
}

void OsgMultiTextureControlVisNode::setEditorValue(int index, QWidget* widget, QVariant value)
{
    Q_UNUSED(index);
    Q_UNUSED(widget);
    Q_UNUSED(value);
}

QVariant OsgMultiTextureControlVisNode::editorValue(int index, QWidget* widget)
{
    Q_UNUSED(index);
    Q_UNUSED(widget);
}

void OsgMultiTextureControlVisNode::connectEditor(int index, QWidget* widget, QObject* receiver, const char* member)
{
    Q_UNUSED(index);
    Q_UNUSED(widget);
    Q_UNUSED(receiver);
    Q_UNUSED(member);
}

QString OsgMultiTextureControlVisNode::propertyValueString(int index)
{
    Q_UNUSED(index);
    return QString();
}

QVariant OsgMultiTextureControlVisNode::propertyValueFromString(int index, QString valueStr)
{
    Q_UNUSED(index);
    Q_UNUSED(valueStr);

    return QVariant();
}

bool OsgMultiTextureControlVisNode::canLoadSaveProperty(int index)
{
    Q_UNUSED(index);

    return false;
}

#endif





