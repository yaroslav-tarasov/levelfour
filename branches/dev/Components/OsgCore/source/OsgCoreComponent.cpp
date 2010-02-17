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

#include "OsgCoreComponent.h"
#include "IVisSystemNodeFactoryRegistry.h"
#include "IVisSystemNodeDesc.h"
#include "IVisNetwork.h"
#include "IVisNetworkCanvas.h"
#include "IVisNetworkConnection.h"
#include "IScriptEngineManager.h"

#include <GCF/IComponentPlugin>
#include <GCF/ComponentFactory>
#include <QApplication>

#include "CGenericVisNodeDesc.h"
#include "CGenericVisNodeBase.h"

#include "OsgSimpleViewVisNode.h"
#include "OsgCompositeViewVisNode.h"
#include "OsgLightVisNode.h"
#include "OsgGroupVisNode.h"
#include "OsgGeodeVisNode.h"
#include "OsgBoxVisNode.h"
#include "OsgSphereVisNode.h"
#include "OsgConeVisNode.h"
#include "OsgCylinderVisNode.h"
#include "OsgSwitchVisNode.h"
#include "OsgPATVisNode.h"
#include "OsgModelVisNode.h"
#include "OsgImageVisNode.h"
#include "OsgCameraVisNode.h"
#include "OsgCapsuleVisNode.h"
#include "OsgBillboardVisNode.h"
#include "OsgInfinitePlaneVisNode.h"
#include "OsgTexture2DVisNode.h"

#include "QOSGGraphics.h"

#include <QWidget>
#include <QVBoxLayout>
#include <QToolBar>
#include <QComboBox>
#include <QAction>
#include <QStackedWidget>


struct OsgCoreComponentData
{
    OsgCoreComponentData() {
        nodeFactoryRegistryComp = 0;
        nodeFactoryRegistry = 0;
        visNetworkCanvas = 0;
        visNetwork = 0;
        scriptEngineManager = 0;
		osgOutputWidget = 0;
    }

    QMap<QString, CGenericVisNodeDesc*> nodeDescMap;
    GCF::AbstractComponent* nodeFactoryRegistryComp;
    IVisSystemNodeFactoryRegistry* nodeFactoryRegistry;
    IVisNetworkCanvas* visNetworkCanvas;
    IVisNetwork* visNetwork;
    IScriptEngineManager* scriptEngineManager;

	QTabWidget* sceneView;
	QWidget* osgOutputWidget;
	QVBoxLayout* sceneLayout;
	QStackedWidget* sceneStack;
	QToolBar* sceneToolBar;
	QComboBox* sceneSelection;
	QComboBox* sceneCameras;
	QComboBox* sceneDisplays;
	QComboBox* sceneViewports;
	
	// Actions for toolbar
	QAction* showgridAction;
	QAction* showgizmoAction;
	QAction* showaxisAction;
	QAction* saveviewAction;

	
};

GCF_DEFINE_COMPONENT(OsgCoreComponent)

OsgCoreComponent & OsgCoreComponent::instance()
{
    static OsgCoreComponent* theInstance = GCF_CREATE_COMPONENT(OsgCoreComponent);
    return *theInstance;
}

