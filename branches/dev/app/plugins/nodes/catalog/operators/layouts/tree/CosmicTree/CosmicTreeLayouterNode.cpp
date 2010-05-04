/*
-----------------------------------------------------------------------------
file "CosmicTreeLayouterNode.cpp"
brief Implementation file for CosmicTreeLayouterNode class.
*/

#include "CosmicTreeLayouterNode.h"
#include "vtkCosmicTreeLayoutStrategy.h"
#include "VTKTableParameter.h"

INIT_INSTANCE_COUNTER(CosmicTreeLayouterNode)
///
/// Constructors and Destructors
///

//!
//! Constructor of the CosmicTreeLayouterNode class.
//!
//! \param name The name for the new node.
//! \param parameterRoot A copy of the parameter tree specific for the type of the node.
//!
CosmicTreeLayouterNode::CosmicTreeLayouterNode ( const QString &name, ParameterGroup *parameterRoot ) :
    VTKTreeLayoutNode(name, parameterRoot)
{
	setTypeName("CosmicTreeLayouterNode");

	m_layoutInstance = vtkCosmicTreeLayoutStrategy::New();

	setChangeFunction("Set Size Leaf Nodes Only", SLOT(setSizeLeafNodesOnly()));
    setCommandFunction("Set Size Leaf Nodes Only", SLOT(setSizeLeafNodesOnly()));

	setChangeFunction("Set Layout Depth", SLOT(setLayoutDepth()));
    setCommandFunction("Set Layout Depth", SLOT(setLayoutDepth()));

	setChangeFunction("Set Layout Root", SLOT(setLayoutRoot()));
    setCommandFunction("Set Layout Root", SLOT(setLayoutRoot()));

	setChangeFunction("Set Node Size Array Name", SLOT(setNodeSizeArrayName()));
    setCommandFunction("Set Node Size Array Name", SLOT(setNodeSizeArrayName()));

	INC_INSTANCE_COUNTER
}


//!
//! Destructor of the CosmicTreeLayouterNode class.
//!
//! Defined virtual to guarantee that the destructor of a derived class
//! will be called if the instance of the derived class is saved in a
//! variable of its parent class type.
//!
CosmicTreeLayouterNode::~CosmicTreeLayouterNode ()
{
	emit destroyed();
    DEC_INSTANCE_COUNTER
    Log::info(QString("CosmicTreeLayouterNode destroyed."), "CosmicTreeLayouterNode::~CosmicTreeLayouterNode");
}

void CosmicTreeLayouterNode::setSizeLeafNodesOnly ()
{
	M_LAYOUT->SetSizeLeafNodesOnly(m_sizeLeafNodesOnly);
}

void CosmicTreeLayouterNode::setLayoutDepth ()
{
	M_LAYOUT->SetLayoutDepth(m_layoutDepth);
}

void CosmicTreeLayouterNode::setLayoutRoot ()
{
	M_LAYOUT->SetLayoutRoot(m_layoutRoot);
}

void CosmicTreeLayouterNode::setNodeSizeArrayName ()
{
	M_LAYOUT->SetNodeSizeArrayName(m_nodeSizeArrayName.toLatin1());
}

