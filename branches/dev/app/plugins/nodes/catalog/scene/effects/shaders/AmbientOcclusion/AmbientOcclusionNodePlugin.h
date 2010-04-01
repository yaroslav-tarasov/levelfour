//!
//! \file "AmbientOcclusionNodePlugin.h"
//! \brief Header file for AmbientOcclusionNodePlugin class.
//!
//! \author     Nils Zweiling <nils.zweiling@filmakademie.de>
//! \author     Stefan Habel <stefan.habel@filmakademie.de>
//! \author     Felix Bucella <felix.bucella@filmakademie.de>
//! \version    1.0
//! \date       23.10.2009 (last updated)
//!

#ifndef AMBIENTOCCLUSIONNODEPLUGIN_H
#define AMBIENTOCCLUSIONNODEPLUGIN_H

#include "NodeTypeInterface.h"


//!
//! Plugin class for creating AmbientOcclusionNode objects.
//!
class AmbientOcclusionNodePlugin : public QObject, public NodeTypeInterface
{

    Q_OBJECT
    Q_INTERFACES(NodeTypeInterface)

public: // functions

    //!
    //! Creates a node of this node type.
    //!
    //! \param name The name for the new node.
    //! \param parameterRoot A copy of the parameter tree specific for the type of the node.
    //! \return A pointer to the new node.
    //!
    virtual Node * createNode ( const QString &name, ParameterGroup *parameterRoot );

};


#endif