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

#include "OsgShadowComponent.h"
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

#include "OsgOccluderGeometryVisNode.h"

struct OsgShadowComponentData
{
    OsgShadowComponentData() {
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

GCF_DEFINE_COMPONENT(OsgShadowComponent)

OsgShadowComponent & OsgShadowComponent::instance()
{
    static OsgShadowComponent* theInstance = GCF_CREATE_COMPONENT(OsgShadowComponent);
    return *theInstance;
}

OsgShadowComponent::OsgShadowComponent()
{
    d = new OsgShadowComponentData;
}

OsgShadowComponent::~OsgShadowComponent()
{
    delete d;
}

QIcon OsgShadowComponent::nodeIcon()
{
    return qApp->windowIcon();
}

QObject* OsgShadowComponent::containerObject()
{
    return this;
}

QStringList OsgShadowComponent::nodeClassNameList()
{
    return d->nodeDescMap.keys();
}

IVisSystemNodeDesc* OsgShadowComponent::nodeDesc(QString nodeClassName)
{
    if(d->nodeDescMap.contains(nodeClassName))
        return d->nodeDescMap[nodeClassName];

    return 0;
}

bool OsgShadowComponent::canCreate(QString nodeClassName)
{
    IVisSystemNodeDesc* desc = nodeDesc(nodeClassName);

    CGenericVisNodeDesc* desc2 = 0;
    desc->queryInterface("CGenericVisNodeDesc", (void**)&desc2);
    if(desc2)
        return desc2->createFunction();

    return 0;
}

IVisSystemNode* OsgShadowComponent::createNode(QString nodeClassName)
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

void OsgShadowComponent::deleteNode(IVisSystemNode* node)
{
    node->finalizeNode();
}

QString OsgShadowComponent::productName() const
{
    return "OsgShadow Classes";
}

QString OsgShadowComponent::organization() const
{
    return "L4";
}

QImage OsgShadowComponent::programLogo() const
{
    return QImage();
}

QString OsgShadowComponent::version() const
{
    return "1.0";
}

QString OsgShadowComponent::shortDescription() const
{
    return "Provides the OsgShadow component";
}

QString OsgShadowComponent::homepage() const
{
    return "OsgShadow Website";
}

QString OsgShadowComponent::bugAddress() const
{
    return "OsgShadow Website";
}

const QList<GCF::AboutPerson> OsgShadowComponent::authors() const
{
    static QList<GCF::AboutPerson> retList;
    return retList;
}

const QList<GCF::AboutPerson> OsgShadowComponent::credits() const
{
    static QList<GCF::AboutPerson> retList;
    return retList;
}

QString OsgShadowComponent::license() const
{
    return "OsgShadow License";
}

QString OsgShadowComponent::copyrightStatement() const
{
    return "Copyright (c) L4 All rights reserved";
}

void OsgShadowComponent::initializeComponent()
{
    REGISTER_NODE(OsgOccluderGeometryVisNode, d->nodeDescMap);
}

void OsgShadowComponent::finalizeComponent()
{
    if(d->nodeFactoryRegistry)
        d->nodeFactoryRegistry->unregisterNodeFactory(qobject_cast<IVisSystemNodeFactory*>(this));
}

void OsgShadowComponent::creationAnnounced()
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

void OsgShadowComponent::componentDestroyed(GCF::AbstractComponent* comp)
{
    if(comp == d->nodeFactoryRegistryComp)
    {
        d->nodeFactoryRegistry = 0;
        d->nodeFactoryRegistryComp = 0;
    }
}

QObject* OsgShadowComponent::fetchObject(const QString& completeName) const
{
    QStringList comps = completeName.split('.');
    OsgShadowComponent* that = const_cast<OsgShadowComponent*>(this);

    if(comps.last() == "controller")
        return that;

    return 0;
}

QWidget* OsgShadowComponent::fetchWidget(const QString& completeName) const
{
    QStringList comps = completeName.split('.');

    return 0;
}

GCF_EXPORT_COMPONENT_PLUGIN(OsgShadowComponent)


