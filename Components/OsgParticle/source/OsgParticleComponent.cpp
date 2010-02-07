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

#include "OsgParticleComponent.h"
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

#include "OsgSmokeEffectVisNode.h"
#include "OsgFireEffectVisNode.h"
#include "OsgExplosionEffectVisNode.h"
#include "OsgPrecipitationEffectVisNode.h"
#include "OsgBoxPlacerVisNode.h"
#include "OsgCenteredPlacerVisNode.h"
#include "OsgConnectedParticleSystemVisNode.h"
#include "OsgForceOperatorVisNode.h"
#include "OsgParticleVisNode.h"
#include "OsgPointPlacerVisNode.h"
#include "OsgRadialShooterVisNode.h"
#include "OsgSectorPlacerVisNode.h"
#include "OsgSegmentPlacerVisNode.h"
#include "OsgRandomRateCounterVisNode.h"
#include "OsgVariableRateCounterVisNode.h"
#include "OsgModularEmitterVisNode.h"

struct OsgParticleComponentData
{
    OsgParticleComponentData() {
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

GCF_DEFINE_COMPONENT(OsgParticleComponent)

OsgParticleComponent & OsgParticleComponent::instance()
{
    static OsgParticleComponent* theInstance = GCF_CREATE_COMPONENT(OsgParticleComponent);
    return *theInstance;
}

OsgParticleComponent::OsgParticleComponent()
{
    d = new OsgParticleComponentData;
}

OsgParticleComponent::~OsgParticleComponent()
{
    delete d;
}

QIcon OsgParticleComponent::nodeIcon()
{
    return qApp->windowIcon();
}

QObject* OsgParticleComponent::containerObject()
{
    return this;
}

QStringList OsgParticleComponent::nodeClassNameList()
{
    return d->nodeDescMap.keys();
}

IVisSystemNodeDesc* OsgParticleComponent::nodeDesc(QString nodeClassName)
{
    if(d->nodeDescMap.contains(nodeClassName))
        return d->nodeDescMap[nodeClassName];

    return 0;
}

bool OsgParticleComponent::canCreate(QString nodeClassName)
{
    IVisSystemNodeDesc* desc = nodeDesc(nodeClassName);

    CGenericVisNodeDesc* desc2 = 0;
    desc->queryInterface("CGenericVisNodeDesc", (void**)&desc2);
    if(desc2)
        return desc2->createFunction();

    return 0;
}

IVisSystemNode* OsgParticleComponent::createNode(QString nodeClassName)
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

void OsgParticleComponent::deleteNode(IVisSystemNode* node)
{
    node->finalizeNode();
}

QString OsgParticleComponent::productName() const
{
    return "OsgParticle Classes";
}

QString OsgParticleComponent::organization() const
{
    return "L4";
}

QImage OsgParticleComponent::programLogo() const
{
    return QImage();
}

QString OsgParticleComponent::version() const
{
    return "1.0";
}

QString OsgParticleComponent::shortDescription() const
{
    return "Provides the OsgParticle component";
}

QString OsgParticleComponent::homepage() const
{
    return "OsgParticle Website";
}

QString OsgParticleComponent::bugAddress() const
{
    return "OsgParticle Website";
}

const QList<GCF::AboutPerson> OsgParticleComponent::authors() const
{
    static QList<GCF::AboutPerson> retList;
    return retList;
}

const QList<GCF::AboutPerson> OsgParticleComponent::credits() const
{
    static QList<GCF::AboutPerson> retList;
    return retList;
}

QString OsgParticleComponent::license() const
{
    return "OsgParticle License";
}

QString OsgParticleComponent::copyrightStatement() const
{
    return "Copyright (c) L4 All rights reserved";
}

void OsgParticleComponent::initializeComponent()
{
    REGISTER_NODE(OsgSmokeEffectVisNode, d->nodeDescMap);
    REGISTER_NODE(OsgFireEffectVisNode, d->nodeDescMap);
    REGISTER_NODE(OsgExplosionEffectVisNode, d->nodeDescMap);
    REGISTER_NODE(OsgPrecipitationEffectVisNode, d->nodeDescMap);
    REGISTER_NODE(OsgBoxPlacerVisNode, d->nodeDescMap);
    REGISTER_NODE(OsgCenteredPlacerVisNode, d->nodeDescMap);
    REGISTER_NODE(OsgConnectedParticleSystemVisNode, d->nodeDescMap);
    REGISTER_NODE(OsgForceOperatorVisNode, d->nodeDescMap);
    REGISTER_NODE(OsgParticleVisNode, d->nodeDescMap);
    REGISTER_NODE(OsgPointPlacerVisNode, d->nodeDescMap);
    REGISTER_NODE(OsgRadialShooterVisNode, d->nodeDescMap);
    REGISTER_NODE(OsgSectorPlacerVisNode, d->nodeDescMap);
    REGISTER_NODE(OsgSegmentPlacerVisNode, d->nodeDescMap);
    REGISTER_NODE(OsgRandomRateCounterVisNode, d->nodeDescMap);
    REGISTER_NODE(OsgVariableRateCounterVisNode, d->nodeDescMap);
    REGISTER_NODE(OsgModularEmitterVisNode, d->nodeDescMap);
}

void OsgParticleComponent::finalizeComponent()
{
    if(d->nodeFactoryRegistry)
        d->nodeFactoryRegistry->unregisterNodeFactory(qobject_cast<IVisSystemNodeFactory*>(this));
}

void OsgParticleComponent::creationAnnounced()
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

void OsgParticleComponent::componentDestroyed(GCF::AbstractComponent* comp)
{
    if(comp == d->nodeFactoryRegistryComp)
    {
        d->nodeFactoryRegistry = 0;
        d->nodeFactoryRegistryComp = 0;
    }
}

QObject* OsgParticleComponent::fetchObject(const QString& completeName) const
{
    QStringList comps = completeName.split('.');
    OsgParticleComponent* that = const_cast<OsgParticleComponent*>(this);

    if(comps.last() == "controller")
        return that;

    return 0;
}

QWidget* OsgParticleComponent::fetchWidget(const QString& completeName) const
{
    QStringList comps = completeName.split('.');

    return 0;
}

GCF_EXPORT_COMPONENT_PLUGIN(OsgParticleComponent)


