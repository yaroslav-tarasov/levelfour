/****************************************************************************
**
** Copyright (C) L4
**
** Use of this file is limited according to the terms specified by
** L4.
**
** Details of those terms are listed in licence.txt included as
** part of the distribution package of this file. This file may not
** be distributed without including the licence.txt file.
**
** This file is provided AS IS with NO WARRANTY OF ANY KIND, INCLUDING THE
** WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
**
****************************************************************************/

#include "ViewportComponent.h"
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

#include "TableViewVisNode.h"
#include "TreeViewVisNode.h"
#include "ListViewVisNode.h"

struct ViewportComponentData
{
    ViewportComponentData() {
        nodeFactoryRegistryComp = 0;
        nodeFactoryRegistry = 0;
        visNetworkCanvas = 0;
        visNetwork = 0;
        scriptEngineManager = 0;
    }

    QMap<QString, CGenericVisNodeDesc*> nodeDescMap;
    GCF::AbstractComponent* nodeFactoryRegistryComp;
    IVisSystemNodeFactoryRegistry* nodeFactoryRegistry;
    IVisNetworkCanvas* visNetworkCanvas;
    IVisNetwork* visNetwork;
    IScriptEngineManager* scriptEngineManager;
};

GCF_DEFINE_COMPONENT(ViewportComponent)

ViewportComponent & ViewportComponent::instance()
{
    static ViewportComponent* theInstance = GCF_CREATE_COMPONENT(ViewportComponent);
    return *theInstance;
}

ViewportComponent::ViewportComponent()
{
    d = new ViewportComponentData;
}

ViewportComponent::~ViewportComponent()
{
    delete d;
}

QIcon ViewportComponent::nodeIcon()
{
    return qApp->windowIcon();
}

QObject* ViewportComponent::containerObject()
{
    return this;
}

QStringList ViewportComponent::nodeClassNameList()
{
    return d->nodeDescMap.keys();
}

IVisSystemNodeDesc* ViewportComponent::nodeDesc(QString nodeClassName)
{
    if(d->nodeDescMap.contains(nodeClassName))
        return d->nodeDescMap[nodeClassName];

    return 0;
}

bool ViewportComponent::canCreate(QString nodeClassName)
{
    IVisSystemNodeDesc* desc = nodeDesc(nodeClassName);

    CGenericVisNodeDesc* desc2 = 0;
    desc->queryInterface("CGenericVisNodeDesc", (void**)&desc2);
    if(desc2)
        return desc2->createFunction();

    return 0;
}

IVisSystemNode* ViewportComponent::createNode(QString nodeClassName)
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

void ViewportComponent::deleteNode(IVisSystemNode* node)
{
    node->finalizeNode();
}

QString ViewportComponent::productName() const
{
    return "Viewport Classes";
}

QString ViewportComponent::organization() const
{
    return "L4";
}

QImage ViewportComponent::programLogo() const
{
    return QImage();
}

QString ViewportComponent::version() const
{
    return "1.0";
}

QString ViewportComponent::shortDescription() const
{
    return "Provides the Viewport component";
}

QString ViewportComponent::homepage() const
{
    return "Viewport Website";
}

QString ViewportComponent::bugAddress() const
{
    return "Viewport Website";
}

const QList<GCF::AboutPerson> ViewportComponent::authors() const
{
    static QList<GCF::AboutPerson> retList;
    return retList;
}

const QList<GCF::AboutPerson> ViewportComponent::credits() const
{
    static QList<GCF::AboutPerson> retList;
    return retList;
}

QString ViewportComponent::license() const
{
    return "Viewport License";
}

QString ViewportComponent::copyrightStatement() const
{
    return "Copyright (c) L4 All rights reserved";
}

void ViewportComponent::initializeComponent()
{
    REGISTER_NODE(TableViewVisNode, d->nodeDescMap);
    REGISTER_NODE(TreeViewVisNode, d->nodeDescMap);
    REGISTER_NODE(ListViewVisNode, d->nodeDescMap);
}

void ViewportComponent::finalizeComponent()
{
    if(d->nodeFactoryRegistry)
        d->nodeFactoryRegistry->unregisterNodeFactory(qobject_cast<IVisSystemNodeFactory*>(this));
}

void ViewportComponent::creationAnnounced()
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

void ViewportComponent::componentDestroyed(GCF::AbstractComponent* comp)
{
    if(comp == d->nodeFactoryRegistryComp)
    {
        d->nodeFactoryRegistry = 0;
        d->nodeFactoryRegistryComp = 0;
    }
}

QObject* ViewportComponent::fetchObject(const QString& completeName) const
{
    QStringList comps = completeName.split('.');
    ViewportComponent* that = const_cast<ViewportComponent*>(this);

    if(comps.last() == "controller")
        return that;

    return 0;
}

QWidget* ViewportComponent::fetchWidget(const QString& completeName) const
{
    QStringList comps = completeName.split('.');

    return 0;
}

GCF_EXPORT_COMPONENT_PLUGIN(ViewportComponent)


