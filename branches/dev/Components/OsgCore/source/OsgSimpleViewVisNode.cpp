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
#include "MainWindowComponent.h"

#include <osgDB/WriteFile>
#include <osgDB/ReadFile>
#include <osg/Node>
#include <osg/ref_ptr>
#include <osg/LightSource>

#include <QTabWidget>
#include <QSize>

#include <osgEarthUtil/EarthManipulator>
#include <osgGA/TrackballManipulator>

DEFINE_VIS_NODE(OsgSimpleViewVisNode, CGenericVisNodeBase)
{
    pDesc->setNodeClassCategory("Renderers");
    pDesc->setNodeClassName("Scene View");
    pDesc->setNodeClassDescription("A view to render a scene.");
    pDesc->setNodeIcon( QIcon(":/OsgCore/SceneView.png") );

    // Uncomment and use the following code template to add input/output paths
    
    pDesc->addConnectionPath(
        new CGenericVisNodeConnectionPath(
                "OsgNodeInput",                                 // Name of the path
                IVisSystemNodeConnectionPath::InputPath,   // Path type can be OutputPath or InputPath
                "osg::ref_ptr<osg::Node>",                                 // Data type of the path
                0,                                          // Path index (don't change)
                true                                       // Allow Multiple Inputs Flag
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
	OsgSimpleViewVisNodeData() : scene(0), root(0) {}

#if USE_QOSG == QOSG_WIDGET
	ViewerQOSG * scene;
#elif USE_QOSG == QOSG_GRAPHICS
	osg::QOSGScene * scene;
#else
	ViewerQT * scene;
#endif

	OsgGroupVisNodeIOData inputData;
	osg::ref_ptr<osg::Light> inputLight;
	osg::ref_ptr<osg::Group> root;
};

OsgSimpleViewVisNode::OsgSimpleViewVisNode() 
{
    OsgSimpleViewVisNode::InitializeNodeDesc();
    d = new OsgSimpleViewVisNodeData;

	d->root = new osg::Group;

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

#if USE_QOSG == QOSG_WIDGET || USE_QOSG == QOSG_ADAPTER
	m_osgOutputWidget = new QWidget;
	d->scene = new ViewerQOSG(m_osgOutputWidget);
	d->scene->updateCamera();
	d->scene->setCameraManipulator(new osgGA::TrackballManipulator);
	d->scene->setSceneData(d->root.get());
#else
	m_osgOutputWidget = new osg::QGLGraphicsView;
	d->scene = new osg::QOSGScene;
	m_osgOutputWidget->setScene(d->scene);
	d->scene->setCameraManipulator(new osgEarthUtil::EarthManipulator);
	d->scene->setLight(d->inputLight);
#endif
	OsgCoreComponent::instance().sceneStack()->addWidget(m_osgOutputWidget);

	QSize s = m_osgOutputWidget->parentWidget()->size();
	if (d->scene)
#ifdef USE_QOSG == QOSG_WIDGET
		d->scene->setGeometry(0, 0, s.width(), s.height());
#elif USE_QOSG == QOSG_GRAPHICS
		d->scene->setSceneRect(0, 0, s.width(), s.height());
#endif
}

OsgSimpleViewVisNode::~OsgSimpleViewVisNode()
{
    // This would be a good time to delete the backend object. If the backend object is a vtkObject
    // subclass, then you have to delete it now.
    // d->VtkObjectPointer->Delete();

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
#if USE_QOSG == QOSG_WIDGET
	d->scene = new ViewerQOSG(m_osgOutputWidget);
	d->scene->updateCamera();
	d->scene->setCameraManipulator(new osgGA::TrackballManipulator);
#elif  USE_QOSG == QOSG_GRAPHICS
	d->scene = new osg::QOSGScene;
	m_osgOutputWidget->setScene(d->scene);
	d->scene->setCameraManipulator(new osgGA::TrackballManipulator);
	d->scene->setLight(d->inputLight);
#else  USE_QOSG == QOSG_ADAPTER
	d->scene = new ViewerQT(m_osgOutputWidget);
	d->scene->setCameraManipulator(new osgEarthUtil::EarthManipulator);
#endif

	d->scene->setSceneData(d->root.get());
	QSize s = m_osgOutputWidget->parentWidget()->size();

	if (d->scene)
#ifdef USE_QOSG == QOSG_WIDGET
		d->scene->setGeometry(0, 0, s.width(), s.height());
#elif USE_QOSG == QOSG_GRAPHICS
		d->scene->setSceneRect(0, 0, s.width(), s.height());
#endif
}

void OsgSimpleViewVisNode::saveOSG()
{
    command_SaveOSG();
}

void OsgSimpleViewVisNode::command_SaveOSG()
{
#if USE_QOSG == QOSG_WIDGET || USE_QOSG == QOSG_ADAPTER
	if (!d->root)
		return;
	osg::ref_ptr<osg::Node> root = d->root;
#elif USE_QOSG == QOSG_GRAPHICS
	if (!d->scene)
		return;
	osg::ref_ptr<osg::Node> root = d->scene->getSceneData();
#endif

	if (!root.valid())
		return;

	osgDB::writeNodeFile(*(root.get()), "test.osg");
	system("D:\\OSG-2.9.6\\bin\\osgviewerQTd.exe test.osg");
}

void OsgSimpleViewVisNode::EarthManipulator()
{
    command_EarthManipulator();
}

void OsgSimpleViewVisNode::command_EarthManipulator()
{
	if (d->scene)
		d->scene->setCameraManipulator(new osgEarthUtil::EarthManipulator);
}

void OsgSimpleViewVisNode::addNode(osg::Node * node)
{
	if (! d->root)
		d->root = new osg::Group;

	if (d->root->containsNode(node))
		return;

	d->root->addChild(node);
}

void OsgSimpleViewVisNode::removeNode(osg::Node * node)
{
	if (! d->root || !d->root->containsNode(node))
		return;

	d->root->removeChild(node);
}

bool OsgSimpleViewVisNode::containsNode(osg::Node * node)
{
	return d->root->containsNode(node);
}

bool OsgSimpleViewVisNode::hasInput(IVisSystemNodeConnectionPath* path)
{
    if(!path)
        return false;

    /*
    If you have added input paths in the description block at the header of this file,
    then you will have to handle inputs here
    */

	if (path->pathName() == "OsgNodeInput")
		return d->root != 0;
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

	if (path->pathName() == "OsgNodeInput")
	{
		OsgGroupVisNodeIOData * data = 0;
		bool success = inputData->queryInterface("OsgNodeVisNodeIOData", (void**)&data);
		if (success && data)
		{
			d->root->addChild(data->getOsgNode());
			d->inputData.setOsgNode(d->root);

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

	if (path->pathName() == "OsgNodeInput")
	{
		OsgGroupVisNodeIOData * data = 0;
		bool success = inputData->queryInterface("OsgNodeVisNodeIOData", (void**)&data);
		if (success && data && d->root->containsNode(data->getOsgNode()))
		{
			d->root->removeChild(data->getOsgNode());
			d->inputData.setOsgNode(d->root);

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

