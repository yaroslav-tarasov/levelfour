
#ifndef Model2SceneNODEPLUGIN_H
#define Model2SceneNODEPLUGIN_H

#include "Model2SceneNode.h"
#include "NodeTypeInterface.h"
#include "Node.h"

//!
//! Plugin class for creation of Model2SceneNode node.
//!
class Model2SceneNodePlugin : public QObject, public NodeTypeInterface
{
    Q_OBJECT
    Q_INTERFACES(NodeTypeInterface)

public: /// methods

    virtual Node * createNode ( const QString &name, ParameterGroup *parameterRoot );


};


#endif
