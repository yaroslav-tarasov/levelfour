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
	Parameter(name, Parameter::T_VTKTree, QVariant::fromValue<vtkTree *>(0)),
	m_Tree(0)
{
}

//!
//! Copy constructor of the VTKTreeParameter class.
//!
//! \param parameter The parameter to copy.
//!
VTKTreeParameter::VTKTreeParameter ( const VTKTreeParameter &parameter ) :
    Parameter(parameter)
{
	m_Tree = parameter.m_Tree;
}

//!
//! Destructor of the VTKTreeParameter class.
//!
VTKTreeParameter::~VTKTreeParameter ()
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
AbstractParameter * VTKTreeParameter::clone ()
{
    return new VTKTreeParameter(*this);
}

//!
//! Returns the vtk Tree.
//!
//! \return The vtk Tree.
//!
vtkTree * VTKTreeParameter::getVTKTree() const
{
    return m_Tree;
}

//!
//! Sets the vtk Tree.
//!
//! \param values The vtk Tree.
//!
void VTKTreeParameter::setVTKTree ( vtkTree * Tree )
{
    m_Tree = Tree;
}
