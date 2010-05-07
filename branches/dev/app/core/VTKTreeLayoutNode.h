//!
//! \file "VTKTreeLayoutNode.h"
//! \brief Header file for VTKGraphLayoutNode class.
//!
//! \version    1.0
//! \date       18.05.2009 (last updated)
//!

#ifndef VTKTreeLayoutNode_H
#define VTKTreeLayoutNode_H

#include "VTKLayoutNode.h"
#include "vtkTree.h"
#include <QString>

//!
//! Class for a VTKTreeLayoutNode
//!
class FRAPPER_CORE_EXPORT VTKTreeLayoutNode : public VTKLayoutNode
{

    Q_OBJECT
    ADD_INSTANCE_COUNTER

public: // constructors and destructors

    //!
    //! Constructor of the VTKGraphLayoutNode class.
    //!
    //! \param name The name for the new node.
    //! \param parameterRoot A copy of the parameter tree specific for the type of the node.
    //!
    VTKTreeLayoutNode ( const QString &name, ParameterGroup *parameterRoot );

    //!
    //! Destructor of the VTKGraphLayoutNode class.
    //!
    //! Defined virtual to guarantee that the destructor of a derived class
    //! will be called if the instance of the derived class is saved in a
    //! variable of its parent class type.
    //!
    virtual ~VTKTreeLayoutNode ();

protected slots: //

    //!
    //! Processes the node's input data to generate the node's output table.
	//! This is layout implementation dependent.
    //!
	virtual void processParameters() { refreshOutput();}

    //!
    //! Create the updated output
    //!
    virtual void refreshOutput();

    //!
    //! Update the input graph (called for example when input parameters have been dirtied)
    //! return It returns 0 if it succeeds, otherwise 1
    //!
    int updateInputTree ();

protected: // data

    //!
    //! The name of the input vtk graph parameter.
    //!
    QString m_inputVTKTreeName;
	
	//!
    //! The input Tree
    //!
    vtkTree * m_inputVTKTree;
};


#endif
