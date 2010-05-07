//!
//! \file "VTKGraphLayoutNode.h"
//! \brief Header file for VTKGraphLayoutNode class.
//!
//! \version    1.0
//! \date       18.05.2009 (last updated)
//!

#ifndef VTKGraphLayoutNode_H
#define VTKGraphLayoutNode_H

#include "VTKLayoutNode.h"
#include "vtkGraph.h"
#include "vtkGraphLayoutStrategy.h"
#include "vtkTable.h"
#include <QString>

//!
//! Class for a VTKGraphLayoutNode
//!
class FRAPPER_CORE_EXPORT VTKGraphLayoutNode : public VTKLayoutNode
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
    VTKGraphLayoutNode ( const QString &name, ParameterGroup *parameterRoot );

    //!
    //! Destructor of the VTKGraphLayoutNode class.
    //!
    //! Defined virtual to guarantee that the destructor of a derived class
    //! will be called if the instance of the derived class is saved in a
    //! variable of its parent class type.
    //!
    virtual ~VTKGraphLayoutNode ();

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
    //! return True if it succeeds, otherwise 1
    //!
    bool updateInputGraph ();

protected: // data 

	//!
    //! The name of the input vtk graph parameter.
    //!
    QString m_inputVTKGraphName;

    //!
    //! The name of the Z range parameter.
    //!
    QString m_zRangeParameterName;

	//!
    //! The input graph
    //!
    vtkGraph * m_inGraph;
};


#endif
