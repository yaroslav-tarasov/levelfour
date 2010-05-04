//!
//! \file "CosmicTreeLayouterNode.h"
//! \brief Header file for CosmicTreeLayouterNode class.
//!
//! \version    1.0
//! \date       18.05.2009 (last updated)
//!

#ifndef CosmicTreeLayouterNODE_H
#define CosmicTreeLayouterNODE_H

#include "VTKTreeLayoutNode.h"
#include "vtkCosmicTreeLayoutStrategy.h"

//!
//! Class for a CosmicTreeLayouter
//!
class CosmicTreeLayouterNode : public VTKTreeLayoutNode
{

    Q_OBJECT
    ADD_INSTANCE_COUNTER
	LAYOUT_TYPE(vtkCosmicTreeLayoutStrategy)

public: // constructors and destructors

    //!
    //! Constructor of the CosmicTreeLayouterNode class.
    //!
    //! \param name The name for the new node.
    //! \param parameterRoot A copy of the parameter tree specific for the type of the node.
    //!
    CosmicTreeLayouterNode ( const QString &name, ParameterGroup *parameterRoot );

    //!
    //! Destructor of the CosmicTreeLayouterNode class.
    //!
    //! Defined virtual to guarantee that the destructor of a derived class
    //! will be called if the instance of the derived class is saved in a
    //! variable of its parent class type.
    //!
    virtual ~CosmicTreeLayouterNode ();

private slots: //

    void setSizeLeafNodesOnly ();
    void setLayoutDepth ();
	void setLayoutRoot ();
	void setNodeSizeArrayName ();

private: // data

	int m_sizeLeafNodesOnly;
	int m_layoutDepth;
	int m_layoutRoot;
	QString m_nodeSizeArrayName;
};


#endif
