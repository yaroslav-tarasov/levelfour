//!
//! \file "AqtRenderNodePlugin.cpp"
//! \brief Implementation file for AqtRenderNodePlugin class.
//!
//! \author     Nils Zweiling <nils.zweiling@filmakademie.de>
//! \author     Stefan Habel <stefan.habel@filmakademie.de>
//! \version    0.1
//! \date       11.02.2009 (last updated)
//!

#include "SurfaceCurvatureNodePlugin.h"
#include "SurfaceCurvatureNode.h"
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
Node * SurfaceCurvatureNodePlugin::createNode ( const QString &name, ParameterGroup *parameterRoot )
{
    return new SurfaceCurvatureNode(name, parameterRoot);
}


Q_EXPORT_PLUGIN2(surfacecurvaturenodeplugin, SurfaceCurvatureNodePlugin)
