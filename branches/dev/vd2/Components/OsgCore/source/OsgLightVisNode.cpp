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

#include "OsgLightVisNode.h"
#include "OsgLightVisNodeIOData.h"
#include "OsgCoreComponent.h"
#include <osg/Light>
#include <osg/ref_ptr>

DEFINE_VIS_NODE(OsgLightVisNode, CGenericVisNodeBase)
{
    pDesc->setNodeClassCategory("Scene");
    pDesc->setNodeClassName("Light");
    pDesc->setNodeClassDescription("Create a light that can be used in a scene. Link this to a group.");
    pDesc->setNodeIcon( QIcon(":/OsgCore/Light.png") );

    // Uncomment and use the following code template to add input/output paths
    
    pDesc->addConnectionPath(
        new CGenericVisNodeConnectionPath(
                "OsgLight",                                 // Name of the path
                IVisSystemNodeConnectionPath::OutputPath,   // Path type can be OutputPath or InputPath
                "osg::ref_ptr<osg::Light>",                                 // Data type of the path
                0,                                          // Path index (don't change)
                false                                       // Allow Multiple Inputs Flag
            )
        );
    
}

struct OsgLightVisNodeData
{
    osg::ref_ptr<osg::Light> outputLight;
	OsgLightVisNodeIOData outputLightData;
};

OsgLightVisNode::OsgLightVisNode()
{
    OsgLightVisNode::InitializeNodeDesc();
    d = new OsgLightVisNodeData;
	d->outputLight = new osg::Light;
	d->outputLight->setAmbient(osg::Vec4(.3f, .3f, .3f, .3f));
	d->outputLight->setDiffuse(osg::Vec4(.8f, .8f, .8f, .1f));
	d->outputLight->setSpecular(osg::Vec4(.8f, .8f, .8f, .1f));
	d->outputLight->setSpotCutoff(25.f);
	setLightNum(2);
}

OsgLightVisNode::~OsgLightVisNode()
{
    // This would be a good time to delete the backend object. If the backend object is a vtkObject
    // subclass, then you have to delete it now.
    // d->VtkObjectPointer->Delete();

    
    delete d;
}

int OsgLightVisNode::lightNum() const
{
	return d->outputLight->getLightNum();
}

void OsgLightVisNode::setLightNum(int l)
{
	d->outputLight->setLightNum(l);
}

double OsgLightVisNode::posX() const
{
	osg::Vec4 pos = d->outputLight->getPosition();
	return pos.x();
}

void OsgLightVisNode::setPosX(double l)
{
	osg::Vec4 pos = d->outputLight->getPosition();
	d->outputLight->setPosition(osg::Vec4(l, pos.y(), pos.z(), pos.w()));
}

double OsgLightVisNode::posY() const
{
	osg::Vec4 pos = d->outputLight->getPosition();
	return pos.y();
}

void OsgLightVisNode::setPosY(double l)
{
	osg::Vec4 pos = d->outputLight->getPosition();
	d->outputLight->setPosition(osg::Vec4(pos.x(), l, pos.z(), pos.w()));
}

double OsgLightVisNode::posZ() const
{
	osg::Vec4 pos = d->outputLight->getPosition();
	return pos.z();
}

void OsgLightVisNode::setPosZ(double l)
{
	osg::Vec4 pos = d->outputLight->getPosition();
	d->outputLight->setPosition(osg::Vec4(pos.x(), pos.y(), l, pos.w()));
}

double OsgLightVisNode::posW() const
{
	osg::Vec4 pos = d->outputLight->getPosition();
	return pos.w();
}

void OsgLightVisNode::setPosW(double l)
{
	osg::Vec4 pos = d->outputLight->getPosition();
	d->outputLight->setPosition(osg::Vec4(pos.x(), pos.y(), pos.z(), l));
}

double OsgLightVisNode::directionX() const
{
	osg::Vec3 dir = d->outputLight->getDirection();
	return dir.x();
}

void OsgLightVisNode::setDirectionX(double l)
{
	osg::Vec3 dir = d->outputLight->getDirection();
	d->outputLight->setDirection(osg::Vec3(l, dir.y(), dir.z()));
}


