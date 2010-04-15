
#ifndef TextDelimiterSourceNODEPLUGIN_H
#define TextDelimiterSourceNODEPLUGIN_H

#include "TextDelimiterSourceNode.h"
#include "NodeTypeInterface.h"
#include "Node.h"

//!
//! Plugin class for creation of TextDelimiterSourceNode node.
//!
class TextDelimiterSourceNodePlugin : public QObject, public NodeTypeInterface
{
    Q_OBJECT
    Q_INTERFACES(NodeTypeInterface)

public: /// methods

    virtual Node * createNode ( const QString &name, ParameterGroup *parameterRoot );


};


#endif
