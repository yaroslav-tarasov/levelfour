//!
//! \file "ForceDirectedLayouterNode.h"
//! \brief Header file for ForceDirectedLayouterNode class.
//!

#ifndef ForceDirectedLayouterNode_H
#define ForceDirectedLayouterNode_H

#include "VTKGraphLayoutNode.h"

class ForceDirectedLayouterNode : public VTKGraphLayoutNode
{

    Q_OBJECT
    ADD_INSTANCE_COUNTER

public: // constructors and destructors

    //!
    //! Constructor of the ForceDirectedLayouterNode class.
    //!
    //! \param name The name for the new node.
    //! \param parameterRoot A copy of the parameter tree specific for the type of the node.
    //!
    ForceDirectedLayouterNode ( const QString &name, ParameterGroup *parameterRoot );

    //!
    //! Destructor of the ForceDirectedLayouterNode class.
    //!
    //! Defined virtual to guarantee that the destructor of a derived class
    //! will be called if the instance of the derived class is saved in a
    //! variable of its parent class type.
    //!
    virtual ~ForceDirectedLayouterNode ();

protected slots: //
	virtual void processParameters();
};


#endif
