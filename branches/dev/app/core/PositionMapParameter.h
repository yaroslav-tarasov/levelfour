//!
//! \file "PositionMapParameter.h"
//! \brief Header file for PositionMapParameter class.
//!
//! \version    1.0
//! \date       19.04.2010 (last updated)
//!

#ifndef PositionMapParameter_H
#define PositionMapParameter_H

#include "FrapperPrerequisites.h"
#include "AbstractMapParameter.h"
#include "OgreEntity.h"

//!
//! Class representing parameters for vtk table values.
//!
class FRAPPER_CORE_EXPORT PositionMapParameter : public AbstractMapParameter 
{
public: // constructors and destructors

    //!
    //! Constructor of the PositionMapParameter class.
    //!
    //! \param name The name of the parameter.
    //!
    PositionMapParameter ( const QString &name );

	//!
    //! Destructor of the PositionMapParameter class.
    //!
    virtual ~PositionMapParameter ();

public: // functions

    //!
    //! Returns the Position associate with the id.
    //!
    //! \return The item Position.
    //!
	Ogre::Vector3 getPosition (const vtkIdType id) const;

    //!
    //! Sets the Position associated to the id.
    //!
    //! \param id The id.
    //! \param position The Position.
    //!
	void setPosition ( const vtkIdType id, const Ogre::Vector3 position);
};


#endif