OsgCoreComponent::OsgCoreComponent()
{
    d = new OsgCoreComponentData;

	d->sceneLayout  = new QVBoxLayout;
	d->sceneStack = new QStackedWidget;
	d->sceneToolBar = new QToolBar;
	d->sceneSelection = new QComboBox;
	d->sceneCameras = new QComboBox;
	d->sceneDisplays = new QComboBox;
	d->sceneViewports = new QComboBox;
	d->osgOutputWidget = new QWidget;
	d->sceneView = new QTabWidget;

	// Toolbar actions
	
	// Scene selection provides toggle between different scene and data view nodes
	d->sceneSelection->addItem("Select Scene");
	d->sceneSelection->addItem("My Graph View");  // This should be a string passed as scene name given in the node
	d->sceneSelection->addItem("My Map View");
	d->sceneSelection->addItem("My Data View");
	// The combo box selects scenes by index
	connect(d->sceneSelection, SIGNAL(activated(int)),
             d->sceneStack, SLOT(setCurrentIndex(int)));

	// Camera actions (these are provided by the combo box)
	d->sceneCameras->addItem("Perspective");
	d->sceneCameras->addItem("Left");
	d->sceneCameras->addItem("Right");
	d->sceneCameras->addItem("Front");
	d->sceneCameras->addItem("Back");
	d->sceneCameras->addItem("Top");
	d->sceneCameras->addItem("Bottom");
	d->sceneCameras->addItem("Isometric");

	// Display actions (these are provided by the combo box)
	d->sceneDisplays->addItem("Wireframe");
	d->sceneDisplays->addItem("Textures");
	d->sceneDisplays->addItem("Backface Culling");
	d->sceneDisplays->addItem("Shading");

	// Viewport actions (these are provided by the combo box)
	d->sceneViewports->addItem("Single");
	d->sceneViewports->addItem("Over Under");
	d->sceneViewports->addItem("Side by Side");
	d->sceneViewports->addItem("2 Up 1 Down");
	d->sceneViewports->addItem("1 Up 2 Down");
	d->sceneViewports->addItem("2 by 2");

	// HUD actions
	d->showgridAction = new QAction(QIcon(":/OsgCore/grid.png"), tr("&Display Grid"), this);
	d->showgridAction->setStatusTip(tr("Displays a grid"));

	d->showgizmoAction = new QAction(QIcon(":/OsgCore/gizmo.png"), tr("&Display Gizmo"), this);
	d->showgizmoAction->setStatusTip(tr("Displays a gizmo control for navigation"));

	d->showaxisAction = new QAction(QIcon(":/OsgCore/axis.png"), tr("&Display Axis"), this);
	d->showaxisAction->setStatusTip(tr("Displays an axis for orientation"));

	// Save view options
	d->saveviewAction = new QAction(QIcon(":/LevelFour/camera.png"), tr("&Save view as..."), this);
	d->saveviewAction->setStatusTip(tr("Provides options for saving view in raster and vector formats"));
	
	// Add widgets and actions to the toolbar
	d->sceneToolBar->addWidget(d->sceneSelection);
	d->sceneToolBar->addWidget(d->sceneCameras);
	d->sceneToolBar->addWidget(d->sceneDisplays);
	d->sceneToolBar->addWidget(d->sceneViewports);
	d->sceneToolBar->addAction(d->showgridAction);
	d->sceneToolBar->addAction(d->showgizmoAction);
	d->sceneToolBar->addAction(d->showaxisAction);
	d->sceneToolBar->addAction(d->saveviewAction);

	// Assemble the panel
	d->sceneLayout->addWidget(d->sceneToolBar);
	d->sceneLayout->addWidget(d->sceneStack);
	// simple viewer will add widgets to scene stack as needed;
	d->sceneView->setLayout(d->sceneLayout);

}

OsgCoreComponent::~OsgCoreComponent()
{
    delete d;
}

QTabWidget* OsgCoreComponent::sceneView() const
{
	return d->sceneView;
}

QVBoxLayout* OsgCoreComponent::sceneLayout() const
{
	return d->sceneLayout;
}

QWidget* OsgCoreComponent::osgOutputWidget() const
{
	return d->osgOutputWidget;
}

QStackedWidget* OsgCoreComponent::sceneStack() const
{
	return d->sceneStack;
}

QIcon OsgCoreComponent::nodeIcon()
{
    return qApp->windowIcon();
}

QObject* OsgCoreComponent::containerObject()
{
    return this;
}

QStringList OsgCoreComponent::nodeClassNameList()
{
    return d->nodeDescMap.keys();
}

IVisSystemNodeDesc* OsgCoreComponent::nodeDesc(QString nodeClassName)
{
    if(d->nodeDescMap.contains(nodeClassName))
        return d->nodeDescMap[nodeClassName];

    return 0;
}

bool OsgCoreComponent::canCreate(QString nodeClassName)
{
    IVisSystemNodeDesc* desc = nodeDesc(nodeClassName);

    CGenericVisNodeDesc* desc2 = 0;
    desc->queryInterface("CGenericVisNodeDesc", (void**)&desc2);
    if(desc2)
        return desc2->createFunction();

    return 0;
}

IVisSystemNode* OsgCoreComponent::createNode(QString nodeClassName)
{
    IVisSystemNodeDesc* desc = nodeDesc(nodeClassName);

    CGenericVisNodeDesc* desc2 = 0;
    desc->queryInterface("CGenericVisNodeDesc", (void**)&desc2);
    if(!desc2)
        return false;

    IVisSystemNode* node = desc2->createNode();
    if(node)
    {
        node->initializeNode();
        CGenericVisNodeBase* nodeBase = qobject_cast<CGenericVisNodeBase*>(node->containerObject());
        if(nodeBase)
            nodeBase->setNodeFactory(this);
    }

    return node;
}

void OsgCoreComponent::deleteNode(IVisSystemNode* node)
{
    node->finalizeNode();
}

QString OsgCoreComponent::productName() const
{
    return "OsgCore Classes";
}

QString OsgCoreComponent::organization() const
{
    return "UNO";
}

QImage OsgCoreComponent::programLogo() const
{
    return QImage();
}

QString OsgCoreComponent::version() const
{
    return "1.0";
}

QString OsgCoreComponent::shortDescription() const
{
    return "Provides the OsgCore component";
}

QString OsgCoreComponent::homepage() const
{
    return "OsgCore Website";
}

QString OsgCoreComponent::bugAddress() const
{
    return "OsgCore Website";
}

const QList<GCF::AboutPerson> OsgCoreComponent::authors() const
{
    static QList<GCF::AboutPerson> retList;
    return retList;
}

