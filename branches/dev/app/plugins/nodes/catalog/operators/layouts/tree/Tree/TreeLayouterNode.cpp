/*
-----------------------------------------------------------------------------
file "TreeLayouterNode.cpp"
brief Implementation file for TreeLayouterNode class.

version    1.0
date       21.04.2010 (last updated)

This is a base class for layouts which are based on
tree structures (vtkTree).

*/

#include "TreeLayouterNode.h"
#include "vtkVariant.h"

INIT_INSTANCE_COUNTER(TreeLayouterNode)

//!
//! Constructor of the TreeLayouterNode class.
//!
//! \param name The name for the new node.
//! \param parameterRoot A copy of the parameter tree specific for the type of the node.
//!
TreeLayouterNode::TreeLayouterNode ( const QString &name, ParameterGroup *parameterRoot ) :
	VTKTreeLayoutNode(name, parameterRoot),
	m_radial(true),
	m_angle(360),
	m_radialParameterName("Radial"),
	m_angleParameterName("Angle")
{
	setTypeName("TreeLayouterNode");

	m_layoutInstance = vtkTreeLayoutStrategy::New();

    // create the mandatory radial parameter 
	Parameter * radialParameter = new Parameter(m_radialParameterName, Parameter::T_Bool, QVariant::fromValue<bool>(true));
	radialParameter->setDefaultValue(QVariant::fromValue<bool>(true));
    parameterRoot->addParameter(radialParameter);
	connect(radialParameter, SIGNAL(dirtied()), SLOT(processParameters()));

    // create the mandatory radial parameter 
	NumberParameter * angleParameter = new NumberParameter(m_angleParameterName, Parameter::T_Float, QVariant::fromValue<double>(360));
	angleParameter->setMinValue(QVariant::fromValue<double>(0));
	angleParameter->setMaxValue(QVariant::fromValue<double>(360));
    parameterRoot->addParameter(angleParameter);
	connect(angleParameter, SIGNAL(dirtied()), SLOT(processParameters()));

	INC_INSTANCE_COUNTER
}


//!
//! Destructor of the TreeLayouterNode class.
//!
//! Defined virtual to guarantee that the destructor of a derived class
//! will be called if the instance of the derived class is saved in a
//! variable of its parent class type.
//!
TreeLayouterNode::~TreeLayouterNode ()
{
	emit destroyed();
    DEC_INSTANCE_COUNTER
    Log::info(QString("TreeLayouterNode destroyed."), "TreeLayouterNode::~TreeLayouterNode");
}

void TreeLayouterNode::processParameters()
{
	if (!updateInputTree())
		return;
	
	setRadial();
	setAngle();

	refreshOutput();
}

//!
//! Apply the radial parameter
//!
void TreeLayouterNode::setRadial()
{
	bool radial = getBoolValue(m_radialParameterName);
	if (radial == m_radial)
		return;

	m_radial = radial;

	M_LAYOUT->SetRadial(m_radial);
}

//!
//! Apply the angle parameter
//!
void TreeLayouterNode::setAngle()
{
	double angle = getDoubleValue(m_angleParameterName);
	if (m_angle == angle)
		return;

	m_angle = angle;

	M_LAYOUT->SetAngle(m_angle);
}