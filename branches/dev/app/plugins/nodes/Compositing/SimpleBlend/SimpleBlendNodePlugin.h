//!
//! \file "SimpleBlendNodePlugin.h"
//! \brief Header file for SimpleBlendNodePlugin class.
//!
//! \author     Felix Bucella <felix.bucella@filmakademie.de>
//! \version    1.0
//! \date       23.10.2009 (last updated)
//!

#ifndef SIMPLEBLENDNODEPLUGIN_H
#define SIMPLEBLENDNODEPLUGIN_H

#include "NodeTypeInterface.h"


//!
//! Plugin class for creating SimpleBlendNode objects.
//!
class SimpleBlendNodePlugin : public QObject, public NodeTypeInterface
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