

#include "Model2SceneNodePlugin.h"
#include "Node.h"

#include <QtCore/QtPlugin>

//!
//! Creates a node of this node type.
//!
//! \param name The name for the new node.
//! \return A pointer to the new node.
//!
Node * Model2SceneNodePlugin::createNode ( const QString &name, ParameterGroup *parameterRoot )
{
	return new Model2SceneNode(name, parameterRoot);
}

Q_EXPORT_PLUGIN2(Model2Scenenodeplugin, Model2SceneNodePlugin)