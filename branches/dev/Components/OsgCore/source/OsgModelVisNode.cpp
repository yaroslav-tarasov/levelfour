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

#include "OsgModelVisNode.h"
#include "OsgCoreComponent.h"
#include "OsgNodeVisNodeIOData.h"
#include <osg/ref_ptr>
#include <osg/Node>
#include <osgDB/ReadFile>
#include <QtCore/QString>

DEFINE_VIS_NODE(OsgModelVisNode, CGenericVisNodeBase)
{
    pDesc->setNodeClassCategory("OsgCore");
    pDesc->setNodeClassName("Model");
    pDesc->setNodeClassDescription("Load osg model");
    pDesc->setNodeIcon( OsgCoreComponent::instance().nodeIcon() );

    // Uncomment and use the following code template to add input/output paths

	pDesc->addConnectionPath(
        new CGenericVisNodeConnectionPath(
                "OsgModel",                                 // Name of the path
                IVisSystemNodeConnectionPath::OutputPath,   // Path type can be OutputPath or InputPath
				"osg::ref_ptr<osg::Node>",                                 // Data type of the path
                0,                                          // Path index (don't change)
                false                                       // Allow Multiple Inputs Flag
            )
        );
}

struct OsgModelVisNodeData
{
	OsgModelVisNodeData() : outputNode(0) {}
	osg::ref_ptr<osg::Group> outputNode;
	OsgNodeVisNodeIOData outputModelData;
};

OsgModelVisNode::OsgModelVisNode()
{
    OsgModelVisNode::InitializeNodeDesc();
    d = new OsgModelVisNodeData;

	d->outputNode = new osg::Group;
	d->outputModelData.setOsgNode(d->outputNode);
}

OsgModelVisNode::~OsgModelVisNode()
{
    // This would be a good time to delete the backend object. If the backend object is a vtkObject
    // subclass, then you have to delete it now.
    // d->VtkObjectPointer->Delete();

    
    delete d;
}

QString OsgModelVisNode::source() const
{
	return _source;
}

void OsgModelVisNode::setSource(QString source)
{
	if (_source.compare(source) != 0)
	{
		_source = source;

		d->outputNode->removeChildren(0, d->outputNode->getNumChildren());

		osg::ref_ptr<osg::Node> model = osgDB::readNodeFile(_source.toStdString());

		if (!model)
			return;
		
		d->outputNode->addChild(model);
	}
}

bool OsgModelVisNode::hasInput(IVisSystemNodeConnectionPath* path)
{
    if(!path)
        return false;

    /*
    If you have added input paths in the description block at the header of this file,
    then you will have to handle inputs here
    */

    return CGenericVisNodeBase::hasInput(path);
}

bool OsgModelVisNode::setInput(IVisSystemNodeConnectionPath* path, IVisSystemNodeIOData* inputData)
{
    if(!path || !inputData)
        return false;

    /*
    If you have added input paths in the description block at the header of this file,
    then you will have to handle inputs here
    */

    return CGenericVisNodeBase::setInput(path, inputData);
}

bool OsgModelVisNode::removeInput(IVisSystemNodeConnectionPath* path, IVisSystemNodeIOData* inputData)
{
    if(!path || !inputData)
        return false;

    /*
    If you have added input paths in the description block at the header of this file,
    then you will have to handle inputs here
    */

    return CGenericVisNodeBase::removeInput(path, inputData);
}

bool OsgModelVisNode::fetchOutput(IVisSystemNodeConnectionPath* path, IVisSystemNodeIOData** outputData)
{
    if(!path || !outputData)
        return false;

    /*
    If you have added output paths in the description block at the header of this file,
    then you will have to handle outputs here
    */
	if (path->pathName() == "OsgModel")
	{
		d->outputModelData.setOsgNode(d->outputNode);
		*outputData = &d->outputModelData;
		return true;
	}

    return CGenericVisNodeBase::fetchOutput(path, outputData);
}

bool OsgModelVisNode::outputDerefed(IVisSystemNodeConnectionPath* path, IVisSystemNodeIOData* outputData)
{
    if(!path || !outputData)
        return false;

    /*
    If you have added output paths in the description block at the header of this file,
    then you will have to handle outputs here
    */
	if (path->pathName() == "OsgModel")
		return true;

    return CGenericVisNodeBase::outputDerefed(path, outputData);
}

#ifdef ENABLE_ADVANCED_PROPERTIES

int OsgModelVisNode::propertyCount()
{
    return 0;
}

QString OsgModelVisNode::propertyName(int index)
{
    Q_UNUSED(index);
    return QString();
}

QString OsgModelVisNode::propertyGroup(int index)
{
    Q_UNUSED(index);
    return QString();
}

QVariant OsgModelVisNode::propertyValue(int index)
{
    Q_UNUSED(index);
    return QVariant();
}

void OsgModelVisNode::setPropertyValue(int index, QVariant value)
{
    Q_UNUSED(index);
    Q_UNUSED(value);
}

bool OsgModelVisNode::hasEditor(int index)
{
    Q_UNUSED(index);
    return false;
}

QWidget* OsgModelVisNode::createEditor(int index)
{
    Q_UNUSED(index);
    return 0;
}

void OsgModelVisNode::setEditorValue(int index, QWidget* widget, QVariant value)
{
    Q_UNUSED(index);
    Q_UNUSED(widget);
    Q_UNUSED(value);
}

QVariant OsgModelVisNode::editorValue(int index, QWidget* widget)
{
    Q_UNUSED(index);
    Q_UNUSED(widget);
}

void OsgModelVisNode::connectEditor(int index, QWidget* widget, QObject* receiver, const char* member)
{
    Q_UNUSED(index);
    Q_UNUSED(widget);
    Q_UNUSED(receiver);
    Q_UNUSED(member);
}

QString OsgModelVisNode::propertyValueString(int index)
{
    Q_UNUSED(index);
    return QString();
}

QVariant OsgModelVisNode::propertyValueFromString(int index, QString valueStr)
{
    Q_UNUSED(index);
    Q_UNUSED(valueStr);

    return QVariant();
}

bool OsgModelVisNode::canLoadSaveProperty(int index)
{
    Q_UNUSED(index);

    return false;
}

#endif





