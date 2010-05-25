/*
-----------------------------------------------------------------------------

-----------------------------------------------------------------------------
*/

//!
//! \file "PosterizeNodePlugin.h"
//! \brief Header file for PosterizeNodePlugin class.
//!
//! \author     Nils Zweiling <nils.zweiling@filmakademie.de>
//! \author     Stefan Habel <stefan.habel@filmakademie.de>
//! \version    0.1
//! \date       11.02.2009 (last updated)
//!

#ifndef PosterizeNODEPLUGIN_H
#define PosterizeNODEPLUGIN_H

#include "NodeTypeInterface.h"


//!
//! Plugin class for creating PosterizeNode objects.
//!
class PosterizeNodePlugin : public QObject, public NodeTypeInterface
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