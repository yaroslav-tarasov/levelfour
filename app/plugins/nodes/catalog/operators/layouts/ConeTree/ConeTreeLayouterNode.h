//!
//! \file "ConeTreeLayouterNode.h"
//! \brief Header file for ConeTreeLayouterNode class.
//!

#ifndef ConeTreeLayouterNode_H
#define ConeTreeLayouterNode_H

#include "Node.h"
#include "vtkGraph.h"
#include "vtkTree.h"
#include "vtkTable.h"
#include <QString>

class ConeTreeLayouterNode : public Node
{

    Q_OBJECT
    ADD_INSTANCE_COUNTER

public: // constructors and destructors

    //!
    //! Constructor of the ConeTreeLayouterNode class.
    //!
    //! \param name The name for the new node.
    //! \param parameterRoot A copy of the parameter tree specific for the type of the node.
    //!
    ConeTreeLayouterNode ( const QString &name, ParameterGroup *parameterRoot );

    //!
    //! Destructor of the ConeTreeLayouterNode class.
    //!
    //! Defined virtual to guarantee that the destructor of a derived class
    //! will be called if the instance of the derived class is saved in a
    //! variable of its parent class type.
    //!
    virtual ~ConeTreeLayouterNode ();

private slots: //

    //!
    //! Processes the node's input data to generate the node's output table.
    //!
    void processOutputVTKTable();

	//!
    //! Update the input graph (called for example when input parameters have been dirtied)
    //! return It returns 0 if it succeeds, otherwise 1
    //!
    int updateInputTree ();

protected: // functions

    //!
    //! Create a vtkTable with ID, x, y, z from an vtk graph 
    //!
	vtkTable * createTableFromGraph(vtkGraph *graph);

private: // data

    //!
    //! The name of the input vtk tree parameter.
    //!
    QString m_inputVTKTreeName;

    //!
    //! The name of the output vtk table parameter.
    //!
    QString m_ouputVTKTableParameterName;
	
	//!
    //! The output Table
    //!
    vtkTable * m_outputTable;

	//!
    //! The input tree
    //!
    vtkTree * m_inTree;

};


#endif
