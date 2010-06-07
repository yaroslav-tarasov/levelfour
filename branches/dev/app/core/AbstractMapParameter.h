//!
//! \file "AbstractMapParameter.h"
//! \brief Header file for AbstractMapParameter class.
//!
//! \version    1.0
//! \date       19.04.2010 (last updated)
//!

#ifndef AbstractMapParameter_H
#define AbstractMapParameter_H

#include "FrapperPrerequisites.h"
#include "VTKTableParameter.h"

#define COL_ID		0
#define COL_NAME	1

//!
//! Class representing parameters for vtk table values.
//!
class FRAPPER_CORE_EXPORT AbstractMapParameter : public VTKTableParameter 
{
public: // constructors and destructors

    //!
    //! Constructor of the AbstractMapParameter class.
    //!
    //! \param name The name of the parameter.
    //! \param type The type of the parameter.
    //!
    AbstractMapParameter ( const QString &name, Parameter::Type type );

	//!
    //! Destructor of the AbstractMapParameter class.
    //!
    virtual ~AbstractMapParameter ();

public: // functions

    //!
    //! Returns the name of the Abstract item.
    //!
    //! \return The name of the Abstract item.
    //!
	char * getName (const vtkIdType id) const;

    //!
    //! Sets the name of the Abstract item.
    //!
    //! \param id The id.
    //! \param name The name of the Abstract item.
    //!
	void setName (const vtkIdType id, vtkIdType name);
};


#endif
