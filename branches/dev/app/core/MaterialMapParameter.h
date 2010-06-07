//!
//! \file "MaterialMapParameter.h"
//! \brief Header file for MaterialMapParameter class.
//!
//! \version    1.0
//! \date       19.04.2010 (last updated)
//!

#ifndef MaterialMapParameter_H
#define MaterialMapParameter_H

#include "FrapperPrerequisites.h"
#include "AbstractMapParameter.h"
#include "OgreEntity.h"

//!
//! Class representing parameters for vtk table values.
//!
class FRAPPER_CORE_EXPORT MaterialMapParameter : public AbstractMapParameter 
{
public: // constructors and destructors

    //!
    //! Constructor of the MaterialMapParameter class.
    //!
    //! \param name The name of the parameter.
    //!
    MaterialMapParameter ( const QString &name );

	//!
    //! Destructor of the MaterialMapParameter class.
    //!
    virtual ~MaterialMapParameter ();

public: // functions

    //!
    //! Returns the Material associate with the id.
    //!
    //! \return The item Material.
    //!
	Ogre::Material * getMaterial (const vtkIdType id) const;

    //!
    //! Sets the Material associated to the id.
    //!
    //! \param id The id.
    //! \param material The Material.
    //!
	void setMaterial ( const vtkIdType id, Ogre::Material * material);
};


#endif
