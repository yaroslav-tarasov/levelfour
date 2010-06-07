//!
//! \file "AbstractMapParameter.cpp"
//! \brief Header file for AbstractMapParameter class.
//!
//! \version    1.0
//! \date       19.04.2010 (last updated)
//!

#include "AbstractMapParameter.h"

//!
//! Constructor of the AbstractMapParameter class.
//!
//! \param name The name of the parameter.
//! \param type The type of the parameter.
//!
AbstractMapParameter::AbstractMapParameter ( const QString &name, Parameter::Type type ) :
	VTKTableParameter(name, type)
{

}

//!
//! Destructor of the AbstractMapParameter class.
//!
AbstractMapParameter::~AbstractMapParameter ()
{

}


//!
//! Returns the name of the Abstract item.
//!
//! \return The name of the Abstract item.
//!
char * AbstractMapParameter::getName (const vtkIdType id) const
{
	return 0;
}

//!
//! Sets the name of the Abstract item.
//!
//! \param id The id.
//! \param name The name of the Abstract item.
//!
void AbstractMapParameter::setName (const vtkIdType id, vtkIdType name)
{

}