//!
//! \file "VTKTableParameter.cpp"
//! \brief Implementation file for VTKTableParameter class.
//!
//! \version    1.0
//! \date       19.04.2010 (last updated)
//!

#include "VTKTableParameter.h"

Q_DECLARE_METATYPE(vtkTable *)

///
/// Constructors and Destructors
///


//!
//! Constructor of the VTKTableParameter class.
//!
//! \param name The name of the parameter.
//!
VTKTableParameter::VTKTableParameter ( const QString &name ) :
	Parameter(name, Parameter::T_VTKTable, QVariant::fromValue<vtkTable *>(0)),
	m_table(0)
{
}

//!
//! Copy constructor of the VTKTableParameter class.
//!
//! \param parameter The parameter to copy.
//!
VTKTableParameter::VTKTableParameter ( const VTKTableParameter &parameter ) :
    Parameter(parameter)
{
	m_table = parameter.m_table;
}

//!
//! Destructor of the VTKTableParameter class.
//!
VTKTableParameter::~VTKTableParameter ()
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
AbstractParameter * VTKTableParameter::clone ()
{
    return new VTKTableParameter(*this);
}

//!
//! Returns the vtk table.
//!
//! \return The vtk table.
//!
vtkTable * VTKTableParameter::getVTKTable() const
{
    return m_table;
}

//!
//! Sets the vtk table.
//!
//! \param values The vtk table.
//!
void VTKTableParameter::setVTKTable ( vtkTable * table )
{
    m_table = table;
}
