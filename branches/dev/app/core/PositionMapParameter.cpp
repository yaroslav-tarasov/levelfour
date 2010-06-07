//!
//! \file "PositionMapParameter.cpp"
//! \brief Header file for PositionMapParameter class.
//!
//! \version    1.0
//! \date       19.04.2010 (last updated)
//!

#include "PositionMapParameter.h"
#include "vtkVariant.h"

#define COL_X 2
#define COL_Y 3
#define COL_Z 4


//!
//! Constructor of the PositionMapParameter class.
//!
//! \param name The name of the parameter.
//!
PositionMapParameter::PositionMapParameter ( const QString &name ) :
	AbstractMapParameter(name, Parameter::T_PositionMap)
{

}

//!
//! Destructor of the PositionMapParameter class.
//!
PositionMapParameter::~PositionMapParameter ()
{

}

//!
//! Returns the Position associate with the id.
//!
//! \return The item Position.
//!
Ogre::Vector3 PositionMapParameter::getPosition (const vtkIdType id) const
{
	Ogre::Vector3 pos(m_table->GetValue(id, COL_X).ToDouble(), 
					m_table->GetValue(id, COL_Y).ToDouble(), 
					m_table->GetValue(id, COL_Z).ToDouble());
	return pos;
}

//!
//! Sets the Position associated to the id.
//!
//! \param id The id.
//! \param position The Position.
//!
void PositionMapParameter::setPosition ( const vtkIdType id, const Ogre::Vector3 position)
{
	m_table->SetValue(id, COL_X, position.x);
	m_table->SetValue(id, COL_Y, position.y);
	m_table->SetValue(id, COL_Z, position.z);
}
