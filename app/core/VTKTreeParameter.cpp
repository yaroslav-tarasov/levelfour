//!
//! \file "VTKTreeParameter.cpp"
//! \brief Implementation file for VTKTreeParameter class.
//!
//! \version    1.0
//! \date       19.04.2010 (last updated)
//!

#include "VTKTreeParameter.h"

Q_DECLARE_METATYPE(vtkTree *)

///
/// Constructors and Destructors
///


//!
//! Constructor of the VTKTreeParameter class.
//!
//! \param name The name of the parameter.
//!
VTKTreeParameter::VTKTreeParameter ( const QString &name ) :
	VTKGraphParameter(name, Parameter::T_VTKTree, QVariant::fromValue<vtkTree *>(0))
{
}


//!
//! Destructor of the VTKTreeParameter class.
//!
VTKTreeParameter::~VTKTreeParameter ()
{
}

//!
//! Sets the vtk tree.
//!
//! \param values The vtk tree.
//!
void VTKTreeParameter::setVTKGraph ( vtkTree * tree )
{
	m_tree = tree;
}

//!
//! Get the vtk tree.
//!
vtkTree * VTKTreeParameter::getVTKGraph () const
{
	return m_tree;
}
