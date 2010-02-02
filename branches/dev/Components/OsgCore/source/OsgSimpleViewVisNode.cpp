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

#include "OsgSimpleViewVisNode.h"
#include "OsgGroupVisNodeIOData.h"
#include "OsgCoreComponent.h"
#include "OsgLightVisNodeIOData.h"

#include <osgGA/TrackballManipulator>

#include <osgDB/WriteFile>
#include <osgDB/ReadFile>
#include <osg/Node>
#include <osg/ref_ptr>
#include <osg/LightSource>

#include <QTabWidget>
#include <QSize>
#include <QtGui/QPushButton>
#include <QtGui/QDialog>
#include <QtGui/QVBoxLayout>

DEFINE_VIS_NODE(OsgSimpleViewVisNode, CGenericVisNodeBase)
{
    pDesc->setNodeClassCategory("OsgCore");
    pDesc->setNodeClassName("Simple scene view");
    pDesc->setNodeClassDescription("Create a Qt widget inside the OSG outpur tab, devoted to render the current scene.");
    pDesc->setNodeIcon( QIcon(":/OsgCore/SceneView.png") );

    // Uncomment and use the following code template to add input/output paths
    
    pDesc->addConnectionPath(
        new CGenericVisNodeConnectionPath(
                "OsgGroupInput",                                 // Name of the path
                IVisSystemNodeConnectionPath::InputPath,   // Path type can be OutputPath or InputPath
                "osg::ref_ptr<osg::Group>",                                 // Data type of the path
                0,                                          // Path index (don't change)
                false                                       // Allow Multiple Inputs Flag
            )
        );
    pDesc->addConnectionPath(
        new CGenericVisNodeConnectionPath(
                "OsgLightInput",                                 // Name of the path
                IVisSystemNodeConnectionPath::InputPath,   // Path type can be OutputPath or InputPath
                "osg::ref_ptr<osg::Light>",                                 // Data type of the path
                0,                                          // Path index (don't change)
                false                                       // Allow Multiple Inputs Flag
            )
        );
}

struct OsgSimpleViewVisNodeData
{
	OsgSimpleViewVisNodeData() : scene(0), inputNode(0) {}
    osg::QOSGScene * scene;
	osg::ref_ptr<osg::Node> inputNode;
	OsgGroupVisNodeIOData inputData;
	osg::ref_ptr<osg::Light> inputLight;
};

OsgSimpleViewVisNode::OsgSimpleViewVisNode() 
{
    OsgSimpleViewVisNode::InitializeNodeDesc();
    d = new OsgSimpleViewVisNodeData;

	m_osgOutputWidget = new osg::QGLGraphicsView;

	OsgCoreComponent::instance().osgOutputWidget()->addTab(m_osgOutputWidget, this->nodeName());

	d->scene = new osg::QOSGScene;
	m_osgOutputWidget->setScene(d->scene);
	d->scene->setCameraManipulator(new osgGA::TrackballManipulator);

	d->inputLight = new osg::Light;
	d->inputLight->setLightNum(2);
	d->inputLight->setAmbient(osg::Vec4(.1f, .1f, .1f, .1f));
	d->inputLight->setDiffuse(osg::Vec4(.8f, .8f, .8f, .1f));
	d->inputLight->setSpecular(osg::Vec4(.8f, .8f, .8f, .1f));
	d->inputLight->setPosition(osg::Vec4(.0f, .0f, .0f, .0f));
	d->inputLight->setDirection(osg::Vec3(.1f, .0f, .0f));
	d->inputLight->setSpotCutoff(25.f);
	osg::LightSource * lightSource = new osg::LightSource;
	lightSource->setLight(d->inputLight.get());
	d->scene->setLight(d->inputLight);
}

OsgSimpleViewVisNode::~OsgSimpleViewVisNode()
{
    // This would be a good time to delete the backend object. If the backend object is a vtkObject
    // subclass, then you have to delete it now.
    // d->VtkObjectPointer->Delete();

	removeSceneWidget();
	delete m_osgOutputWidget;
	delete d;
}

void OsgSimpleViewVisNode::render()
{
    command_Render();
}

void OsgSimpleViewVisNode::command_Render()
{
	delete d->scene;
	d->scene = new osg::QOSGScene;
	d->scene->setCameraManipulator(new osgGA::TrackballManipulator);
	d->scene->setLight(d->inputLight);
	d->scene->setSceneData(d->inputNode);
	m_osgOutputWidget->setScene(d->scene);
	QSize s(m_osgOutputWidget->size());
	if (d->scene)
		d->scene->setSceneRect(0, 0, s.width(), s.height());
}

void OsgSimpleViewVisNode::saveOSG()
{
    command_SaveOSG();
}

void OsgSimpleViewVisNode::command_SaveOSG()
{
	if (!d->scene)
		return;

	osg::ref_ptr<osg::Node> root = d->scene->getSceneData();
	if (!root.valid())
		return;

	osgDB::writeNodeFile(*(root.get()), "test.osg");
	system("D:\\OSG-2.9.6\\bin\\osgviewerQTd.exe test.osg");
}

bool OsgSimpleViewVisNode::hasInput(IVisSystemNodeConnectionPath* path)
{
    if(!path)
        return false;

    /*
    If you have added input paths in the description block at the header of this file,
    then you will have to handle inputs here
    */

	if (path->pathName() == "OsgGroupInput")
		return d->inputNode != 0;
    return CGenericVisNodeBase::hasInput(path);
}

