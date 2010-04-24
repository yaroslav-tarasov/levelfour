/*
-----------------------------------------------------------------------------
This source file is part of Acuity4D
research.animationsinstitut.de
sourceforge.net/projects/frapper

Copyright (c) 2008-2009 Filmakademie Baden-Wuerttemberg, Institute of Animation 

This program is free software; you can redistribute it and/or modify it under
the terms of the GNU Lesser General Public License as published by the Free Software
Foundation; version 2.1 of the License.

This program is distributed in the hope that it will be useful, but WITHOUT
ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
FOR A PARTICULAR PURPOSE. See the GNU Lesser General Public License for more details.

You should have received a copy of the GNU Lesser General Public License along with
this program; if not, write to the Free Software Foundation, Inc., 59 Temple
Place - Suite 330, Boston, MA 02111-1307, USA, or go to
http://www.gnu.org/licenses/old-licenses/lgpl-2.1.html
-----------------------------------------------------------------------------
*/

//!
//! \file "SingleCycleLayouterNode.h"
//! \brief Header file for SingleCycleLayouterNode class.
//!
//! \version    1.0
//! \date       18.05.2009 (last updated)
//!

#ifndef SingleCycleLayouterNODE_H
#define SingleCycleLayouterNODE_H

#include "Node.h"
#include "vtkGraph.h"
#include "vtkTree.h"
#include "vtkTable.h"
#include <QString>


//!
//! Class for a SingleCycleLayouter
//!
class SingleCycleLayouterNode : public Node
{

    Q_OBJECT
    ADD_INSTANCE_COUNTER

public: // constructors and destructors

    //!
    //! Constructor of the SingleCycleLayouterNode class.
    //!
    //! \param name The name for the new node.
    //! \param parameterRoot A copy of the parameter tree specific for the type of the node.
    //!
    SingleCycleLayouterNode ( const QString &name, ParameterGroup *parameterRoot );

    //!
    //! Destructor of the SingleCycleLayouterNode class.
    //!
    //! Defined virtual to guarantee that the destructor of a derived class
    //! will be called if the instance of the derived class is saved in a
    //! variable of its parent class type.
    //!
    virtual ~SingleCycleLayouterNode ();

private slots: //

    //!
    //! Processes the node's input data to generate the node's output table.
    //!
    void processOutputVTKTable();

    //!
    //! Update the input graph (called for example when input parameters have been dirtied)
    //! return It returns 0 if it succeeds, otherwise 1
    //!
    int updateInputGraph ();

protected: // functions

    //!
    //! Create a vtkTable with ID, x, y, z from an vtk graph 
    //!
	vtkTable * createTableFromGraph(vtkGraph *graph);

private: // data

    //!
    //! The name of the input vtk graph parameter.
    //!
    QString m_inputVTKGraphName;

    //!
    //! The name of the output vtk table parameter.
    //!
    QString m_ouputVTKTableParameterName;
	
	//!
    //! The output Table
    //!
    vtkTable * m_outputTable;

	//!
    //! The input graph
    //!
    vtkGraph * m_inGraph;
};


#endif
