/*
-----------------------------------------------------------------------------
file "HierarchicalTreeLayouterNode.cpp"
brief Implementation file for HierarchicalTreeLayouterNode class.
*/

#include "HierarchicalTreeLayouterNode.h"
#include "VTKTableParameter.h"

INIT_INSTANCE_COUNTER(HierarchicalTreeLayouterNode)
///
/// Constructors and Destructors
///

//!
//! Constructor of the HierarchicalTreeLayouterNode class.
//!
//! \param name The name for the new node.
//! \param parameterRoot A copy of the parameter tree specific for the type of the node.
//!
HierarchicalTreeLayouterNode::HierarchicalTreeLayouterNode ( const QString &name, ParameterGroup *parameterRoot ) :
	VTKTreeLayoutNode(name, parameterRoot)
{
	setTypeName("HierarchicalTreeLayouterNode");

	m_layoutInstance = vtkTreeLayoutStrategy::New();
	M_LAYOUT->SetRadial(false);

	// set affections and callback functions
	setChangeFunction("Set Z Range", SLOT(setZRange()));
    setCommandFunction("Set Z Range", SLOT(setZRange()));

	setChangeFunction("Set Angle", SLOT(setAngle()));
    setCommandFunction("Set Angle", SLOT(setAngle()));

	setChangeFunction("Set Log Spacing Value", SLOT(setLogSpacingValue()));
    setCommandFunction("Set Log Spacing Value", SLOT(setLogSpacingValue()));

	setChangeFunction("Set Leaf Spacing", SLOT(setLeafSpacing()));
    setCommandFunction("Set Leaf Spacing", SLOT(setLeafSpacing()));

	setChangeFunction("Set Distance Array Name", SLOT(setDistanceArrayName()));
    setCommandFunction("Set Distance Array Name", SLOT(setDistanceArrayName()));

	setChangeFunction("Set Rotation", SLOT(setRotation()));
    setCommandFunction("Set Rotation", SLOT(setRotation()));

	INC_INSTANCE_COUNTER
}


//!
//! Destructor of the HierarchicalTreeLayouterNode class.
//!
//! Defined virtual to guarantee that the destructor of a derived class
//! will be called if the instance of the derived class is saved in a
//! variable of its parent class type.
//!
HierarchicalTreeLayouterNode::~HierarchicalTreeLayouterNode ()
{
	emit destroyed();
    DEC_INSTANCE_COUNTER
    Log::info(QString("HierarchicalTreeLayouterNode destroyed."), "HierarchicalTreeLayouterNode::~HierarchicalTreeLayouterNode");
}

//!
//! Set the single layout angular radius property
//!
void HierarchicalTreeLayouterNode::setZRange ()
{
//	M_LAYOUT->SetZRange(m_zRange);
}

void HierarchicalTreeLayouterNode::setAngle ()
{
	M_LAYOUT->SetAngle(m_Angle); // 0 to 180
}

void HierarchicalTreeLayouterNode::setLogSpacingValue ()
{
	M_LAYOUT->SetLogSpacingValue(m_logSpacingValue); // 0 to 1
}

void HierarchicalTreeLayouterNode::setLeafSpacing ()
{
	M_LAYOUT->SetLeafSpacing(m_leafSpacing); // 0 to 1
}

void HierarchicalTreeLayouterNode::setDistanceArrayName ()
{
	M_LAYOUT->SetDistanceArrayName(m_distanceArrayName.toLatin1());
}

void HierarchicalTreeLayouterNode::setRotation ()
{
//	M_LAYOUT->SetRotation(m_Rotation);
}
