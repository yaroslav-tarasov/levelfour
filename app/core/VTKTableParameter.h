//!
//! \file "VTKTableParameter.h"
//! \brief Header file for VTKTableParameter class.
//!
//! \version    1.0
//! \date       19.04.2010 (last updated)
//!

#ifndef VTKTABLEPARAMETER_H
#define VTKTABLEPARAMETER_H

#include "FrapperPrerequisites.h"
#include "Parameter.h"
#include "vtkTable.h"

//!
//! Class representing parameters for vtk table values.
//!
class FRAPPER_CORE_EXPORT VTKTableParameter : public Parameter
{

public: // constructors and destructors

    //!
    //! Constructor of the VTKTableParameter class.
    //!
    //! \param name The name of the parameter.
    //!
    VTKTableParameter ( const QString &name );

    //!
    //! Copy constructor of the VTKTableParameter class.
    //!
    //! \param parameter The parameter to copy.
    //!
    VTKTableParameter ( const VTKTableParameter &parameter );

	//!
    //! Destructor of the VTKTableParameter class.
    //!
    virtual ~VTKTableParameter ();

public: // functions

    //!
    //! Creates an exact copy of the parameter.
    //!
    //! \return An exact copy of the parameter.
    //!
    virtual AbstractParameter * clone ();

    //!
    //! Returns the vtk table.
    //!
    //! \return The vtk table.
    //!
    vtkTable * getVTKTable () const;

    //!
    //! Sets the vtk table.
    //!
    //! \param values The vtk table.
    //!
    void setVTKTable ( vtkTable * table );

private: // data

    //!
    //! The vtk table.
    //!
    vtkTable * m_table;

};


#endif
