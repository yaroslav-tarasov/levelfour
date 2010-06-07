//!
//! \file "ShapeMapParameter.cpp"
//! \brief Header file for ShapeMapParameter class.
//!
//! \version    1.0
//! \date       19.04.2010 (last updated)
//!

#include "ShapeMapParameter.h"
#include "vtkVariant.h"

#define COL_SHAPE	2
#define COL_ALIGN	3

//!
//! Constructor of the ShapeMapParameter class.
//!
//! \param name The name of the parameter.
//!
ShapeMapParameter::ShapeMapParameter (const QString &name) :
	AbstractMapParameter(name, Parameter::T_ShapeMap),
		m_type(ShapeType::Un_known)
{
}

//!
//! Destructor of the ShapeMapParameter class.
//!
ShapeMapParameter::~ShapeMapParameter ()
{}


//!
//! Returns the entity associate with the id.
//!
//! \return The ogre entity.
//!
Ogre::Entity * ShapeMapParameter::getShape (const vtkIdType id) const
{
	// retreive the entities from the vtkUnsignedIntArray pointers
	Ogre::Entity * entity = reinterpret_cast<Ogre::Entity*>(m_table->GetValueByName(id, "entity_pointers").ToUnsignedInt());
	return entity;
}

//!
//! Sets the shape associated to the id.
//!
//! \param id The id.
//! \param entity The ogre entity representing the shape.
//!
void ShapeMapParameter::setShape ( const vtkIdType id, Ogre::Entity * entity)
{

}

//!
//! Returns the entity associate with the id.
//!
//! \return The ogre entity.
//!
QString ShapeMapParameter::getShapeName (const vtkIdType id) const
{
	if (m_table)
		return QString(m_table->GetValueByName(id, "mesh_names").ToString().c_str());
	return 0;
}

//!
//! Sets the shape name associated to the id.
//!
//! \param id The id.
//! \param name The ogre entity name representing the shape.
//!
void ShapeMapParameter::setShapeName ( const vtkIdType id, QString name)
{

}

//!
//! Returns the alignement associate with the id.
//!
//! \return The alignement string.
//!
ShapeMapParameter::Alignement ShapeMapParameter::getAlignement(const vtkIdType id) const
{
	return Alignement::BottomCenter;
}

//!
//! Sets the alignement associate with the id.
//!
//! \param id The id.
//! \param entity The alignement.
//!
void ShapeMapParameter::setAlignement (const vtkIdType id, Alignement alignement)
{

}


//!
//! Returns the type of shapes associated with this mapper.
//!
//! \return The shape type.
//!
ShapeMapParameter::ShapeType  ShapeMapParameter::getShapeType() const
{
	return m_type;
}

//!
//! Sets the type of shapes associated with this mapper.
//! \param type The shape type.
//!
void ShapeMapParameter::setShapeType (const ShapeType  type)
{
	m_type = type;
}
