/*
-----------------------------------------------------------------------------
file "SpanTreeLayouterNode.cpp"
brief Implementation file for SpanTreeLayouterNode class.
*/

#include "SpanTreeLayouterNode.h"
#include "VTKTableParameter.h"

INIT_INSTANCE_COUNTER(SpanTreeLayouterNode)
//!
//! Constructor of the SpanTreeLayouterNode class.
//!
//! \param name The name for the new node.
//! \param parameterRoot A copy of the parameter tree specific for the type of the node.
//!
SpanTreeLayouterNode::SpanTreeLayouterNode ( const QString &name, ParameterGroup *parameterRoot ) :
	VTKTreeLayoutNode(name, parameterRoot)
{
	setTypeName("SpanTreeLayouterNode");

	m_layoutInstance = vtkSpanTreeLayoutStrategy::New();

	setChangeFunction("Set Depth First Spanning Tree", SLOT(setDepthFirstSpanningTree()));
    setCommandFunction("Set Depth First Spanning Tree", SLOT(setDepthFirstSpanningTree()));

	INC_INSTANCE_COUNTER
}


//!
//! Destructor of the SpanTreeLayouterNode class.
//!
//! Defined virtual to guarantee that the destructor of a derived class
//! will be called if the instance of the derived class is saved in a
//! variable of its parent class type.
//!
SpanTreeLayouterNode::~SpanTreeLayouterNode ()
{
	emit destroyed();
    DEC_INSTANCE_COUNTER
    Log::info(QString("SpanTreeLayouterNode destroyed."), "SpanTreeLayouterNode::~SpanTreeLayouterNode");
}

//!
//! Set the single layout angular radius property
//!
void SpanTreeLayouterNode::setDepthFirstSpanningTree ()
{
	M_LAYOUT->SetDepthFirstSpanningTree(m_depthFirst);
}
