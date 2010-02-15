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

#include "MapVisNode.h"
#include "OsgVisComponent.h"
#include "OsgLayerVisNodeIOData.h"
#include "OsgGroupVisNodeIOData.h"

#include <osgEarth/MapNode>
#include <osgEarth/MapLayer>
#include <osgEarth/Map>

bool isMapCreated;

DEFINE_VIS_NODE(MapVisNode, CGenericVisNodeBase)
{
    pDesc->setNodeClassCategory("OsgVis");
    pDesc->setNodeClassName("Map");
    pDesc->setNodeClassDescription("Map");
    pDesc->setNodeIcon( OsgVisComponent::instance().nodeIcon() );

    // Uncomment and use the following code template to add input/output paths
    pDesc->addConnectionPath(
        new CGenericVisNodeConnectionPath(
                "Layer",                                 // Name of the path
                IVisSystemNodeConnectionPath::InputPath,   // Path type can be OutputPath or InputPath
                "osg::ref_ptr<osgEarth::MapLayer>",                                 // Data type of the path
                0,                                          // Path index (don't change)
                true                                       // Allow Multiple Inputs Flag
            )
        );

	pDesc->addConnectionPath(
        new CGenericVisNodeConnectionPath(
                "MapNode",                                 // Name of the path
                IVisSystemNodeConnectionPath::OutputPath,   // Path type can be OutputPath or InputPath
				"osgEarth::ref_ptr<osgEarth::MapNode>",                                 // Data type of the path
                0,                                          // Path index (don't change)
                false                                       // Allow Multiple Inputs Flag
            )
        );
}

struct MapVisNodeData
{
	MapVisNodeData() : map(0) { }
	OsgGroupVisNodeIOData outputMapData;
	osg::ref_ptr<osg::Group> outputMap;
	osgEarth::Map * map;
};

MapVisNode::MapVisNode()
{
    MapVisNode::InitializeNodeDesc();
    d = new MapVisNodeData;

	d->outputMap = new osg::Group;
	d->outputMapData.setOsgNode(d->outputMap);
	isMapCreated = false;
}

MapVisNode::~MapVisNode()
{
    // This would be a good time to delete the backend object. If the backend object is a vtkObject
    // subclass, then you have to delete it now.
    // d->VtkObjectPointer->Delete();

    
    delete d;
}

QString MapVisNode::type() const
{
	return _type;
}

void MapVisNode::setType(QString type)
{
	if (_type.compare(type) != 0 && !isMapCreated)
	{
		_type = type;
		if (_type.toStdString() == "projected")
		{
				d->map = new osgEarth::Map(osgEarth::Map::CoordinateSystemType::CSTYPE_PROJECTED);
				isMapCreated = true;
		} else if (_type.toStdString() == "geocentric") {
				d->map = new osgEarth::Map(osgEarth::Map::CoordinateSystemType::CSTYPE_GEOCENTRIC);
				isMapCreated = true;
		} else if (_type.toStdString() == "geocentric-cube") {
				d->map = new osgEarth::Map(osgEarth::Map::CoordinateSystemType::CSTYPE_GEOCENTRIC_CUBE);
				isMapCreated = true;
		}
	}
}

bool MapVisNode::hasInput(IVisSystemNodeConnectionPath* path)
{
    if(!path)
        return false;

    /*
    If you have added input paths in the description block at the header of this file,
    then you will have to handle inputs here
    */

    return CGenericVisNodeBase::hasInput(path);
}

bool MapVisNode::setInput(IVisSystemNodeConnectionPath* path, IVisSystemNodeIOData* inputData)
{
    if(!path || !inputData)
        return false;

    /*
    If you have added input paths in the description block at the header of this file,
    then you will have to handle inputs here
    */
	if (path->pathName() == "Layer")
	{
		OsgLayerVisNodeIOData * layerData = 0;
		bool success = false;
		if (success = inputData->queryInterface("OsgLayerVisNodeIOData", (void**)&layerData)
			&& layerData)
		{
			// remove all from the group that contains the mapNode
			d->outputMap->removeChildren(0, d->outputMap->getNumChildren());
			// add the layer from the mapNode
			osgEarth::MapLayer * mLayer = layerData->getLayer();
			d->map->addMapLayer(mLayer);
			osg::ref_ptr<osgEarth::MapNode> mapNode = new osgEarth::MapNode(d->map);
			if (mapNode.valid()) // readd the mapNode to the group
				d->outputMap->addChild(mapNode.get()); // TODO: see setInput todo
			return true;
		}
	}
    return CGenericVisNodeBase::setInput(path, inputData);
}

