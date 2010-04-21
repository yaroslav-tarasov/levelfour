//!
//! \file "VTKGraphParameter.cpp"
//! \brief Implementation file for VTKGraphParameter class.
//!
//! \version    1.0
//! \date       19.04.2010 (last updated)
//!

#include "VTKGraphParameter.h"

Q_DECLARE_METATYPE(vtkGraph *)

///
/// Constructors and Destructors
///


//!
//! Constructor of the VTKGraphParameter class.
//!
//! \param name The name of the parameter.
//!
VTKGraphParameter::VTKGraphParameter ( const QString &name ) :
	Parameter(name, Parameter::T_VTKGraph, QVariant::fromValue<vtkGraph *>(0)),
	m_graph(0)
{
}

//!
//! Copy constructor of the VTKGraphParameter class.
//!
//! \param parameter The parameter to copy.
//!
VTKGraphParameter::VTKGraphParameter ( const VTKGraphParameter &parameter ) :
    Parameter(parameter)
{
	m_graph = parameter.m_graph;
}

//!
//! Destructor of the VTKGraphParameter class.
//!
VTKGraphParameter::~VTKGraphParameter ()
{
}


///
/// Public Functions
///


//!
//! Creates an exact copy of the parameter.
//!
//! \return An exact copy of the parameter.
//!
AbstractParameter * VTKGraphParameter::clone ()
{
    return new VTKGraphParameter(*this);
}

//!
//! Returns the vtk graph.
//!
//! \return The vtk graph.
//!
vtkGraph * VTKGraphParameter::getVTKGraph() const
{
    return m_graph;
}

//!
//! Sets the vtk graph.
//!
//! \param values The vtk graph.
//!
void VTKGraphParameter::setVTKGraph ( vtkGraph * graph )
{
    m_graph = graph;
}
