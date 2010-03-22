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

#include "circleGraphLayoutVisNode.h"
#include "D2MComponent.h"
#include <boost/graph/adjacency_list.hpp>
#include "CBoostGraphData.h"
#include "CBoostPositionMapData.h"
#include <boost/graph/circle_layout.hpp>
#include <boost/graph/graph_utility.hpp>

#include <windows.h>
#include <osg/ref_ptr>
#include <osg/Geode>
#include <osg/Notify>
#include <osg/ShapeDrawable>
#include <osgDB/WriteFile>


DEFINE_VIS_NODE(circleGraphLayoutVisNode, CGenericVisNodeBase)
{
    pDesc->setNodeClassCategory("D2M");
    pDesc->setNodeClassName("circleGraphLayout");
    pDesc->setNodeClassDescription("circleGraphLayout");
    pDesc->setNodeIcon( D2MComponent::instance().nodeIcon() );

    pDesc->addConnectionPath(
        new CGenericVisNodeConnectionPath(
                "GraphInput",                               // Name of the path
                IVisSystemNodeConnectionPath::InputPath,    // Path type can be OutputPath or InputPath
				"graphdefs::Graph",                            // Data type of the path
                0,                                          // Path index (don't change)
                false                                       // Allow Multiple Inputs Flag
            )
        );

    pDesc->addConnectionPath(
        new CGenericVisNodeConnectionPath(
                "PositionMapInput",                         // Name of the path
                IVisSystemNodeConnectionPath::InputPath,    // Path type can be OutputPath or InputPath
				"graphdefs::PositionMap",                      // Data type of the path
                0,                                          // Path index (don't change)
                false                                       // Allow Multiple Inputs Flag
            )
        );

    pDesc->addConnectionPath(
        new CGenericVisNodeConnectionPath(
                "GraphOutput",                              // Name of the path
                IVisSystemNodeConnectionPath::OutputPath,   // Path type can be OutputPath or InputPath
				"graphdefs::Graph",                            // Data type of the path
                0,                                          // Path index (don't change)
                false                                       // Allow Multiple Inputs Flag
            )
        );

    pDesc->addConnectionPath(
        new CGenericVisNodeConnectionPath(
                "PositionMapOutput",                       // Name of the path
                IVisSystemNodeConnectionPath::OutputPath,   // Path type can be OutputPath or InputPath
				"graphdefs::PositionMap",                      // Data type of the path
                0,                                          // Path index (don't change)
                false                                       // Allow Multiple Inputs Flag
            )
        );
}

struct circleGraphLayoutVisNodeData
{
	circleGraphLayoutVisNodeData() : graphAddress(0), positionMapAddress(0) {}
	graphdefs::Graph* graphAddress;
	graphdefs::PositionMap* positionMapAddress;
	CBoostGraphData graphData;
	CBoostPositionMapData positionMapData;
};

circleGraphLayoutVisNode::circleGraphLayoutVisNode()
{
    circleGraphLayoutVisNode::InitializeNodeDesc();
    d = new circleGraphLayoutVisNodeData;   
}

circleGraphLayoutVisNode::~circleGraphLayoutVisNode()
{
    // This would be a good time to delete the backend object. If the backend object is a vtkObject
    // subclass, then you have to delete it now.
    // d->VtkObjectPointer->Delete();
    delete d;
}

void circleGraphLayoutVisNode::saveOsgFile(osg::ref_ptr<osg::Node> sceneGraph)
{
	if (!sceneGraph.valid())
		osg::notify(osg::FATAL) << "Failed in saveOsgFile\n";

	bool result = osgDB::writeNodeFile( *(sceneGraph.get()), "SASN.osg" );
	if( !result )
		osg::notify(osg::FATAL) << "Failed in osgDB::writeNodeFile().\n";
}

void circleGraphLayoutVisNode::calcLayout()
{
	if( d->graphAddress != 0 && d->positionMapAddress != 0 )
	{
		graphdefs::PositionMap positionMap = *d->positionMapAddress;

		// apply Boost's circle layout
		boost::circle_graph_layout(*d->graphAddress, positionMap, 100);

		//This last part should later be moved to another node class, a renderer
		//Create a cube for each vertex and add all cubes to a Geode
		float cubeSize = 1.f;
		osg::Vec3 position;
		osg::ref_ptr<osg::ShapeDrawable> cube;
		osg::ref_ptr<osg::Geode> geode = new osg::Geode;

		boost::graph_traits<graphdefs::Graph>::vertex_iterator i, end;
		for (boost::tie(i, end) = boost::vertices(*d->graphAddress); i != end; ++i) {
			position = osg::Vec3( positionMap[*i][0], 0.f, positionMap[*i][1] );
			cube = new osg::ShapeDrawable( new osg::Box(position, cubeSize) );
		    geode->addDrawable(cube);
		}
		
		//Save the Geode in an osg file
		saveOsgFile(geode);
	}
}


bool circleGraphLayoutVisNode::hasInput(IVisSystemNodeConnectionPath* path)
{
    if(!path)
        return false;

	if(path->pathName() == "GraphInput")
		return d->graphAddress != 0;
	if(path->pathName() == "PositionMapInput")
		return d->positionMapAddress != 0;

    return CGenericVisNodeBase::hasInput(path);
}

