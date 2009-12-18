/****************************************************************************
**
** Copyright (C) LevelFour
**
** Use of this file is limited according to the terms specified by
** LevelFour.
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
#include <GCF/ComponentFactory>
#include <GCF/IComponentPlugin>

GCF_DEFINE_COMPONENT(D2MComponent)

struct D2MComponentData
{

};

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

/*
This method is called before the GUIXML of the component is read, and 
just after the component object has been created. At this point the 
component settings has not been read/initialized. So dont use
GCF::AbstractComponent::settings() within this function
*/
void D2MComponent::initializeComponent()
{
    // Do nothing.
}

/*
This method is called just before the UI of the component is unloaded.
It is safe to use GCF::AbstractComponent::settings() here. Anything
you write into settings() in this function will be stored and loaded
back when the component class is used next
*/
void D2MComponent::finalizeComponent()
{
    // Do nothing
}

/*
This function is called after all the other components in the system 
are informed about the creation of this component. 

At this point the GUIXML of the component is read and processed. 
initializeComponent() will have already been called. aboutToActivate()
and justActivated() may not have been called.

This is the best time to hook up with objects exposed by other components.
Example:

    IPropertyEditor* propEditor = GCF::findObject<IPropertyEditor*>();
*/
void D2MComponent::creationAnnounced()
{
    // Do nothing
}

/*
This function is called when the destruction of this component has been
announced to other components in the system. This function is called
before finalizeComponent(), that means the UI of the component is 
still available. 

You can write into settings() within this function if needed.
*/
void D2MComponent::destructionAnnounced()
{
    // Do nothing
}

/*
This function is called before the component is about to be activated.
GUIXML of the component is read and processed before this function call.
*/
void D2MComponent::aboutToActivate()
{
    // Do nothing
}

/*
This function is called just after the component has been activated.
*/
void D2MComponent::justActivated()
{
    // Do nothing
}

/*
This function is called just before the component is about to get 
deactivated.
*/
void D2MComponent::aboutToDeactivate()
{
    // Do nothing
}

/*
This function is called just after the component has been deactivated.
*/
void D2MComponent::justDeactivated()
{
    // Do nothing
}

/*
This function is called when another component (pointed by comp) has
been created and initalized in the system. 

Within this function you can look for objects that implement known
interfaces within the new component.

Example:

    QList<IProjectPart*> projectParts = GCF::findObjects<IProjectPart*>(comp);
    Q_FOREACH(IProjectPart* projectPart, projectParts)
    {
        // Do something with projectPart
    }
*/
void D2MComponent::componentCreated(AbstractComponent* comp)
{
    Q_UNUSED(comp);
}

/*
This function is called just before the component (pointed by comp)
is destroyed. When this function is called, the component and its
UI/objects are still available.

This function is good time to stop using objects referenced
within componentCreated().
*/
void D2MComponent::componentDestroyed(AbstractComponent* comp)
{
    Q_UNUSED(comp);
}

QObject* D2MComponent::containerObject()
{
    return this;
}

QString D2MComponent::productName() const
{
    return "D2M";
}

QString D2MComponent::organization() const
{
    return "LevelFour";
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
    return "Provides D2M";
}

QString D2MComponent::homepage() const
{
    return "LevelFour website";
}

QString D2MComponent::bugAddress() const
{
    return "LevelFour website";
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
    return "LevelFour {{AppName}} License";
}

QString D2MComponent::copyrightStatement() const
{
    return QString();
}

GCF_EXPORT_COMPONENT_PLUGIN(D2MComponent)

