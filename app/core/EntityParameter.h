//!
//! \file "EntityParameter.h"
//! \brief Header file for EntityParameter class.
//!
//! \author		Simon Spielmann <sspielma@filmakademie.de>
//! \version    1.0
//! \date       16.04.2010 (last updated)
//!

#ifndef EntityParameter_H
#define EntityParameter_H

#include "FrapperPrerequisites.h"
#include "Parameter.h"


//!
//! Class representing parameters for numeric values.
//!
class FRAPPER_CORE_EXPORT EntityParameter : public Parameter
{

public: // constructors and destructors

	//!
	//! Constructor of the EntityParameter class.
	//!
	//! \param name The name of the parameter.
	//! \param type The type of the parameter's value(s).
	//! \param value The parameter's value.
	//! \param animated The parameter's animation status.
	//!
	EntityParameter ( const QString &name);

	//!
	//! Destructor of the FilenameParameter class.
	//!
	virtual ~EntityParameter ();

public: // functions

	//!
	//! Convenience function for setting the entity of an unsigned integer
	//!
	//! \param entity The entity to set for the entity parameter with the given name.
	//!
	void setEntity ( Ogre::Entity * entity );

	//!
	//! Returns the parameter's value.
	//!
	//! \return The parameter's value.
	//!
	Ogre::Entity * getEntity ( ) const;

private: // data

	Ogre::Entity * m_entity;
};

#endif
