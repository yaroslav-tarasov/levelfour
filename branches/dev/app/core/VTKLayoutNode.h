//!
//! \file "VTKLayoutNode.h"
//! \brief Header file for VTKLayoutNode class.
//!
//! \version    1.0
//! \date       18.05.2009 (last updated)
//!

#ifndef VTKLayoutNode_H
#define VTKLayoutNode_H

#include "Node.h"
#include "vtkGraph.h"
#include "vtkGraphLayoutStrategy.h"
#include "vtkTable.h"
#include <QString>

//!
//! Macro definition that defines the type of the instance of the graph layout algorithm 
//! to be used (circular, radial, etc)
//!
#define LAYOUT_TYPE(className) \
	typedef className GraphLayout;

//!
//! Macro definition that force the dynamic cast of the specific layout 
//! to the instance of the graph layout algorithm 
//!
#define M_LAYOUT \
	dynamic_cast<GraphLayout*>(m_layoutInstance)

//!
//! Class for a VTKLayoutNode
//!
class FRAPPER_CORE_EXPORT VTKLayoutNode : public Node
{

    Q_OBJECT
    ADD_INSTANCE_COUNTER

public: // constructors and destructors

    //!
    //! Constructor of the VTKLayoutNode class.
    //!
    //! \param name The name for the new node.
    //! \param parameterRoot A copy of the parameter tree specific for the type of the node.
    //!
    VTKLayoutNode ( const QString &name, ParameterGroup *parameterRoot );

    //!
    //! Destructor of the VTKLayoutNode class.
    //!
    //! Defined virtual to guarantee that the destructor of a derived class
    //! will be called if the instance of the derived class is saved in a
    //! variable of its parent class type.
    //!
    virtual ~VTKLayoutNode ();

protected slots: //

    //!
    //! Processes the node's input data to generate the node's output table.
	//! This is layout implementation dependent.
    //!
	virtual void processParameters() = 0;

    //!
    //! Create the updated output
    //!
    virtual void refreshOutput() = 0;

protected: // functions

    //!
    //! Create a vtkTable with ID, x, y, z from an vtk graph 
    //!
	vtkTable * createTableFromGraph(vtkGraph *graph);

protected: // data 
	//!
    //! The graph layout algorithm to be used (circular, radial, etc)
    //!
	vtkGraphLayoutStrategy * m_layoutInstance;

    //!
    //! The name of the output vtk table parameter.
    //!
    QString m_ouputVTKTableParameterName;
	
	//!
    //! The output Table
    //!
    vtkTable * m_outputTable;
};


#endif
