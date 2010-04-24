#ifndef VTKTreeNODE_H
#define VTKTreeNODE_H

#include "vtkTree.h"
#include "Node.h"

Q_DECLARE_METATYPE(vtkTree *)

//!
//! Base class representing nodes that provide a vtkTree output.
//!
class FRAPPER_CORE_EXPORT VTKTreeNode : public Node 
{

    Q_OBJECT

public: // constructors and destructors

    //!
    //! Constructor of the VTKTreeNode class.
    //!
    //! \param name The name to give the new mesh node.
    //! \param parameterRoot A copy of the parameter tree specific for the type of the node.
    //!
    VTKTreeNode ( const QString &name, ParameterGroup *parameterRoot, const QString &outputVTKTreeName /* = "VTKTree" */ );

    //!
    //! Destructor of the VTKTreeNode class.
    //!
    ~VTKTreeNode ();

    //!
    //! Return the current vtk Tree associated with this node.
    //!
	vtkTree * getTree() const;

signals:
    //!
    //! Signal that is emitted when the vtk Tree is changed.
    //!
	void treeChanged(vtkTree * Tree);
	void destroyed();

private slots: //

protected: // functions
	//!
    //! Update the vtk Tree associated with the text delimited source.
    //!
    //! \return void.
    //!
	virtual void updateTree() { emit treeChanged(m_Tree); }

protected: // data

	vtkTree * m_Tree;

    //!
    //! The name of the vtk Tree output parameter.
    //!
	QString m_outputVTKTreeName; 

};


#endif
