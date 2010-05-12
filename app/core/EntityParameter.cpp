//!
//! \file "EntityParameter.cpp"
//! \brief Implementation file for EntityParameter class.
//!
//! \author     Simon Spielmann <sspielma@filmakademie.de>
//! \version    1.0
//! \date       16.04.2010 (last updated)
//!

#include "EntityParameter.h"
#include "Node.h"
#include <QVariant>
#include "OgreEntity.h"

Q_DECLARE_METATYPE(Ogre::Entity *)

///
/// Constructors and Destructors
///


//!
//! Constructor of the FilenameParameter class.
//!
//! \param name The name of the parameter.
//! \param type The type of the parameter's value(s).
//! \param value The parameter's value.
//! \param animated The parameter's animation status.
//!
EntityParameter::EntityParameter ( const QString &name ) :
	Parameter(name, Type::T_Generic, QVariant::fromValue<Ogre::Entity*>(0)),
	m_entity(0)
{
}

//!
//! Destructor of the FilenameParameter class.
//!
EntityParameter::~EntityParameter ()
{
}


///
/// Public Functions
///

Ogre::Entity * EntityParameter::getEntity() const
{
    return m_entity;
}

void EntityParameter::setEntity( Ogre::Entity * entity)
{
	if (entity != m_entity)
		m_entity = entity;
}