bool circleGraphLayoutVisNode::setInput(IVisSystemNodeConnectionPath* path, IVisSystemNodeIOData* inputData)
{
    if(!path || !inputData)
        return false;

	if(path->pathName() == "GraphInput")
	{
		IBoostGraphIOData* data = 0;
		bool success = inputData->queryInterface("IBoostGraphIOData", (void**)&data);
		if(success && data)
		{
			graphdefs::Graph* graph = data->getBoostGraph();
			d->graphAddress = graph;
			calcLayout();
			return true;
		}

		return false;
	}

	if(path->pathName() == "PositionMapInput")
	{
		IBoostPositionMapIOData* data = 0;
		bool success = inputData->queryInterface("IBoostPositionMapIOData", (void**)&data);
		if(success && data)
		{
			graphdefs::PositionMap* positionMap = data->getBoostPositionMap();
			d->positionMapAddress = positionMap;
			calcLayout();
			return true;
		}

		return false;
	}

    return CGenericVisNodeBase::setInput(path, inputData);
}

bool circleGraphLayoutVisNode::removeInput(IVisSystemNodeConnectionPath* path, IVisSystemNodeIOData* inputData)
{
    if(!path || !inputData)
        return false;

	if(path->pathName() == "GraphInput")
	{
		IBoostGraphIOData* data = 0;
		bool success = inputData->queryInterface("IBoostGraphIOData", (void**)&data);
		if(success && data &&
		   data->getBoostGraph()== d->graphAddress)
		{
			d->graphAddress = 0;
			return true;
		}
		return false;
	}

	if(path->pathName() == "PositionMapInput")
	{
		IBoostPositionMapIOData* data = 0;
		bool success = inputData->queryInterface("IBoostPositionMapIOData", (void**)&data);
		if(success && data &&
		   data->getBoostPositionMap()== d->positionMapAddress)
		{
			d->positionMapAddress = 0;
			return true;
		}
		return false;
	}
	return CGenericVisNodeBase::removeInput(path, inputData);
}

bool circleGraphLayoutVisNode::fetchOutput(IVisSystemNodeConnectionPath* path, IVisSystemNodeIOData** outputData)
{
    if(!path || !outputData)
        return false;

    if(path->pathName() == "GraphOutput")
	{
		d->graphData.setGraph( d->graphAddress );
		*outputData = &d->graphData;
		return true;
	}

    if(path->pathName() == "PositionMapOutput")
	{
		d->positionMapData.setPositionMap( d->positionMapAddress );
		*outputData = &d->positionMapData;
		return true;
	}
	
	return CGenericVisNodeBase::fetchOutput(path, outputData);
}

bool circleGraphLayoutVisNode::outputDerefed(IVisSystemNodeConnectionPath* path, IVisSystemNodeIOData* outputData)
{
    if(!path || !outputData)
        return false;

	if(path->pathName() == "GraphOutput")
		return true;
	if(path->pathName() == "PositionMapOutput")
		return true;

    return CGenericVisNodeBase::outputDerefed(path, outputData);
}

#ifdef ENABLE_ADVANCED_PROPERTIES

int circleGraphLayoutVisNode::propertyCount()
{
    return 0;
}

QString circleGraphLayoutVisNode::propertyName(int index)
{
    Q_UNUSED(index);
    return QString();
}

QString circleGraphLayoutVisNode::propertyGroup(int index)
{
    Q_UNUSED(index);
    return QString();
}

QVariant circleGraphLayoutVisNode::propertyValue(int index)
{
    Q_UNUSED(index);
    return QVariant();
}

void circleGraphLayoutVisNode::setPropertyValue(int index, QVariant value)
{
    Q_UNUSED(index);
    Q_UNUSED(value);
}

bool circleGraphLayoutVisNode::hasEditor(int index)
{
    Q_UNUSED(index);
    return false;
}

QWidget* circleGraphLayoutVisNode::createEditor(int index)
{
    Q_UNUSED(index);
    return 0;
}

void circleGraphLayoutVisNode::setEditorValue(int index, QWidget* widget, QVariant value)
{
    Q_UNUSED(index);
    Q_UNUSED(widget);
    Q_UNUSED(value);
}

QVariant circleGraphLayoutVisNode::editorValue(int index, QWidget* widget)
{
    Q_UNUSED(index);
    Q_UNUSED(widget);
}

void circleGraphLayoutVisNode::connectEditor(int index, QWidget* widget, QObject* receiver, const char* member)
{
    Q_UNUSED(index);
    Q_UNUSED(widget);
    Q_UNUSED(receiver);
    Q_UNUSED(member);
}

QString circleGraphLayoutVisNode::propertyValueString(int index)
{
    Q_UNUSED(index);
    return QString();
}

QVariant circleGraphLayoutVisNode::propertyValueFromString(int index, QString valueStr)
{
    Q_UNUSED(index);
    Q_UNUSED(valueStr);

    return QVariant();
}

bool circleGraphLayoutVisNode::canLoadSaveProperty(int index)
{
    Q_UNUSED(index);

    return false;
}

#endif





