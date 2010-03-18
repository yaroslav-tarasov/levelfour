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

#include "D2MComponent.h"
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

#include "sampleGraphVisNode.h"
#include "circleGraphLayoutVisNode.h"

struct D2MComponentData
{
    D2MComponentData() {
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

GCF_DEFINE_COMPONENT(D2MComponent)

D2MComponent & D2MComponent::instance()
{
    static D2MComponent* theInstance = GCF_CREATE_COMPONENT(D2MComponent);
    return *theInstance;
}

D2MComponent::D2MComponent()
{
    d = new D2MComponentData;
}

D2MComponent::~D2MComponent()
{
    delete d;
}

QIcon D2MComponent::nodeIcon()
{
    return qApp->windowIcon();
}

QObject* D2MComponent::containerObject()
{
    return this;
}

QStringList D2MComponent::nodeClassNameList()
{
    return d->nodeDescMap.keys();
}

IVisSystemNodeDesc* D2MComponent::nodeDesc(QString nodeClassName)
{
    if(d->nodeDescMap.contains(nodeClassName))
        return d->nodeDescMap[nodeClassName];

    return 0;
}

bool D2MComponent::canCreate(QString nodeClassName)
{
    IVisSystemNodeDesc* desc = nodeDesc(nodeClassName);

    CGenericVisNodeDesc* desc2 = 0;
    desc->queryInterface("CGenericVisNodeDesc", (void**)&desc2);
    if(desc2)
        return desc2->createFunction();

    return 0;
}

IVisSystemNode* D2MComponent::createNode(QString nodeClassName)
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

void D2MComponent::deleteNode(IVisSystemNode* node)
{
    node->finalizeNode();
}

QString D2MComponent::productName() const
{
    return "D2M Classes";
}

QString D2MComponent::organization() const
{
    return "UNO";
}

QImage D2MComponent::programLogo() const
{
    return QImage();
}

QString D2MComponent::version() const
{
    return "1.0";
}

QString D2MComponent::shortDescription() const
{
    return "Provides the D2M component";
}

QString D2MComponent::homepage() const
{
    return "D2M Website";
}

QString D2MComponent::bugAddress() const
{
    return "D2M Website";
}

const QList<GCF::AboutPerson> D2MComponent::authors() const
{
    static QList<GCF::AboutPerson> retList;
    return retList;
}

const QList<GCF::AboutPerson> D2MComponent::credits() const
{
    static QList<GCF::AboutPerson> retList;
    return retList;
}

QString D2MComponent::license() const
{
    return "D2M License";
}

QString D2MComponent::copyrightStatement() const
{
    return "Copyright (c) UNO All rights reserved";
}

void D2MComponent::initializeComponent()
{
    REGISTER_NODE(sampleGraphVisNode, d->nodeDescMap);
    REGISTER_NODE(circleGraphLayoutVisNode, d->nodeDescMap);
}

void D2MComponent::finalizeComponent()
{
    if(d->nodeFactoryRegistry)
        d->nodeFactoryRegistry->unregisterNodeFactory(qobject_cast<IVisSystemNodeFactory*>(this));
}

void D2MComponent::creationAnnounced()
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

void D2MComponent::componentDestroyed(GCF::AbstractComponent* comp)
{
    if(comp == d->nodeFactoryRegistryComp)
    {
        d->nodeFactoryRegistry = 0;
        d->nodeFactoryRegistryComp = 0;
    }
}

QObject* D2MComponent::fetchObject(const QString& completeName) const
{
    QStringList comps = completeName.split('.');
    D2MComponent* that = const_cast<D2MComponent*>(this);

    if(comps.last() == "controller")
        return that;

    return 0;
}

QWidget* D2MComponent::fetchWidget(const QString& completeName) const
{
    QStringList comps = completeName.split('.');

    return 0;
}

GCF_EXPORT_COMPONENT_PLUGIN(D2MComponent)