bool MapVisNode::removeInput(IVisSystemNodeConnectionPath* path, IVisSystemNodeIOData* inputData)
{
    if(!path || !inputData)
        return false;

    /*
    If you have added input paths in the description block at the header of this file,
    then you will have to handle inputs here
    */
	if (path->pathName() == "Layer")
	{
		OsgLayerVisNodeIOData * layerData = 0;
		bool success = false;

		if (success = inputData->queryInterface("OsgLayerVisNodeIOData", (void**)&layerData)
			&& layerData)
			d->map->removeMapLayer(layerData->getLayer());

		if (success)
		{
			// remove all from the group that contains the mapNode
			d->outputMap->removeChildren(0, d->outputMap->getNumChildren());
			// remove the layer from the mapNode
			d->map->removeMapLayer(layerData->getLayer());
			if (! d->map->getImageMapLayers().empty())
			{
				osg::ref_ptr<osgEarth::MapNode> mapNode = new osgEarth::MapNode(d->map);
				if (mapNode.valid()) // readd the mapNode to the group
					d->outputMap->addChild(mapNode.get()); // TODO: see setInput todo
			}
			return true;
		}
	}

    return CGenericVisNodeBase::removeInput(path, inputData);
}

bool MapVisNode::fetchOutput(IVisSystemNodeConnectionPath* path, IVisSystemNodeIOData** outputData)
{
    if(!path || !outputData)
        return false;

    /*
    If you have added output paths in the description block at the header of this file,
    then you will have to handle outputs here
    */
	if (path->pathName() == "MapNode")
	{
		*outputData = &d->outputMapData;
		return true;
	}

    return CGenericVisNodeBase::fetchOutput(path, outputData);
}

bool MapVisNode::outputDerefed(IVisSystemNodeConnectionPath* path, IVisSystemNodeIOData* outputData)
{
    if(!path || !outputData)
        return false;

    /*
    If you have added output paths in the description block at the header of this file,
    then you will have to handle outputs here
    */
	if (path->pathName() == "MapNode")
		return true;

    return CGenericVisNodeBase::outputDerefed(path, outputData);
}

#ifdef ENABLE_ADVANCED_PROPERTIES

int MapVisNode::propertyCount()
{
    return 0;
}

QString MapVisNode::propertyName(int index)
{
    Q_UNUSED(index);
    return QString();
}

QString MapVisNode::propertyGroup(int index)
{
    Q_UNUSED(index);
    return QString();
}

QVariant MapVisNode::propertyValue(int index)
{
    Q_UNUSED(index);
    return QVariant();
}

void MapVisNode::setPropertyValue(int index, QVariant value)
{
    Q_UNUSED(index);
    Q_UNUSED(value);
}

bool MapVisNode::hasEditor(int index)
{
    Q_UNUSED(index);
    return false;
}

QWidget* MapVisNode::createEditor(int index)
{
    Q_UNUSED(index);
    return 0;
}

void MapVisNode::setEditorValue(int index, QWidget* widget, QVariant value)
{
    Q_UNUSED(index);
    Q_UNUSED(widget);
    Q_UNUSED(value);
}

QVariant MapVisNode::editorValue(int index, QWidget* widget)
{
    Q_UNUSED(index);
    Q_UNUSED(widget);
}

void MapVisNode::connectEditor(int index, QWidget* widget, QObject* receiver, const char* member)
{
    Q_UNUSED(index);
    Q_UNUSED(widget);
    Q_UNUSED(receiver);
    Q_UNUSED(member);
}

QString MapVisNode::propertyValueString(int index)
{
    Q_UNUSED(index);
    return QString();
}

QVariant MapVisNode::propertyValueFromString(int index, QString valueStr)
{
    Q_UNUSED(index);
    Q_UNUSED(valueStr);

    return QVariant();
}

bool MapVisNode::canLoadSaveProperty(int index)
{
    Q_UNUSED(index);

    return false;
}

#endif





