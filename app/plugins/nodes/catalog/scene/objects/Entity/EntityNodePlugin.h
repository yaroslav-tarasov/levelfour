
#ifndef EntityNODEPLUGIN_H
#define EntityNODEPLUGIN_H

#include "EntityNode.h"
#include "NodeTypeInterface.h"
#include "Node.h"

//!
//! Plugin class for creation of EntityNode node.
//!
class EntityNodePlugin : public QObject, public NodeTypeInterface
{
    Q_OBJECT
    Q_INTERFACES(NodeTypeInterface)

public: /// methods

    virtual Node * createNode ( const QString &name, ParameterGroup *parameterRoot );


};


#endif
