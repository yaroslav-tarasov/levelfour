/*
-----------------------------------------------------------------------------
file "ShapeMapperNode.cpp"
brief Implementation file for ShapeMapperNode class.

version    1.0
date       18.05.2009 (last updated)

Description
-----------


Parameters
----------
      
Returns
-------

    
Notes
-----

Reference
---------

-----------------------------------------------------------------------------
*/


#include "ShapeMapperNode.h"
#include "EntityParameter.h"
#include "ParameterGroup.h"

INIT_INSTANCE_COUNTER(ShapeMapperNode)

///
/// Constructors and Destructors
///

//!
//! Constructor of the ShapeMapperNode class.
//!
//! \param name The name for the new node.
//! \param parameterRoot A copy of the parameter tree specific for the type of the node.
//!
ShapeMapperNode::ShapeMapperNode ( const QString &name, ParameterGroup *parameterRoot ) :
    Node(name, parameterRoot),
	m_outputGeometryParameterName("GeometryOutput"),
	m_entity(0)
{
	// create the geometry output parameter (shape mapper)
	m_outputGeometryParameter = new EntityParameter(m_outputGeometryParameterName);
	m_outputGeometryParameter->setPinType(Parameter::PT_Output);
	parameterRoot->addParameter(m_outputGeometryParameter);

	INC_INSTANCE_COUNTER
}

//!
//! Destructor of the ShapeMapperNode class.
//!
//! Defined virtual to guarantee that the destructor of a derived class
//! will be called if the instance of the derived class is saved in a
//! variable of its parent class type.
//!
ShapeMapperNode::~ShapeMapperNode ()
{
	emit destroyed();
	Log::info(QString("ShapeMapperNode destroyed."), "ShapeMapperNode::~ShapeMapperNode");
	DEC_INSTANCE_COUNTER
}