bool OsgSimpleViewVisNode::setInput(IVisSystemNodeConnectionPath* path, IVisSystemNodeIOData* inputData)
{
    if(!path || !inputData)
        return false;

    /*
    If you have added input paths in the description block at the header of this file,
    then you will have to handle inputs here
    */

	if (path->pathName() == "OsgGroupInput")
	{
		OsgGroupVisNodeIOData * data = 0;
		bool success = inputData->queryInterface("OsgNodeVisNodeIOData", (void**)&data);
		if (success && data)
		{
			d->inputData.setOsgNode(d->inputNode = data->getOsgNode());
			command_Render();

			return true;
		}
	}

	if (path->pathName() == "OsgLightInput")
	{
		OsgLightVisNodeIOData * inputLightData = 0;
		bool success = inputData->queryInterface("OsgLightVisNodeIOData", (void**)&inputLightData);

		if (success && inputLightData)
		{
			d->inputLight = inputLightData->getOsgLight();
			osg::LightSource * lightSource = new osg::LightSource;
			lightSource->setLight(d->inputLight.get());
			d->scene->setLight(d->inputLight);
			return true;
		}
	}

	return CGenericVisNodeBase::setInput(path, inputData);
}

bool OsgSimpleViewVisNode::removeInput(IVisSystemNodeConnectionPath* path, IVisSystemNodeIOData* inputData)
{
    if(!path || !inputData)
        return false;

    /*
    If you have added input paths in the description block at the header of this file,
    then you will have to handle inputs here
    */

	if (path->pathName() == "OsgGroupInput")
	{
		OsgGroupVisNodeIOData * data = 0;
		bool success = inputData->queryInterface("OsgNodeVisNodeIOData", (void**)&data);
		if (success && data && data->getOsgNode() == d->inputNode)
		{
			d->inputData.setOsgNode(d->inputNode = 0);

			return  true;
		}
	}

	if (path->pathName() == "OsgLightInput")
	{
		OsgLightVisNodeIOData * inputLightData = 0;
		bool success = inputData->queryInterface("OsgLightVisNodeIOData", (void**)&inputLightData);

		if (success && inputLightData)
		{
			d->scene->setLight(d->inputLight = 0);
			return true;
		}
	}

	return CGenericVisNodeBase::removeInput(path, inputData);
}

bool OsgSimpleViewVisNode::fetchOutput(IVisSystemNodeConnectionPath* path, IVisSystemNodeIOData** outputData)
{
    if(!path || !outputData)
        return false;

    /*
    If you have added output paths in the description block at the header of this file,
    then you will have to handle outputs here
    */

    return CGenericVisNodeBase::fetchOutput(path, outputData);
}

bool OsgSimpleViewVisNode::outputDerefed(IVisSystemNodeConnectionPath* path, IVisSystemNodeIOData* outputData)
{
    if(!path || !outputData)
        return false;

    /*
    If you have added output paths in the description block at the header of this file,
    then you will have to handle outputs here
    */

    return CGenericVisNodeBase::outputDerefed(path, outputData);
}

void OsgSimpleViewVisNode::removeSceneWidget()
{
	QTabWidget * osgOutputWidget = OsgCoreComponent::instance().osgOutputWidget();
	osgOutputWidget->removeTab(osgOutputWidget->indexOf(m_osgOutputWidget));
}

#ifdef ENABLE_ADVANCED_PROPERTIES

int OsgSimpleViewVisNode::propertyCount()
{
    return 0;
}

QString OsgSimpleViewVisNode::propertyName(int index)
{
    Q_UNUSED(index);
    return QString();
}

QString OsgSimpleViewVisNode::propertyGroup(int index)
{
    Q_UNUSED(index);
    return QString();
}

QVariant OsgSimpleViewVisNode::propertyValue(int index)
{
    Q_UNUSED(index);
    return QVariant();
}

void OsgSimpleViewVisNode::setPropertyValue(int index, QVariant value)
{
    Q_UNUSED(index);
    Q_UNUSED(value);
}

bool OsgSimpleViewVisNode::hasEditor(int index)
{
    Q_UNUSED(index);
    return false;
}

QWidget* OsgSimpleViewVisNode::createEditor(int index)
{
    Q_UNUSED(index);
    return 0;
}

void OsgSimpleViewVisNode::setEditorValue(int index, QWidget* widget, QVariant value)
{
    Q_UNUSED(index);
    Q_UNUSED(widget);
    Q_UNUSED(value);
}

QVariant OsgSimpleViewVisNode::editorValue(int index, QWidget* widget)
{
    Q_UNUSED(index);
    Q_UNUSED(widget);
}

void OsgSimpleViewVisNode::connectEditor(int index, QWidget* widget, QObject* receiver, const char* member)
{
    Q_UNUSED(index);
    Q_UNUSED(widget);
    Q_UNUSED(receiver);
    Q_UNUSED(member);
}

QString OsgSimpleViewVisNode::propertyValueString(int index)
{
    Q_UNUSED(index);
    return QString();
}

QVariant OsgSimpleViewVisNode::propertyValueFromString(int index, QString valueStr)
{
    Q_UNUSED(index);
    Q_UNUSED(valueStr);

    return QVariant();
}

bool OsgSimpleViewVisNode::canLoadSaveProperty(int index)
{
    Q_UNUSED(index);

    return false;
}

#endif

