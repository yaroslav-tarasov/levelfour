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

#include "OsgFXComponent.h"
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

#include "OsgAnisotropicLightingVisNode.h"
#include "OsgBumpMappingVisNode.h"
#include "OsgCartoonVisNode.h"
#include "OsgMultiTextureControlVisNode.h"
#include "OsgScribeVisNode.h"
#include "OsgSpecularHighlightsVisNode.h"

struct OsgFXComponentData
{
    OsgFXComponentData() {
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

GCF_DEFINE_COMPONENT(OsgFXComponent)

OsgFXComponent & OsgFXComponent::instance()
{
    static OsgFXComponent* theInstance = GCF_CREATE_COMPONENT(OsgFXComponent);
    return *theInstance;
}

OsgFXComponent::OsgFXComponent()
{
    d = new OsgFXComponentData;
}

OsgFXComponent::~OsgFXComponent()
{
    delete d;
}

QIcon OsgFXComponent::nodeIcon()
{
    return qApp->windowIcon();
}

QObject* OsgFXComponent::containerObject()
{
    return this;
}

QStringList OsgFXComponent::nodeClassNameList()
{
    return d->nodeDescMap.keys();
}

IVisSystemNodeDesc* OsgFXComponent::nodeDesc(QString nodeClassName)
{
    if(d->nodeDescMap.contains(nodeClassName))
        return d->nodeDescMap[nodeClassName];

    return 0;
}

bool OsgFXComponent::canCreate(QString nodeClassName)
{
    IVisSystemNodeDesc* desc = nodeDesc(nodeClassName);

    CGenericVisNodeDesc* desc2 = 0;
    desc->queryInterface("CGenericVisNodeDesc", (void**)&desc2);
    if(desc2)
        return desc2->createFunction();

    return 0;
}

IVisSystemNode* OsgFXComponent::createNode(QString nodeClassName)
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

void OsgFXComponent::deleteNode(IVisSystemNode* node)
{
    node->finalizeNode();
}

QString OsgFXComponent::productName() const
{
    return "OsgFX Classes";
}

QString OsgFXComponent::organization() const
{
    return "L4";
}

QImage OsgFXComponent::programLogo() const
{
    return QImage();
}

QString OsgFXComponent::version() const
{
    return "1.0";
}

QString OsgFXComponent::shortDescription() const
{
    return "Provides the OsgFX component";
}

QString OsgFXComponent::homepage() const
{
    return "OsgFX Website";
}

QString OsgFXComponent::bugAddress() const
{
    return "OsgFX Website";
}

const QList<GCF::AboutPerson> OsgFXComponent::authors() const
{
    static QList<GCF::AboutPerson> retList;
    return retList;
}

const QList<GCF::AboutPerson> OsgFXComponent::credits() const
{
    static QList<GCF::AboutPerson> retList;
    return retList;
}

QString OsgFXComponent::license() const
{
    return "OsgFX License";
}

QString OsgFXComponent::copyrightStatement() const
{
    return "Copyright (c) L4 All rights reserved";
}

void OsgFXComponent::initializeComponent()
{
    REGISTER_NODE(OsgAnisotropicLightingVisNode, d->nodeDescMap);
    REGISTER_NODE(OsgBumpMappingVisNode, d->nodeDescMap);
    REGISTER_NODE(OsgCartoonVisNode, d->nodeDescMap);
    REGISTER_NODE(OsgMultiTextureControlVisNode, d->nodeDescMap);
    REGISTER_NODE(OsgScribeVisNode, d->nodeDescMap);
    REGISTER_NODE(OsgSpecularHighlightsVisNode, d->nodeDescMap);
}

void OsgFXComponent::finalizeComponent()
{
    if(d->nodeFactoryRegistry)
        d->nodeFactoryRegistry->unregisterNodeFactory(qobject_cast<IVisSystemNodeFactory*>(this));
}

void OsgFXComponent::creationAnnounced()
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

void OsgFXComponent::componentDestroyed(GCF::AbstractComponent* comp)
{
    if(comp == d->nodeFactoryRegistryComp)
    {
        d->nodeFactoryRegistry = 0;
        d->nodeFactoryRegistryComp = 0;
    }
}

QObject* OsgFXComponent::fetchObject(const QString& completeName) const
{
    QStringList comps = completeName.split('.');
    OsgFXComponent* that = const_cast<OsgFXComponent*>(this);

    if(comps.last() == "controller")
        return that;

    return 0;
}

QWidget* OsgFXComponent::fetchWidget(const QString& completeName) const
{
    QStringList comps = completeName.split('.');

    return 0;
}

GCF_EXPORT_COMPONENT_PLUGIN(OsgFXComponent)


