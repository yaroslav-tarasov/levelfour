//!
//! \file "VTKGraphParameter.h"
//! \brief Header file for VTKGraphParameter class.
//!
//! \version    1.0
//! \date       19.04.2010 (last updated)
//!

#ifndef VTKGraphParameter_H
#define VTKGraphParameter_H

#include "FrapperPrerequisites.h"
#include "Parameter.h"
#include "vtkGraph.h"

//!
//! Class representing parameters for vtk table values.
//!
class FRAPPER_CORE_EXPORT VTKGraphParameter : public Parameter
{

public: // constructors and destructors

    //!
    //! Constructor of the VTKGraphParameter class.
    //!
    //! \param name The name of the parameter.
    //!
    VTKGraphParameter ( const QString &name );

    //!
    //! Copy constructor of the VTKGraphParameter class.
    //!
    //! \param parameter The parameter to copy.
    //!
    VTKGraphParameter ( const VTKGraphParameter &parameter );

	//!
    //! Destructor of the VTKGraphParameter class.
    //!
    virtual ~VTKGraphParameter ();

public: // functions

    //!
    //! Creates an exact copy of the parameter.
    //!
    //! \return An exact copy of the parameter.
    //!
    virtual AbstractParameter * clone ();

    //!
    //! Returns the vtk graph.
    //!
    //! \return The vtk graph.
    //!
    vtkGraph * getVTKGraph () const;

    //!
    //! Sets the vtk graph.
    //!
    //! \param values The vtk graph.
    //!
    void setVTKGraph ( vtkGraph * graph );

private: // data

    //!
    //! The vtk graph.
    //!
    vtkGraph * m_graph;

};


#endif
