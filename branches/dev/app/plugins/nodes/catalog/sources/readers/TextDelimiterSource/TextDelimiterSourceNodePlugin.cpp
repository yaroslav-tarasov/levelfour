

#include "TextDelimiterSourceNodePlugin.h"
#include "Node.h"

#include <QtCore/QtPlugin>

//!
//! Creates a node of this node type.
//!
//! \param name The name for the new node.
//! \return A pointer to the new node.
//!
Node * TextDelimiterSourceNodePlugin::createNode ( const QString &name, ParameterGroup *parameterRoot )
{
	return new TextDelimiterSourceNode(name, parameterRoot);
}

Q_EXPORT_PLUGIN2(TextDelimiterSourcenodeplugin, TextDelimiterSourceNodePlugin)