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

#include "FeaturesVisNode.h"
#include "OsgVisComponent.h"
#include "FeaturesVisNodeIOData.h"

DEFINE_VIS_NODE(FeaturesVisNode, CGenericVisNodeBase)
{
    pDesc->setNodeClassCategory("OsgVis");
    pDesc->setNodeClassName("Features");
    pDesc->setNodeClassDescription("Features");
    pDesc->setNodeIcon( OsgVisComponent::instance().nodeIcon() );

    // Uncomment and use the following code template to add input/output paths
    pDesc->addConnectionPath(
        new CGenericVisNodeConnectionPath(
                "Features",                                 // Name of the path
                IVisSystemNodeConnectionPath::OutputPath,   // Path type can be OutputPath or InputPath
                "osg::ref_ptr<osgEarth::Config>",                                 // Data type of the path
                0,                                          // Path index (don't change)
                false                                       // Allow Multiple Inputs Flag
            )
        );
}

struct FeaturesVisNodeData
{
	FeaturesVisNodeData() : outputFeatures(0) { }
	FeaturesVisNodeIOData outputFeaturesData;
	osgEarth::Config * outputFeatures;
};

FeaturesVisNode::FeaturesVisNode()
{
    FeaturesVisNode::InitializeNodeDesc();
    d = new FeaturesVisNodeData;

	_name = this->nodeName();
	d->outputFeatures = new osgEarth::Config(_name.toStdString());
	d->outputFeaturesData.setFeatures(d->outputFeatures);
}

FeaturesVisNode::~FeaturesVisNode()
{
    // This would be a good time to delete the backend object. If the backend object is a vtkObject
    // subclass, then you have to delete it now.
    // d->VtkObjectPointer->Delete();

    
    delete d;
}

QString FeaturesVisNode::source() const
{
	return _source;
}

void FeaturesVisNode::setSource(QString source)
{
	if (_source.compare(source) != 0)
	{
		_source = source;
		d->outputFeatures->add("url", _source.toStdString());
	}
}

QString FeaturesVisNode::driver() const
{
	return _driver;
}

void FeaturesVisNode::setDriver(QString driver)
{
	if (_driver.compare(driver) != 0)
	{
		_driver = driver;
		d->outputFeatures->add("driver", _driver.toStdString());
	}
}

QString FeaturesVisNode::getOgrDriver() const
{
	return _ogrDriver;
}

void FeaturesVisNode::setOgrDriver(QString ogrDriver)
{
	if (_ogrDriver.compare(ogrDriver) != 0)
	{
		_ogrDriver = ogrDriver;
		d->outputFeatures->add("ogr_driver", _ogrDriver.toStdString());
	}
}

bool FeaturesVisNode::hasInput(IVisSystemNodeConnectionPath* path)
{
    if(!path)
        return false;

    /*
    If you have added input paths in the description block at the header of this file,
    then you will have to handle inputs here
    */

    return CGenericVisNodeBase::hasInput(path);
}

bool FeaturesVisNode::setInput(IVisSystemNodeConnectionPath* path, IVisSystemNodeIOData* inputData)
{
    if(!path || !inputData)
        return false;

    /*
    If you have added input paths in the description block at the header of this file,
    then you will have to handle inputs here
    */

    return CGenericVisNodeBase::setInput(path, inputData);
}

bool FeaturesVisNode::removeInput(IVisSystemNodeConnectionPath* path, IVisSystemNodeIOData* inputData)
{
    if(!path || !inputData)
        return false;

    /*
    If you have added input paths in the description block at the header of this file,
    then you will have to handle inputs here
    */

    return CGenericVisNodeBase::removeInput(path, inputData);
}

bool FeaturesVisNode::fetchOutput(IVisSystemNodeConnectionPath* path, IVisSystemNodeIOData** outputData)
{
    if(!path || !outputData)
        return false;

    /*
    If you have added output paths in the description block at the header of this file,
    then you will have to handle outputs here
    */
	if (path->pathName() == "Features")
	{
		*outputData = &d->outputFeaturesData;
		return true;
	}


    return CGenericVisNodeBase::fetchOutput(path, outputData);
}

bool FeaturesVisNode::outputDerefed(IVisSystemNodeConnectionPath* path, IVisSystemNodeIOData* outputData)
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

int FeaturesVisNode::propertyCount()
{
    return 0;
}

QString FeaturesVisNode::propertyName(int index)
{
    Q_UNUSED(index);
    return QString();
}

QString FeaturesVisNode::propertyGroup(int index)
{
    Q_UNUSED(index);
    return QString();
}

QVariant FeaturesVisNode::propertyValue(int index)
{
    Q_UNUSED(index);
    return QVariant();
}

void FeaturesVisNode::setPropertyValue(int index, QVariant value)
{
    Q_UNUSED(index);
    Q_UNUSED(value);
}

bool FeaturesVisNode::hasEditor(int index)
{
    Q_UNUSED(index);
    return false;
}

QWidget* FeaturesVisNode::createEditor(int index)
{
    Q_UNUSED(index);
    return 0;
}

void FeaturesVisNode::setEditorValue(int index, QWidget* widget, QVariant value)
{
    Q_UNUSED(index);
    Q_UNUSED(widget);
    Q_UNUSED(value);
}

QVariant FeaturesVisNode::editorValue(int index, QWidget* widget)
{
    Q_UNUSED(index);
    Q_UNUSED(widget);
}

void FeaturesVisNode::connectEditor(int index, QWidget* widget, QObject* receiver, const char* member)
{
    Q_UNUSED(index);
    Q_UNUSED(widget);
    Q_UNUSED(receiver);
    Q_UNUSED(member);
}

QString FeaturesVisNode::propertyValueString(int index)
{
    Q_UNUSED(index);
    return QString();
}

QVariant FeaturesVisNode::propertyValueFromString(int index, QString valueStr)
{
    Q_UNUSED(index);
    Q_UNUSED(valueStr);

    return QVariant();
}

bool FeaturesVisNode::canLoadSaveProperty(int index)
{
    Q_UNUSED(index);

    return false;
}

#endif





