//!
//! \file "ForceDirectedLayouterNode.cpp"
//! \brief Implementation file for ForceDirectedLayouterNode class.
//!

#include "ForceDirectedLayouterNode.h"
#include "vtkForceDirectedLayoutStrategy.h"

INIT_INSTANCE_COUNTER(ForceDirectedLayouterNode)

//!
//! Constructor of the ForceDirectedLayouterNode class.
//!
//! \param name The name for the new node.
//! \param parameterRoot A copy of the parameter tree specific for the type of the node.
//!
ForceDirectedLayouterNode::ForceDirectedLayouterNode ( const QString &name, ParameterGroup *parameterRoot ) :
    VTKGraphLayoutNode(name, parameterRoot)
{
	setTypeName("ForceDirectedLayouterNode");

    // create the vtk graph layout instance
	m_layoutInstance = vtkForceDirectedLayoutStrategy::New();

    INC_INSTANCE_COUNTER
}


//!
//! Destructor of the ForceDirectedLayouterNode class.
//!
//! Defined virtual to guarantee that the destructor of a derived class
//! will be called if the instance of the derived class is saved in a
//! variable of its parent class type.
//!
ForceDirectedLayouterNode::~ForceDirectedLayouterNode ()
{
	emit destroyed();
    DEC_INSTANCE_COUNTER
    Log::info(QString("ForceDirectedLayouterNode destroyed."), "ForceDirectedLayouterNode::~ForceDirectedLayouterNode");
}

void ForceDirectedLayouterNode::processParameters()
{
	if (!updateInputGraph())
		return;
	
	refreshOutput();
}