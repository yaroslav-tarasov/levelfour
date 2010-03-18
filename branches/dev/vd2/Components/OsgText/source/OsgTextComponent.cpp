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

#include "OsgTextComponent.h"
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

#include "OsgFadeTextVisNode.h"
#include "OsgStringVisNode.h"
#include "OsgTextVisNode.h"
#include "OsgText3DVisNode.h"

struct OsgTextComponentData
{
    OsgTextComponentData() {
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

GCF_DEFINE_COMPONENT(OsgTextComponent)

OsgTextComponent & OsgTextComponent::instance()
{
    static OsgTextComponent* theInstance = GCF_CREATE_COMPONENT(OsgTextComponent);
    return *theInstance;
}

OsgTextComponent::OsgTextComponent()
{
    d = new OsgTextComponentData;
}

OsgTextComponent::~OsgTextComponent()
{
    delete d;
}

QIcon OsgTextComponent::nodeIcon()
{
    return qApp->windowIcon();
}

QObject* OsgTextComponent::containerObject()
{
    return this;
}

QStringList OsgTextComponent::nodeClassNameList()
{
    return d->nodeDescMap.keys();
}

IVisSystemNodeDesc* OsgTextComponent::nodeDesc(QString nodeClassName)
{
    if(d->nodeDescMap.contains(nodeClassName))
        return d->nodeDescMap[nodeClassName];

    return 0;
}

bool OsgTextComponent::canCreate(QString nodeClassName)
{
    IVisSystemNodeDesc* desc = nodeDesc(nodeClassName);

    CGenericVisNodeDesc* desc2 = 0;
    desc->queryInterface("CGenericVisNodeDesc", (void**)&desc2);
    if(desc2)
        return desc2->createFunction();

    return 0;
}

IVisSystemNode* OsgTextComponent::createNode(QString nodeClassName)
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

void OsgTextComponent::deleteNode(IVisSystemNode* node)
{
    node->finalizeNode();
}

QString OsgTextComponent::productName() const
{
    return "OsgText Classes";
}

QString OsgTextComponent::organization() const
{
    return "L4";
}

QImage OsgTextComponent::programLogo() const
{
    return QImage();
}

QString OsgTextComponent::version() const
{
    return "1.0";
}

QString OsgTextComponent::shortDescription() const
{
    return "Provides the OsgText component";
}

QString OsgTextComponent::homepage() const
{
    return "OsgText Website";
}

QString OsgTextComponent::bugAddress() const
{
    return "OsgText Website";
}

const QList<GCF::AboutPerson> OsgTextComponent::authors() const
{
    static QList<GCF::AboutPerson> retList;
    return retList;
}

const QList<GCF::AboutPerson> OsgTextComponent::credits() const
{
    static QList<GCF::AboutPerson> retList;
    return retList;
}

QString OsgTextComponent::license() const
{
    return "OsgText License";
}

QString OsgTextComponent::copyrightStatement() const
{
    return "Copyright (c) L4 All rights reserved";
}

void OsgTextComponent::initializeComponent()
{
    REGISTER_NODE(OsgFadeTextVisNode, d->nodeDescMap);
    REGISTER_NODE(OsgStringVisNode, d->nodeDescMap);
    REGISTER_NODE(OsgTextVisNode, d->nodeDescMap);
    REGISTER_NODE(OsgText3DVisNode, d->nodeDescMap);
}

void OsgTextComponent::finalizeComponent()
{
    if(d->nodeFactoryRegistry)
        d->nodeFactoryRegistry->unregisterNodeFactory(qobject_cast<IVisSystemNodeFactory*>(this));
}

void OsgTextComponent::creationAnnounced()
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

void OsgTextComponent::componentDestroyed(GCF::AbstractComponent* comp)
{
    if(comp == d->nodeFactoryRegistryComp)
    {
        d->nodeFactoryRegistry = 0;
        d->nodeFactoryRegistryComp = 0;
    }
}

QObject* OsgTextComponent::fetchObject(const QString& completeName) const
{
    QStringList comps = completeName.split('.');
    OsgTextComponent* that = const_cast<OsgTextComponent*>(this);

    if(comps.last() == "controller")
        return that;

    return 0;
}

QWidget* OsgTextComponent::fetchWidget(const QString& completeName) const
{
    QStringList comps = completeName.split('.');

    return 0;
}

GCF_EXPORT_COMPONENT_PLUGIN(OsgTextComponent)


