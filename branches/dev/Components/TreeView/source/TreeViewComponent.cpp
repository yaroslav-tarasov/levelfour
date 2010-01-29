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

#include "TreeViewComponent.h"
#include <GCF/ComponentFactory>
#include <GCF/IComponentPlugin>

GCF_DEFINE_COMPONENT(TreeViewComponent)

struct TreeViewComponentData
{

};

TreeViewComponent & TreeViewComponent::instance()
{
    static TreeViewComponent* theInstance = GCF_CREATE_COMPONENT(TreeViewComponent);
    return *theInstance;
}

TreeViewComponent::TreeViewComponent()
{
    d = new TreeViewComponentData;
}

TreeViewComponent::~TreeViewComponent()
{
    delete d;
}

QObject* TreeViewComponent::fetchObject(const QString& completeName) const
{
    QStringList comps = completeName.split('.');

    TreeViewComponent* that = const_cast<TreeViewComponent*>(this);
    
    if(comps.last() == "controller")
        return that;

    return 0;
}

QWidget* TreeViewComponent::fetchWidget(const QString& completeName) const
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
void TreeViewComponent::initializeComponent()
{
    // Do nothing.
}

/*
This method is called just before the UI of the component is unloaded.
It is safe to use GCF::AbstractComponent::settings() here. Anything
you write into settings() in this function will be stored and loaded
back when the component class is used next
*/
void TreeViewComponent::finalizeComponent()
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
void TreeViewComponent::creationAnnounced()
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
void TreeViewComponent::destructionAnnounced()
{
    // Do nothing
}

/*
This function is called before the component is about to be activated.
GUIXML of the component is read and processed before this function call.
*/
void TreeViewComponent::aboutToActivate()
{
    // Do nothing
}

/*
This function is called just after the component has been activated.
*/
void TreeViewComponent::justActivated()
{
    // Do nothing
}

/*
This function is called just before the component is about to get 
deactivated.
*/
void TreeViewComponent::aboutToDeactivate()
{
    // Do nothing
}

/*
This function is called just after the component has been deactivated.
*/
void TreeViewComponent::justDeactivated()
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
void TreeViewComponent::componentCreated(AbstractComponent* comp)
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
void TreeViewComponent::componentDestroyed(AbstractComponent* comp)
{
    Q_UNUSED(comp);
}

QObject* TreeViewComponent::containerObject()
{
    return this;
}

QString TreeViewComponent::productName() const
{
    return "TreeView";
}

QString TreeViewComponent::organization() const
{
    return "LevelFour";
}

QImage TreeViewComponent::programLogo() const
{
    return QImage();
}

QString TreeViewComponent::version() const
{
    return "1.0";
}

QString TreeViewComponent::shortDescription() const
{
    return "Provides TreeView";
}

QString TreeViewComponent::homepage() const
{
    return "LevelFour website";
}

QString TreeViewComponent::bugAddress() const
{
    return "LevelFour website";
}

const QList<GCF::AboutPerson> TreeViewComponent::authors() const
{
    static QList<GCF::AboutPerson> retList;
    return retList;
}

const QList<GCF::AboutPerson> TreeViewComponent::credits() const
{
    static QList<GCF::AboutPerson> retList;
    return retList;
}

QString TreeViewComponent::license() const
{
    return "LevelFour {{AppName}} License";
}

QString TreeViewComponent::copyrightStatement() const
{
    return QString();
}

GCF_EXPORT_COMPONENT_PLUGIN(TreeViewComponent)