double OsgLightVisNode::directionY() const
{
	osg::Vec3 dir = d->outputLight->getDirection();
	return dir.y();
}

void OsgLightVisNode::setDirectionY(double l)
{
	osg::Vec3 dir = d->outputLight->getDirection();
	d->outputLight->setDirection(osg::Vec3(dir.x(), l, dir.z()));
}


double OsgLightVisNode::directionZ() const
{
	osg::Vec3 dir = d->outputLight->getDirection();
	return dir.z();
}

void OsgLightVisNode::setDirectionZ(double l)
{
	osg::Vec3 dir = d->outputLight->getDirection();
	d->outputLight->setDirection(osg::Vec3(dir.x(), dir.y(), l));
}

bool OsgLightVisNode::hasInput(IVisSystemNodeConnectionPath* path)
{
    if(!path)
        return false;

    /*
    If you have added input paths in the description block at the header of this file,
    then you will have to handle inputs here
    */

    return CGenericVisNodeBase::hasInput(path);
}

bool OsgLightVisNode::setInput(IVisSystemNodeConnectionPath* path, IVisSystemNodeIOData* inputData)
{
    if(!path || !inputData)
        return false;

    /*
    If you have added input paths in the description block at the header of this file,
    then you will have to handle inputs here
    */

    return CGenericVisNodeBase::setInput(path, inputData);
}

bool OsgLightVisNode::removeInput(IVisSystemNodeConnectionPath* path, IVisSystemNodeIOData* inputData)
{
    if(!path || !inputData)
        return false;

    /*
    If you have added input paths in the description block at the header of this file,
    then you will have to handle inputs here
    */

    return CGenericVisNodeBase::removeInput(path, inputData);
}

bool OsgLightVisNode::fetchOutput(IVisSystemNodeConnectionPath* path, IVisSystemNodeIOData** outputData)
{
    if(!path || !outputData)
        return false;

    /*
    If you have added output paths in the description block at the header of this file,
    then you will have to handle outputs here
    */
	if (path->pathName() == "OsgLight")
	{
		d->outputLightData.setOsgLight(d->outputLight);
		*outputData = &d->outputLightData;
		return true;
	}

    return CGenericVisNodeBase::fetchOutput(path, outputData);
}

bool OsgLightVisNode::outputDerefed(IVisSystemNodeConnectionPath* path, IVisSystemNodeIOData* outputData)
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

int OsgLightVisNode::propertyCount()
{
    return 0;
}

QString OsgLightVisNode::propertyName(int index)
{
    Q_UNUSED(index);
    return QString();
}

QString OsgLightVisNode::propertyGroup(int index)
{
    Q_UNUSED(index);
    return QString();
}

QVariant OsgLightVisNode::propertyValue(int index)
{
    Q_UNUSED(index);
    return QVariant();
}

void OsgLightVisNode::setPropertyValue(int index, QVariant value)
{
    Q_UNUSED(index);
    Q_UNUSED(value);
}

bool OsgLightVisNode::hasEditor(int index)
{
    Q_UNUSED(index);
    return false;
}

QWidget* OsgLightVisNode::createEditor(int index)
{
    Q_UNUSED(index);
    return 0;
}

void OsgLightVisNode::setEditorValue(int index, QWidget* widget, QVariant value)
{
    Q_UNUSED(index);
    Q_UNUSED(widget);
    Q_UNUSED(value);
}

QVariant OsgLightVisNode::editorValue(int index, QWidget* widget)
{
    Q_UNUSED(index);
    Q_UNUSED(widget);
}

void OsgLightVisNode::connectEditor(int index, QWidget* widget, QObject* receiver, const char* member)
{
    Q_UNUSED(index);
    Q_UNUSED(widget);
    Q_UNUSED(receiver);
    Q_UNUSED(member);
}

QString OsgLightVisNode::propertyValueString(int index)
{
    Q_UNUSED(index);
    return QString();
}

QVariant OsgLightVisNode::propertyValueFromString(int index, QString valueStr)
{
    Q_UNUSED(index);
    Q_UNUSED(valueStr);

    return QVariant();
}

bool OsgLightVisNode::canLoadSaveProperty(int index)
{
    Q_UNUSED(index);

    return false;
}

#endif





