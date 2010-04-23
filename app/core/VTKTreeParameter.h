//!
//! \file "VTKTreeParameter.h"
//! \brief Header file for VTKTreeParameter class.
//!
//! \version    1.0
//! \date       19.04.2010 (last updated)
//!

#ifndef VTKTreePARAMETER_H
#define VTKTreePARAMETER_H

#include "FrapperPrerequisites.h"
#include "Parameter.h"
#include "vtkTree.h"

//!
//! Class representing parameters for vtk Tree values.
//!
class FRAPPER_CORE_EXPORT VTKTreeParameter : public Parameter
{

public: // constructors and destructors

    //!
    //! Constructor of the VTKTreeParameter class.
    //!
    //! \param name The name of the parameter.
    //!
    VTKTreeParameter ( const QString &name );

    //!
    //! Copy constructor of the VTKTreeParameter class.
    //!
    //! \param parameter The parameter to copy.
    //!
    VTKTreeParameter ( const VTKTreeParameter &parameter );

	//!
    //! Destructor of the VTKTreeParameter class.
    //!
    virtual ~VTKTreeParameter ();

public: // functions

    //!
    //! Creates an exact copy of the parameter.
    //!
    //! \return An exact copy of the parameter.
    //!
    virtual AbstractParameter * clone ();

    //!
    //! Returns the vtk Tree.
    //!
    //! \return The vtk Tree.
    //!
    vtkTree * getVTKTree () const;

    //!
    //! Sets the vtk Tree.
    //!
    //! \param values The vtk Tree.
    //!
    void setVTKTree ( vtkTree * Tree );

private: // data

    //!
    //! The vtk Tree.
    //!
    vtkTree * m_Tree;

};


#endif
