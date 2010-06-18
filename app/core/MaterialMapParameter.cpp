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
	AbstractMapParameter(name, Parameter::T_MaterialMap),
	m_materialForAll("")
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
Ogre::String MaterialMapParameter::getMaterial (const vtkIdType id) const
{
	if (m_materialForAll != "")
		return m_materialForAll;
	return "Default";
}

//!
//! Sets the Material associated to the id.
//!
//! \param id The id.
//! \param material The Material.
//!
void MaterialMapParameter::setMaterial (const vtkIdType id, Ogre::String material)
{

}

//!
//! Returns the unique (if any) Material for all shapes.
//!
//! \return The item Material.
//!
Ogre::String MaterialMapParameter::getMaterialForAll() const
{
	return m_materialForAll;
}

//!
//! Sets unique Material for all shapes.
//!
//! \param material The Material.
//!
void MaterialMapParameter::setMaterialForAll(const Ogre::String material)
{
	m_materialForAll = material;
}
