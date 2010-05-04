/*
-----------------------------------------------------------------------------
file "TreeOrbitLayouterNode.cpp"
brief Implementation file for TreeOrbitLayouterNode class.
*/

#include "TreeOrbitLayouterNode.h"
#include "VTKTableParameter.h"

INIT_INSTANCE_COUNTER(TreeOrbitLayouterNode)
//!
//! Constructor of the TreeOrbitLayouterNode class.
//!
//! \param name The name for the new node.
//! \param parameterRoot A copy of the parameter tree specific for the type of the node.
//!
TreeOrbitLayouterNode::TreeOrbitLayouterNode ( const QString &name, ParameterGroup *parameterRoot ) :
    VTKTreeLayoutNode(name, parameterRoot)
{
	setTypeName("TreeOrbitLayouterNode");

	m_layoutInstance = vtkTreeOrbitLayoutStrategy::New();

	setChangeFunction("Set Log Spacing Value", SLOT(setLogSpacingValue()));
    setCommandFunction("Set Log Spacing Value", SLOT(setLogSpacingValue()));

	setChangeFunction("Set Leaf Spacing", SLOT(setLeafSpacing()));
    setCommandFunction("Set Leaf Spacing", SLOT(setLeafSpacing()));

	setChangeFunction("Set Child Radius Factor", SLOT(setChildRadiusFactor()));
    setCommandFunction("Set Child Radius Factor", SLOT(setChildRadiusFactor()));

	INC_INSTANCE_COUNTER
}


//!
//! Destructor of the TreeOrbitLayouterNode class.
//!
//! Defined virtual to guarantee that the destructor of a derived class
//! will be called if the instance of the derived class is saved in a
//! variable of its parent class type.
//!
TreeOrbitLayouterNode::~TreeOrbitLayouterNode ()
{
	emit destroyed();
    DEC_INSTANCE_COUNTER
    Log::info(QString("TreeOrbitLayouterNode destroyed."), "TreeOrbitLayouterNode::~TreeOrbitLayouterNode");
}

//!
//! Set the single layout angular radius property
//!
void TreeOrbitLayouterNode::setLogSpacingValue ()
{
	M_LAYOUT->SetLogSpacingValue(m_logSpacingValue);
}

void TreeOrbitLayouterNode::setLeafSpacing ()
{
	M_LAYOUT->SetLeafSpacing(m_leafSpacing);
}

void TreeOrbitLayouterNode::setChildRadiusFactor ()
{
	M_LAYOUT->SetChildRadiusFactor(m_childRadiusFactor);
}

