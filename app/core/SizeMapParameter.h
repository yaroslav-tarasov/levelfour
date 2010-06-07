//!
//! \file "SizeMapParameter.h"
//! \brief Header file for SizeMapParameter class.
//!
//! \version    1.0
//! \date       19.04.2010 (last updated)
//!

#ifndef SizeMapParameter_H
#define SizeMapParameter_H

#include "FrapperPrerequisites.h"
#include "AbstractMapParameter.h"
#include "OgreEntity.h"

//!
//! Class representing parameters for vtk table values.
//!
class FRAPPER_CORE_EXPORT SizeMapParameter : public AbstractMapParameter 
{
public: // constructors and destructors

    //!
    //! Constructor of the SizeMapParameter class.
    //!
    //! \param name The name of the parameter.
    //!
    SizeMapParameter ( const QString &name );

	//!
    //! Destructor of the SizeMapParameter class.
    //!
    virtual ~SizeMapParameter ();

public: // functions

    //!
    //! Remove the Size item associate with the id.
    //!
	void removeSizeItem (const vtkIdType id) const;

    //!
    //! Add a Size item.
    //!
    //! \param id The id.
    //! \param size The size.
	//!
	void addSizeItem ( const vtkIdType id, Ogre::Vector3 size, const QString name = "");

    //!
    //! Returns the name of the Size item.
    //!
    //! \return The name of the Size item.
    //!
	QString getName (const vtkIdType id) const;

    //!
    //! Sets the name of the Size item.
    //!
    //! \param id The id.
    //! \param name The name of the Size item.
    //!
	void setName (const vtkIdType id, QString name);

    //!
    //! Returns the size associate with the id.
    //!
    //! \return The item size.
    //!
	Ogre::Vector3 getSize (const vtkIdType id) const;

    //!
    //! Sets the Size associated to the id.
    //!
    //! \param id The id.
    //! \param size The size.
    //!
	void setSize ( const vtkIdType id, const Ogre::Vector3 size);
};


#endif
