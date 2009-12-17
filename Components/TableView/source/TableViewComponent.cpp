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

#include "TableViewComponent.h"
#include <GCF/ComponentFactory>
#include <GCF/IComponentPlugin>

GCF_DEFINE_COMPONENT(TableViewComponent)

struct TableViewComponentData
{

};

TableViewComponent & TableViewComponent::instance()
{
    static TableViewComponent* theInstance = GCF_CREATE_COMPONENT(TableViewComponent);
    return *theInstance;
}

TableViewComponent::TableViewComponent()
{
    d = new TableViewComponentData;
}

TableViewComponent::~TableViewComponent()
{
    delete d;
}

QObject* TableViewComponent::fetchObject(const QString& completeName) const
{
    QStringList comps = completeName.split('.');

    TableViewComponent* that = const_cast<TableViewComponent*>(this);
    
    if(comps.last() == "controller")
        return that;

    return 0;
}

QWidget* TableViewComponent::fetchWidget(const QString& completeName) const
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
void TableViewComponent::initializeComponent()
{
    // Do nothing.
}

/*
This method is called just before the UI of the component is unloaded.
It is safe to use GCF::AbstractComponent::settings() here. Anything
you write into settings() in this function will be stored and loaded
back when the component class is used next
*/
void TableViewComponent::finalizeComponent()
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
void TableViewComponent::creationAnnounced()
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
void TableViewComponent::destructionAnnounced()
{
    // Do nothing
}

/*
This function is called before the component is about to be activated.
GUIXML of the component is read and processed before this function call.
*/
void TableViewComponent::aboutToActivate()
{
    // Do nothing
}

/*
This function is called just after the component has been activated.
*/
void TableViewComponent::justActivated()
{
    // Do nothing
}

/*
This function is called just before the component is about to get 
deactivated.
*/
void TableViewComponent::aboutToDeactivate()
{
    // Do nothing
}

/*
This function is called just after the component has been deactivated.
*/
void TableViewComponent::justDeactivated()
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
void TableViewComponent::componentCreated(AbstractComponent* comp)
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
void TableViewComponent::componentDestroyed(AbstractComponent* comp)
{
    Q_UNUSED(comp);
}

QObject* TableViewComponent::containerObject()
{
    return this;
}

QString TableViewComponent::productName() const
{
    return "TableView";
}

QString TableViewComponent::organization() const
{
    return "LevelFour";
}

QImage TableViewComponent::programLogo() const
{
    return QImage();
}

QString TableViewComponent::version() const
{
    return "1.0";
}

QString TableViewComponent::shortDescription() const
{
    return "Provides TableView";
}

QString TableViewComponent::homepage() const
{
    return "LevelFour website";
}

QString TableViewComponent::bugAddress() const
{
    return "LevelFour website";
}

const QList<GCF::AboutPerson> TableViewComponent::authors() const
{
    static QList<GCF::AboutPerson> retList;
    return retList;
}

const QList<GCF::AboutPerson> TableViewComponent::credits() const
{
    static QList<GCF::AboutPerson> retList;
    return retList;
}

QString TableViewComponent::license() const
{
    return "LevelFour {{AppName}} License";
}

QString TableViewComponent::copyrightStatement() const
{
    return QString();
}

GCF_EXPORT_COMPONENT_PLUGIN(TableViewComponent)

