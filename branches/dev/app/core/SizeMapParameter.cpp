//!
//! \file "SizeMapParameter.cpp"
//! \brief Header file for SizeMapParameter class.
//!
//! \version    1.0
//! \date       19.04.2010 (last updated)
//!

#include "SizeMapParameter.h"
#include "vtkVariant.h"

#define COL_SIZE_X	2
#define COL_SIZE_Y	3
#define COL_SIZE_Z	4

//!
//! Constructor of the SizeMapParameter class.
//!
//! \param name The name of the parameter.
//!
SizeMapParameter::SizeMapParameter ( const QString &name ):
	AbstractMapParameter(name, Parameter::T_SizeMap)
{

}


//!
//! Destructor of the SizeMapParameter class.
//!
SizeMapParameter::~SizeMapParameter ()
{

}

//!
//! Returns the size associate with the id.
//!
//! \return The item size.
//!
Ogre::Vector3 SizeMapParameter::getSize (const vtkIdType id) const
{
	Ogre::Vector3 pos(m_table->GetValue(id, COL_SIZE_X).ToDouble(), 
					m_table->GetValue(id, COL_SIZE_Y).ToDouble(), 
					m_table->GetValue(id, COL_SIZE_Z).ToDouble());
	return pos;
}

//!
//! Sets the Size associated to the id.
//!
//! \param id The id.
//! \param size The size.
//!
void SizeMapParameter::setSize ( const vtkIdType id, const Ogre::Vector3 size)
{
	m_table->SetValue(id, COL_SIZE_X, size.x);
	m_table->SetValue(id, COL_SIZE_Y, size.y);
	m_table->SetValue(id, COL_SIZE_Z, size.z);
}

