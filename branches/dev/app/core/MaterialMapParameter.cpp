//!
//! \file "MaterialMapParameter.cpp"
//! \brief Header file for MaterialMapParameter class.
//!
//! \version    1.0
//! \date       19.04.2010 (last updated)
//!

#include "MaterialMapParameter.h"

#define COL_ID		0
#define COL_NAME	1
#define COL_MAT		2

//!
//! Constructor of the MaterialMapParameter class.
//!
//! \param name The name of the parameter.
//!
MaterialMapParameter::MaterialMapParameter ( const QString &name ) :
	AbstractMapParameter(name, Parameter::T_MaterialMap)
{

}

//!
//! Destructor of the MaterialMapParameter class.
//!
MaterialMapParameter::~MaterialMapParameter ()
{
}
//!
//! Returns the Material associate with the id.
//!
//! \return The item Material.
//!
Ogre::Material * MaterialMapParameter::getMaterial (const vtkIdType id) const
{
	return 0;
}

//!
//! Sets the Material associated to the id.
//!
//! \param id The id.
//! \param material The Material.
//!
void MaterialMapParameter::setMaterial ( const vtkIdType id, Ogre::Material * material)
{

}
