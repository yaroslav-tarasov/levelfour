//!
//! \file "SimpleBlendNodePlugin.cpp"
//! \brief Implementation file for SimpleBlendNodePlugin class.
//!
//! \author     Nils Zweiling <nils.zweiling@filmakademie.de>
//! \author     Stefan Habel <stefan.habel@filmakademie.de>
//! \author     Felix Bucella <felix.bucella@filmakademie.de>
//! \version    1.0
//! \date       23.10.2009 (last updated)
//!

#include "SimpleBlendNodePlugin.h"
#include "SimpleBlendNode.h"
#include <QtCore/QtPlugin>


///
/// Public Functions
///


//!
//! Creates a node of this node type.
//!
//! \param name The name for the new node.
//! \param parameterRoot A copy of the parameter tree specific for the type of the node.
//! \return A pointer to the new node.
//!
Node * SimpleBlendNodePlugin::createNode ( const QString &name, ParameterGroup *parameterRoot )
{
    return new SimpleBlendNode(name, parameterRoot);
}


Q_EXPORT_PLUGIN2(sipleblendnodeplugin, SimpleBlendNodePlugin)
