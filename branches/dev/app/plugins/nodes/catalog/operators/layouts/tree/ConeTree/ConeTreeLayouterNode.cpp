/*
-----------------------------------------------------------------------------
file "ConeTreeLayouterNode.cpp"
brief Implementation file for ConeTreeLayouterNode class.
*/

#include "ConeTreeLayouterNode.h"
#include "vtkConeLayoutStrategy.h"
#include "VTKTableParameter.h"

INIT_INSTANCE_COUNTER(ConeTreeLayouterNode)

//!
//! Constructor of the ConeTreeLayouterNode class.
//!
//! \param name The name for the new node.
//! \param parameterRoot A copy of the parameter tree specific for the type of the node.
//!
ConeTreeLayouterNode::ConeTreeLayouterNode ( const QString &name, ParameterGroup *parameterRoot ) :
    VTKTreeLayoutNode(name, parameterRoot)
{
	setTypeName("ConeTreeLayouterNode");

	m_layoutInstance = vtkConeLayoutStrategy::New();

	setChangeFunction("Set Compactness", SLOT(setCompactness()));
    setCommandFunction("Set Compactness", SLOT(setCompactness()));

	setChangeFunction("Set Compression", SLOT(setCompression()));
    setCommandFunction("Set Compression", SLOT(setCompression()));

	setChangeFunction("Set Spacing", SLOT(setSpacing()));
    setCommandFunction("Set Spacing", SLOT(setSpacing()));

	INC_INSTANCE_COUNTER
}


//!
//! Destructor of the ConeTreeLayouterNode class.
//!
//! Defined virtual to guarantee that the destructor of a derived class
//! will be called if the instance of the derived class is saved in a
//! variable of its parent class type.
//!
ConeTreeLayouterNode::~ConeTreeLayouterNode ()
{
	emit destroyed();
    DEC_INSTANCE_COUNTER
    Log::info(QString("ConeTreeLayouterNode destroyed."), "ConeTreeLayouterNode::~ConeTreeLayouterNode");
}

//!
//! Set the various parameters from property editor
//!
void ConeTreeLayouterNode::setCompactness ()
{
	m_layoutInstance->SetCompactness(m_Compactness);
}

void ConeTreeLayouterNode::setCompression ()
{
	m_layoutInstance->SetCompression(m_Compression);
}

void ConeTreeLayouterNode::setSpacing ()
{
	m_layoutInstance->SetSpacing(m_Spacing);
}

