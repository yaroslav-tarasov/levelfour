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

#include "ModelVisNode.h"
#include "ModelVisNodeIOData.h"
#include "FeaturesVisNodeIOData.h"
#include "OsgVisComponent.h"

#include <osg/ref_ptr>
#include <osgEarth/ModelLayer>
#include <osgEarth/Config>

DEFINE_VIS_NODE(ModelVisNode, CGenericVisNodeBase)
{
    pDesc->setNodeClassCategory("OsgVis");
    pDesc->setNodeClassName("Model Layer");
    pDesc->setNodeClassDescription("Model");
    pDesc->setNodeIcon( OsgVisComponent::instance().nodeIcon() );

    // Uncomment and use the following code template to add input/output paths

	pDesc->addConnectionPath(
        new CGenericVisNodeConnectionPath(
                "Features",                                 // Name of the path
                IVisSystemNodeConnectionPath::InputPath,   // Path type can be OutputPath or InputPath
                "osg::ref_ptr<osgEarth::Config>",                                 // Data type of the path
                0,                                          // Path index (don't change)
                false                                       // Allow Multiple Inputs Flag
            )
        );

	pDesc->addConnectionPath(
        new CGenericVisNodeConnectionPath(
                "Model",                                 // Name of the path
                IVisSystemNodeConnectionPath::OutputPath,   // Path type can be OutputPath or InputPath
                "osg::ref_ptr<osgEarth::ModelLayer>",                                 // Data type of the path
                0,                                          // Path index (don't change)
                false                                       // Allow Multiple Inputs Flag
            )
        );
}

struct ModelVisNodeData
{
	ModelVisNodeData() : model(0), inputFeatures(0), conf(0) { }
	ModelVisNodeIOData outputModelData;
	osg::ref_ptr<osgEarth::ModelLayer> model;
	osgEarth::Config* conf;
	osgEarth::Config * inputFeatures;
};

ModelVisNode::ModelVisNode()
{
    ModelVisNode::InitializeNodeDesc();
    d = new ModelVisNodeData;

	_name = this->nodeName();
	_driver = "feature_stencil";
	_geometryType = "polygon";
	isCompiled = false;
}

ModelVisNode::~ModelVisNode()
{
    // This would be a good time to delete the backend object. If the backend object is a vtkObject
    // subclass, then you have to delete it now.
    // d->VtkObjectPointer->Delete();

    
    delete d;
}

QString ModelVisNode::getDriver() const
{
	return _driver;
}

void ModelVisNode::setDriver(QString driver)
{
	if (_driver.compare(driver) != 0)
	{
		isCompiled = false;
		_driver = driver;
	}
}

QString ModelVisNode::getGeometry() const
{
	return _geometryType;
}

void ModelVisNode::setGeometry(QString geom)
{
	if (_geometryType.compare(geom) != 0)
	{
		isCompiled = false;
		_geometryType = geom;
	}
}

QString ModelVisNode::getExtrusionDistance() const
{
	return _extrusionDistance;
}

void ModelVisNode::setExtrusionDistance(QString extr)
{
	if (_extrusionDistance.compare(extr) != 0)
	{
		isCompiled = false;
		_extrusionDistance = extr;
	}
}

QString ModelVisNode::getHeightOffset() const
{
	return _extrusionDistance;
}

void ModelVisNode::setHeightOffset(QString heightOffset)
{
	if (_heightOffset.compare(heightOffset) != 0)
	{
		isCompiled = false;
		_heightOffset = heightOffset;
	}
}

void ModelVisNode::process()
{
	command_Process();
}

void ModelVisNode::command_Process()
{
	d->conf = new osgEarth::Config;
	if (_geometryType.size() > 0)
		d->conf->add("geometry_type", _geometryType.toStdString());
	if (_extrusionDistance.size() > 0)
		d->conf->add("extrusion_distance", _extrusionDistance.toStdString());

	if (d->inputFeatures)
		d->conf->addChild(*d->inputFeatures);

	d->model = new osgEarth::ModelLayer(_name.toStdString(), _driver.toStdString(), *d->conf);
	d->outputModelData.setModel(d->model);
}

bool ModelVisNode::hasInput(IVisSystemNodeConnectionPath* path)
{
    if(!path)
        return false;

    /*
    If you have added input paths in the description block at the header of this file,
    then you will have to handle inputs here
    */

    return CGenericVisNodeBase::hasInput(path);
}

