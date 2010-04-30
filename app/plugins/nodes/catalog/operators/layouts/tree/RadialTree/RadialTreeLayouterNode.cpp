/*
-----------------------------------------------------------------------------
file "RadialTreeLayouterNode.cpp"
brief Implementation file for RadialTreeLayouterNode class.
*/

#include "RadialTreeLayouterNode.h"
#include "vtkTreeLayoutStrategy.h"
#include "VTKTableParameter.h"

INIT_INSTANCE_COUNTER(RadialTreeLayouterNode)
///
/// Constructors and Destructors
///

//!
//! Constructor of the RadialTreeLayouterNode class.
//!
//! \param name The name for the new node.
//! \param parameterRoot A copy of the parameter tree specific for the type of the node.
//!
RadialTreeLayouterNode::RadialTreeLayouterNode ( const QString &name, ParameterGroup *parameterRoot ) :
    VTKTreeLayoutNode(name, parameterRoot)
{
	setTypeName("RadialTreeLayouterNode");

	m_layoutInstance = vtkTreeLayoutStrategy::New();
	m_layoutInstance->SetRadial(true);

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
//! Destructor of the RadialTreeLayouterNode class.
//!
//! Defined virtual to guarantee that the destructor of a derived class
//! will be called if the instance of the derived class is saved in a
//! variable of its parent class type.
//!
RadialTreeLayouterNode::~RadialTreeLayouterNode ()
{
	emit destroyed();
    DEC_INSTANCE_COUNTER
    Log::info(QString("RadialTreeLayouterNode destroyed."), "RadialTreeLayouterNode::~RadialTreeLayouterNode");
}

//!
//! Set the single layout angular radius property
//!
void RadialTreeLayouterNode::setZRange ()
{
	m_layoutInstance->SetZRange(m_zRange);
}

void RadialTreeLayouterNode::setAngle ()
{
	m_layoutInstance->SetAngle(m_Angle); // 0 to 360
}

void RadialTreeLayouterNode::setLogSpacingValue ()
{
	m_layoutInstance->SetLogSpacingValue(m_logSpacingValue); // 0 to 1
}

void RadialTreeLayouterNode::setLeafSpacing ()
{
	m_layoutInstance->SetLeafSpacing(m_leafSpacing); // 0 to 1
}

void RadialTreeLayouterNode::setDistanceArrayName ()
{
	m_layoutInstance->SetDistanceArrayName(m_distanceArrayName);
}

void RadialTreeLayouterNode::setRotation ()
{
	m_layoutInstance->SetRotation(m_Rotation);
}
