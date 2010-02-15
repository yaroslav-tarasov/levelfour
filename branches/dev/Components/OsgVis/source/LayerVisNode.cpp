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

#include "LayerVisNode.h"
#include "OsgVisComponent.h"
#include "OsgLayerVisNodeIOData.h"
#include <osg/Node>
#include <osg/ref_ptr>
#include <osgEarth/MapLayer>
#include <osgEarth/MapNode>
#include <osgEarth/Map>
#include <osgEarth/Config>

bool isCompiled = false;

DEFINE_VIS_NODE(LayerVisNode, CGenericVisNodeBase)
{
    pDesc->setNodeClassCategory("OsgVis");
    pDesc->setNodeClassName("Layer");
    pDesc->setNodeClassDescription("Layer");
    pDesc->setNodeIcon( OsgVisComponent::instance().nodeIcon() );

    // Uncomment and use the following code template to add input/output paths

    // Uncomment and use the following code template to add input/output paths
    pDesc->addConnectionPath(
        new CGenericVisNodeConnectionPath(
                "Layer",                                 // Name of the path
                IVisSystemNodeConnectionPath::OutputPath,   // Path type can be OutputPath or InputPath
                "osg::ref_ptr<osgEarth::MapLayer>",                                 // Data type of the path
                0,                                          // Path index (don't change)
                false                                       // Allow Multiple Inputs Flag
            )
        );
}

struct LayerVisNodeData
{
	LayerVisNodeData() : layer(0) { }
	OsgLayerVisNodeIOData outputLayerData;
	osg::ref_ptr<osgEarth::MapLayer> layer;
	osgEarth::Config* conf;
};

LayerVisNode::LayerVisNode()
{
    LayerVisNode::InitializeNodeDesc();
    d = new LayerVisNodeData;

	_driver = "gdal";
	_type = "image";

	_visible = true;
}

LayerVisNode::~LayerVisNode()
{
    // This would be a good time to delete the backend object. If the backend object is a vtkObject
    // subclass, then you have to delete it now.
    // d->VtkObjectPointer->Delete();

    
    delete d;
}

bool LayerVisNode::getVisibility() const
{
	return _visible;
}

void LayerVisNode::setVisibility(bool visible)
{
	if (_visible != visible)
	{
		_visible = visible;

		if (_visible)
			d->layer->setOpacity(1);
		else 
			d->layer->setOpacity(0);
	}
}

QString LayerVisNode::source() const
{
	return _source;
}

void LayerVisNode::setSource(QString source)
{
	if (_source.compare(source) != 0)
	{
		isCompiled = false;
		_source = source;
	}
}

QString LayerVisNode::driver() const
{
	return _driver;
}

void LayerVisNode::setDriver(QString driver)
{
	if (_driver.compare(driver) != 0)
	{
		isCompiled = false;
		_driver = driver;
	}
}

QString LayerVisNode::type() const
{
	return _type;
}

void LayerVisNode::setType(QString type)
{
	if (_type.compare(type) != 0)
	{
		isCompiled = false;
		_type = type;
	}
}

QString LayerVisNode::format() const
{
	return _format;
}

void LayerVisNode::setFormat(QString format)
{
	if (_format.compare(format) != 0)
	{
		isCompiled = false;
		_format = format;
	}
}

QString LayerVisNode::tileWidth() const
{
	return _tileWidth;
}
void LayerVisNode::setTileWidth(QString tileWidth)
{
	if (_tileWidth.compare(tileWidth) != 0)
	{
		isCompiled = false;
		_tileWidth = tileWidth;
	}
}

QString LayerVisNode::tileHeight() const
{
	return _tileHeight;
}

void LayerVisNode::setTileHeight(QString tileHeight)
{
	if (_tileHeight.compare(tileHeight) != 0)
	{
		isCompiled = false;
		_tileHeight = tileHeight;
	}
}

QString LayerVisNode::tmsType() const
{
	return _tmsType;
}

void LayerVisNode::setTmsType(QString tmsType)
{
	if (_tmsType.compare(tmsType) != 0)
	{
		isCompiled = false;
		_tmsType = tmsType;
	}
}

QString LayerVisNode::profile() const
{
	return _profile;
}

void LayerVisNode::setProfile(QString profile)
{
	if (_profile.compare(profile) != 0)
	{
		isCompiled = false;
		_profile = profile;
	}
}

QString LayerVisNode::layers() const
{
	return _layers;
}

void LayerVisNode::setLayers(QString layers)
{
	if (_layers.compare(layers) != 0)
	{
		isCompiled = false;
		_layers = layers;
	}
}

QString LayerVisNode::tileSize() const
{
	return _tileSize;
}

void LayerVisNode::setTileSize(QString tileSize)
{
	if (_tileSize.compare(tileSize) != 0)
	{
		isCompiled = false;
		_tileSize = tileSize;
	}
}

QString LayerVisNode::SRS() const
{
	return _srs;
}

void LayerVisNode::setSRS(QString srs)
{
	if (_srs.compare(srs) != 0)
	{
		isCompiled = false;
		_srs = srs;
	}
}

QString LayerVisNode::dataSet() const
{
	return _dataSet;
}

void LayerVisNode::setDataSet(QString dataSet)
{
	if (_dataSet.compare(dataSet) != 0)
	{
		isCompiled = false;
		_dataSet = dataSet;
	}
}

QString LayerVisNode::style() const
{
	return _style;
}

void LayerVisNode::setStyle(QString style)
{
	if (_style.compare(style) != 0)
	{
		isCompiled = false;
		_style = style;
	}
}

QString LayerVisNode::elevationUnit() const
{
	return _elevationUnit;
}