bool ModelVisNode::setInput(IVisSystemNodeConnectionPath* path, IVisSystemNodeIOData* inputData)
{
    if(!path || !inputData)
        return false;

    /*
    If you have added input paths in the description block at the header of this file,
    then you will have to handle inputs here
    */
	if (path->pathName() == "Features")
	{
		FeaturesVisNodeIOData * featuresData = 0;
		bool success = false;
		if (success = inputData->queryInterface("FeaturesVisNodeIOData", (void**)&featuresData)
			&& featuresData)
		{
			// add the layer to the map
			d->inputFeatures = featuresData->getFeatures();
			if (!d->conf)
				d->conf = new osgEarth::Config;
			d->conf->addChild(* d->inputFeatures);
			return true;
		}
	}

    return CGenericVisNodeBase::setInput(path, inputData);
}

bool ModelVisNode::removeInput(IVisSystemNodeConnectionPath* path, IVisSystemNodeIOData* inputData)
{
    if(!path || !inputData)
        return false;

    /*
    If you have added input paths in the description block at the header of this file,
    then you will have to handle inputs here
    */
	if (path->pathName() == "Features")
	{
		FeaturesVisNodeIOData * featuresData = 0;
		bool success = false;
		if (success = inputData->queryInterface("FeaturesVisNodeIOData", (void**)&featuresData)
			&& featuresData)
		{

			return true;
		}
	}

    return CGenericVisNodeBase::removeInput(path, inputData);
}

bool ModelVisNode::fetchOutput(IVisSystemNodeConnectionPath* path, IVisSystemNodeIOData** outputData)
{
    if(!path || !outputData)
        return false;

    /*
    If you have added output paths in the description block at the header of this file,
    then you will have to handle outputs here
    */
	if (path->pathName() == "Model")
	{
		*outputData = &d->outputModelData;
		return true;
	}


    return CGenericVisNodeBase::fetchOutput(path, outputData);
}

bool ModelVisNode::outputDerefed(IVisSystemNodeConnectionPath* path, IVisSystemNodeIOData* outputData)
{
    if(!path || !outputData)
        return false;

    /*
    If you have added output paths in the description block at the header of this file,
    then you will have to handle outputs here
    */
	if (path->pathName() == "Model")
		return true;

    return CGenericVisNodeBase::outputDerefed(path, outputData);
}

#ifdef ENABLE_ADVANCED_PROPERTIES

int ModelVisNode::propertyCount()
{
    return 0;
}

QString ModelVisNode::propertyName(int index)
{
    Q_UNUSED(index);
    return QString();
}

QString ModelVisNode::propertyGroup(int index)
{
    Q_UNUSED(index);
    return QString();
}

QVariant ModelVisNode::propertyValue(int index)
{
    Q_UNUSED(index);
    return QVariant();
}

void ModelVisNode::setPropertyValue(int index, QVariant value)
{
    Q_UNUSED(index);
    Q_UNUSED(value);
}

bool ModelVisNode::hasEditor(int index)
{
    Q_UNUSED(index);
    return false;
}

QWidget* ModelVisNode::createEditor(int index)
{
    Q_UNUSED(index);
    return 0;
}

void ModelVisNode::setEditorValue(int index, QWidget* widget, QVariant value)
{
    Q_UNUSED(index);
    Q_UNUSED(widget);
    Q_UNUSED(value);
}

QVariant ModelVisNode::editorValue(int index, QWidget* widget)
{
    Q_UNUSED(index);
    Q_UNUSED(widget);
}

void ModelVisNode::connectEditor(int index, QWidget* widget, QObject* receiver, const char* member)
{
    Q_UNUSED(index);
    Q_UNUSED(widget);
    Q_UNUSED(receiver);
    Q_UNUSED(member);
}

QString ModelVisNode::propertyValueString(int index)
{
    Q_UNUSED(index);
    return QString();
}

QVariant ModelVisNode::propertyValueFromString(int index, QString valueStr)
{
    Q_UNUSED(index);
    Q_UNUSED(valueStr);

    return QVariant();
}

bool ModelVisNode::canLoadSaveProperty(int index)
{
    Q_UNUSED(index);

    return false;
}

#endif





