//!
//! \file "GeometryBlurNodePlugin.h"
//! \brief Header file for GeometryBlurNodePlugin class.
//!
//! \author     Felix Bucella <felix.bucella@filmakademie.de>
//! \version    1.0
//! \date       23.10.2009 (last updated)
//!

#ifndef GEOMETRYBLURNODEPLUGIN_H
#define GEOMETRYBLURNODEPLUGIN_H

#include "NodeTypeInterface.h"


//!
//! Plugin class for creating GeometryBlurNode objects.
//!
class GeometryBlurNodePlugin : public QObject, public NodeTypeInterface
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