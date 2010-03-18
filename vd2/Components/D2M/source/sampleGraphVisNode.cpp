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

#include "sampleGraphVisNode.h"
#include "D2MComponent.h"
#include <boost/graph/adjacency_list.hpp>
#include "CBoostGraphData.h"
#include "CBoostPositionMapData.h"

#include <boost/graph/topology.hpp>

DEFINE_VIS_NODE(sampleGraphVisNode, CGenericVisNodeBase)
{
    pDesc->setNodeClassCategory("D2M");
    pDesc->setNodeClassName("sampleGraph");
    pDesc->setNodeClassDescription("sampleGraph");
    pDesc->setNodeIcon( D2MComponent::instance().nodeIcon() );

    // Uncomment and use the following code template to add input/output paths
    
    pDesc->addConnectionPath(
        new CGenericVisNodeConnectionPath(
                "GraphOutput",                              // Name of the path
                IVisSystemNodeConnectionPath::OutputPath,   // Path type can be OutputPath or InputPath
				"sandra::Graph",                            // Data type of the path
                0,                                          // Path index (don't change)
                false                                       // Allow Multiple Inputs Flag
            )
        );
    pDesc->addConnectionPath(
        new CGenericVisNodeConnectionPath(
                "PositionMap",                              // Name of the path
                IVisSystemNodeConnectionPath::OutputPath,   // Path type can be OutputPath or InputPath
				"sandra::PositionMap",						// Data type of the path
                0,                                          // Path index (don't change)
                false                                       // Allow Multiple Inputs Flag
            )
        );
}

struct sampleGraphVisNodeData
{
	sampleGraphVisNodeData() : graphAddress(0), vertexIdMapAddress(0), positionMapAddress(0) {}
	sandra::Graph* graphAddress;
	sandra::VertexIndexPropertyMap* vertexIdMapAddress;
	sandra::PositionMap* positionMapAddress;
	CBoostGraphData graphData;
	CBoostPositionMapData positionMapData;
};

sampleGraphVisNode::sampleGraphVisNode()
{
    sampleGraphVisNode::InitializeNodeDesc();
    d = new sampleGraphVisNodeData;
	sandra::Graph *g = new sandra::Graph();
	d->graphAddress = g;

	sandra::VertexIndexPropertyMap vertexIdPropertyMap = boost::get(&sandra::VertexProperties::index, *g);

	// add vertices to the graph object
	for (int i = 0; i < 8; ++i) {
		sandra::VertexDescriptor vd = boost::add_vertex(*g);
		vertexIdPropertyMap[vd] = i;
	}

	// Create property maps for vertex position //and for edge weight
	d->positionMapAddress = &(boost::get(&sandra::VertexProperties::point, *g));
	//WeightPropertyMap weightPropertyMap = boost::get(&EdgeProperty::weight, g);


}

sampleGraphVisNode::~sampleGraphVisNode()
{
    // This would be a good time to delete the backend object. If the backend object is a vtkObject
    // subclass, then you have to delete it now.
    // d->VtkObjectPointer->Delete();

    
    delete d;
}

bool sampleGraphVisNode::hasInput(IVisSystemNodeConnectionPath* path)
{
    if(!path)
        return false;

    /*
    If you have added input paths in the description block at the header of this file,
    then you will have to handle inputs here
    */

    return CGenericVisNodeBase::hasInput(path);
}

bool sampleGraphVisNode::setInput(IVisSystemNodeConnectionPath* path, IVisSystemNodeIOData* inputData)
{
    if(!path || !inputData)
        return false;

    /*
    If you have added input paths in the description block at the header of this file,
    then you will have to handle inputs here
    */

    return CGenericVisNodeBase::setInput(path, inputData);
}

bool sampleGraphVisNode::removeInput(IVisSystemNodeConnectionPath* path, IVisSystemNodeIOData* inputData)
{
    if(!path || !inputData)
        return false;

    /*
    If you have added input paths in the description block at the header of this file,
    then you will have to handle inputs here
    */

    return CGenericVisNodeBase::removeInput(path, inputData);
}

bool sampleGraphVisNode::fetchOutput(IVisSystemNodeConnectionPath* path, IVisSystemNodeIOData** outputData)
{
    if(!path || !outputData)
        return false;
	
	if(path->pathName() == "GraphOutput")
	{
		d->graphData.setGraph( d->graphAddress );
		*outputData = &d->graphData;
		return true;
	}
	if(path->pathName() == "PositionMap")
	{
		d->positionMapData.setPositionMap( d->positionMapAddress );
		*outputData = &d->positionMapData;
		return true;
	}

    return CGenericVisNodeBase::fetchOutput(path, outputData);
}

bool sampleGraphVisNode::outputDerefed(IVisSystemNodeConnectionPath* path, IVisSystemNodeIOData* outputData)
{
    if(!path || !outputData)
        return false;

	if(path->pathName() == "GraphOutput")
	{
		return true;
	}
	if(path->pathName() == "PositionMap")
	{
		return true;
	}

    return CGenericVisNodeBase::outputDerefed(path, outputData);
}

#ifdef ENABLE_ADVANCED_PROPERTIES

int sampleGraphVisNode::propertyCount()
{
    return 0;
}

QString sampleGraphVisNode::propertyName(int index)
{
    Q_UNUSED(index);
    return QString();
}

QString sampleGraphVisNode::propertyGroup(int index)
{
    Q_UNUSED(index);
    return QString();
}

QVariant sampleGraphVisNode::propertyValue(int index)
{
    Q_UNUSED(index);
    return QVariant();
}

void sampleGraphVisNode::setPropertyValue(int index, QVariant value)
{
    Q_UNUSED(index);
    Q_UNUSED(value);
}

bool sampleGraphVisNode::hasEditor(int index)
{
    Q_UNUSED(index);
    return false;
}

QWidget* sampleGraphVisNode::createEditor(int index)
{
    Q_UNUSED(index);
    return 0;
}

void sampleGraphVisNode::setEditorValue(int index, QWidget* widget, QVariant value)
{
    Q_UNUSED(index);
    Q_UNUSED(widget);
    Q_UNUSED(value);
}

QVariant sampleGraphVisNode::editorValue(int index, QWidget* widget)
{
    Q_UNUSED(index);
    Q_UNUSED(widget);
}

void sampleGraphVisNode::connectEditor(int index, QWidget* widget, QObject* receiver, const char* member)
{
    Q_UNUSED(index);
    Q_UNUSED(widget);
    Q_UNUSED(receiver);
    Q_UNUSED(member);
}

QString sampleGraphVisNode::propertyValueString(int index)
{
    Q_UNUSED(index);
    return QString();
}

QVariant sampleGraphVisNode::propertyValueFromString(int index, QString valueStr)
{
    Q_UNUSED(index);
    Q_UNUSED(valueStr);

    return QVariant();
}

bool sampleGraphVisNode::canLoadSaveProperty(int index)
{
    Q_UNUSED(index);

    return false;
}

#endif