void LayerVisNode::setElevationUnit(QString elevationUnit)
{
	if (_elevationUnit.compare(elevationUnit) != 0)
	{
		isCompiled = false;
		_elevationUnit= elevationUnit;
	}
}

void LayerVisNode::compile()
{
    command_Compile();
}

void LayerVisNode::command_Compile()
{
	d->conf = new osgEarth::Config;
	d->conf->add("url", _source.toStdString());
	if (_format.size() > 0)
		d->conf->add("format", _format.toStdString());
	if (_profile.size() > 0)
		d->conf->add("profile", _profile.toStdString());
	if (_tileWidth.size() > 0)
		d->conf->add("tile_width", _tileWidth.toStdString());
	if (_tileHeight.size() > 0)
		d->conf->add("tile_height", _tileHeight.toStdString());
	if (_tmsType.size() > 0)
		d->conf->add("tms_type", _tmsType.toStdString());
	if (_tileSize.size() > 0)
		d->conf->add("tile_size", _tileSize.toStdString());
	if (_layers.size() > 0)
		d->conf->add("layers", _layers.toStdString());
	if (_srs.size() > 0)
		d->conf->add("srs", _srs.toStdString());
	if (_dataSet.size() > 0)
		d->conf->add("dataset", _dataSet.toStdString());
	if (_elevationUnit.size() > 0)
		d->conf->add("elevation_unit", _elevationUnit.toStdString());
	if (_style.size() > 0)
		d->conf->add("style", _style.toStdString());
	
	osgEarth::MapLayer::Type t;
	if (_type.toStdString() == "image")
		t = osgEarth::MapLayer::Type::TYPE_IMAGE;
	else
		t = osgEarth::MapLayer::Type::TYPE_HEIGHTFIELD;

	d->layer = new osgEarth::MapLayer("OSM", t, _driver.toStdString(), *d->conf);
	d->outputLayerData.setLayer(d->layer);
	isCompiled = true;
}

bool LayerVisNode::hasInput(IVisSystemNodeConnectionPath* path)
{
    if(!path)
        return false;

    /*
    If you have added input paths in the description block at the header of this file,
    then you will have to handle inputs here
    */

    return CGenericVisNodeBase::hasInput(path);
}

bool LayerVisNode::setInput(IVisSystemNodeConnectionPath* path, IVisSystemNodeIOData* inputData)
{
    if(!path || !inputData)
        return false;

    /*
    If you have added input paths in the description block at the header of this file,
    then you will have to handle inputs here
    */

    return CGenericVisNodeBase::setInput(path, inputData);
}

bool LayerVisNode::removeInput(IVisSystemNodeConnectionPath* path, IVisSystemNodeIOData* inputData)
{
    if(!path || !inputData)
        return false;

    /*
    If you have added input paths in the description block at the header of this file,
    then you will have to handle inputs here
    */

    return CGenericVisNodeBase::removeInput(path, inputData);
}

bool LayerVisNode::fetchOutput(IVisSystemNodeConnectionPath* path, IVisSystemNodeIOData** outputData)
{
	if(!path || !outputData || !isCompiled)
        return false;

    /*
    If you have added output paths in the description block at the header of this file,
    then you will have to handle outputs here
    */
	if (path->pathName() == "Layer")
	{
		*outputData = &d->outputLayerData;
		return true;
	}

    return CGenericVisNodeBase::fetchOutput(path, outputData);
}

bool LayerVisNode::outputDerefed(IVisSystemNodeConnectionPath* path, IVisSystemNodeIOData* outputData)
{
    if(!path || !outputData)
        return false;

    /*
    If you have added output paths in the description block at the header of this file,
    then you will have to handle outputs here
    */
	if (path->pathName() == "Layer")
		return true;

    return CGenericVisNodeBase::outputDerefed(path, outputData);
}

#ifdef ENABLE_ADVANCED_PROPERTIES

int LayerVisNode::propertyCount()
{
    return 0;
}

QString LayerVisNode::propertyName(int index)
{
    Q_UNUSED(index);
    return QString();
}

QString LayerVisNode::propertyGroup(int index)
{
    Q_UNUSED(index);
    return QString();
}

QVariant LayerVisNode::propertyValue(int index)
{
    Q_UNUSED(index);
    return QVariant();
}

void LayerVisNode::setPropertyValue(int index, QVariant value)
{
    Q_UNUSED(index);
    Q_UNUSED(value);
}

bool LayerVisNode::hasEditor(int index)
{
    Q_UNUSED(index);
    return false;
}

QWidget* LayerVisNode::createEditor(int index)
{
    Q_UNUSED(index);
    return 0;
}

void LayerVisNode::setEditorValue(int index, QWidget* widget, QVariant value)
{
    Q_UNUSED(index);
    Q_UNUSED(widget);
    Q_UNUSED(value);
}

QVariant LayerVisNode::editorValue(int index, QWidget* widget)
{
    Q_UNUSED(index);
    Q_UNUSED(widget);
}

void LayerVisNode::connectEditor(int index, QWidget* widget, QObject* receiver, const char* member)
{
    Q_UNUSED(index);
    Q_UNUSED(widget);
    Q_UNUSED(receiver);
    Q_UNUSED(member);
}

QString LayerVisNode::propertyValueString(int index)
{
    Q_UNUSED(index);
    return QString();
}

QVariant LayerVisNode::propertyValueFromString(int index, QString valueStr)
{
    Q_UNUSED(index);
    Q_UNUSED(valueStr);

    return QVariant();
}

bool LayerVisNode::canLoadSaveProperty(int index)
{
    Q_UNUSED(index);

    return false;
}

#endif





