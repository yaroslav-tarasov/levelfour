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
#include "VTKGraphParameter.h"
#include "vtkTree.h"

//!
//! Class representing parameters for vtk Tree values.
//!
class FRAPPER_CORE_EXPORT VTKTreeParameter : public VTKGraphParameter
{
	GRAPH_TYPE(vtkTree)
	
public: // constructors and destructors

    //!
    //! Constructor of the VTKTreeParameter class.
    //!
    //! \param name The name of the parameter.
    //!
    VTKTreeParameter ( const QString &name );

	//!
    //! Destructor of the VTKTreeParameter class.
    //!
    virtual ~VTKTreeParameter ();

    //!
    //! Sets the vtk tree.
    //!
    //! \param values The vtk tree.
    //!
    void setVTKGraph ( vtkTree * tree );

    //!
    //! Returns the vtk tree.
    //!
    //! \return The vtk tree.
    //!
    vtkTree * getVTKGraph () const;

protected: // data

    //!
    //! The vtk tree.
    //!
    vtkTree * m_tree;
};


#endif
