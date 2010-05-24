//!
//! \file "BleachBypassNodePlugin.h"
//! \brief Header file for BleachBypassNodePlugin class.
//!
//! \author     Nils Zweiling <nils.zweiling@filmakademie.de>
//! \author     Stefan Habel <stefan.habel@filmakademie.de>
//! \version    0.1
//! \date       11.02.2009 (last updated)
//!

#ifndef BleachBypassNODEPLUGIN_H
#define BleachBypassNODEPLUGIN_H

#include "NodeTypeInterface.h"


//!
//! Plugin class for creating BleachBypassNode objects.
//!
class BleachBypassNodePlugin : public QObject, public NodeTypeInterface
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