const QList<GCF::AboutPerson> OsgCoreComponent::credits() const
{
    static QList<GCF::AboutPerson> retList;
    return retList;
}

QString OsgCoreComponent::license() const
{
    return "OsgCore License";
}

QString OsgCoreComponent::copyrightStatement() const
{
    return "Copyright (c) UNO All rights reserved";
}

void OsgCoreComponent::initializeComponent()
{
    REGISTER_NODE(OsgSimpleViewVisNode, d->nodeDescMap);
    REGISTER_NODE(OsgGroupVisNode, d->nodeDescMap);
    REGISTER_NODE(OsgLightVisNode, d->nodeDescMap);
	REGISTER_NODE(OsgGeodeVisNode, d->nodeDescMap);
	REGISTER_NODE(OsgBoxVisNode, d->nodeDescMap);
	REGISTER_NODE(OsgSphereVisNode, d->nodeDescMap);
	REGISTER_NODE(OsgConeVisNode, d->nodeDescMap);
	REGISTER_NODE(OsgCylinderVisNode, d->nodeDescMap);
	REGISTER_NODE(OsgSwitchVisNode, d->nodeDescMap);
	REGISTER_NODE(OsgPATVisNode, d->nodeDescMap);
	REGISTER_NODE(OsgModelVisNode, d->nodeDescMap);

	REGISTER_NODE(OsgBillboardVisNode, d->nodeDescMap);
	REGISTER_NODE(OsgCameraVisNode, d->nodeDescMap);
	REGISTER_NODE(OsgImageVisNode, d->nodeDescMap);
	REGISTER_NODE(OsgCapsuleVisNode, d->nodeDescMap);
	REGISTER_NODE(OsgInfinitePlaneVisNode, d->nodeDescMap);
	REGISTER_NODE(OsgTexture2DVisNode, d->nodeDescMap);
/*    REGISTER_NODE(OsgCompositeViewVisNode, d->nodeDescMap);
*/}

void OsgCoreComponent::finalizeComponent()
{
    if(d->nodeFactoryRegistry)
        d->nodeFactoryRegistry->unregisterNodeFactory(qobject_cast<IVisSystemNodeFactory*>(this));
}

void OsgCoreComponent::creationAnnounced()
{
    QString completeName;
    GCF::AbstractComponent* compPtr = 0;
    QObject* objectPtr = findObject("IVisSystemNodeFactoryRegistry", completeName, &compPtr);
    if(objectPtr)
    {
        IVisSystemNodeFactoryRegistry* nodeFacReg = qobject_cast<IVisSystemNodeFactoryRegistry*>(objectPtr);
        nodeFacReg->registerNodeFactory(qobject_cast<IVisSystemNodeFactory*>(this));
        d->nodeFactoryRegistry = nodeFacReg;
        d->nodeFactoryRegistryComp = compPtr;
        GCF::AbstractComponent::addDependency(this, compPtr);
    }

    objectPtr = findObject("IVisNetwork", completeName, &compPtr);
    if(objectPtr)
    {
        d->visNetwork = qobject_cast<IVisNetwork*>(objectPtr);
        // This should be done to ensure that this component is finalized
        // after the network is destroyed.
        GCF::AbstractComponent::addDependency(this, compPtr);
    }

    objectPtr = findObject("IVisNetworkCanvas", completeName, &compPtr);
    if(objectPtr)
    {
        d->visNetworkCanvas = qobject_cast<IVisNetworkCanvas*>(objectPtr);
        // This should be done to ensure that this component is finalized
        // after the network is destroyed.
        GCF::AbstractComponent::addDependency(this, compPtr);
    }

    objectPtr = findObject("IScriptEngineManager", completeName, &compPtr);
    if(objectPtr)
    {
        d->scriptEngineManager = qobject_cast<IScriptEngineManager*>(objectPtr);
        // This should be done to ensure that this component is finalized
        // after the network is destroyed.
        GCF::AbstractComponent::addDependency(this, compPtr);
    }
}

void OsgCoreComponent::componentDestroyed(GCF::AbstractComponent* comp)
{
    if(comp == d->nodeFactoryRegistryComp)
    {
        d->nodeFactoryRegistry = 0;
        d->nodeFactoryRegistryComp = 0;
    }
}

QObject* OsgCoreComponent::fetchObject(const QString& completeName) const
{
    QStringList comps = completeName.split('.');
    OsgCoreComponent* that = const_cast<OsgCoreComponent*>(this);

    if(comps.last() == "controller")
        return that;

    return 0;
}

QWidget* OsgCoreComponent::fetchWidget(const QString& completeName) const
{
    QStringList comps = completeName.split('.');

	if(comps.last() == "sceneView")
		return d->sceneView;

    return 0;
}

GCF_EXPORT_COMPONENT_PLUGIN(OsgCoreComponent)


