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
//! \file "TableToGraphNode.h"
//! \brief Header file for TableToGraphNode class.
//!
//! \version    1.0
//! \date       18.05.2009 (last updated)
//!

#ifndef TableToTreeNODE_H
#define TableToTreeNODE_H

#include "vtkTree.h"
#include "vtkTable.h"
#include "Node.h"
#include "vtkTableToTreeFilter.h"

//!
//! Class for a Degree Centrality Measure
//!
class TableToTreeNode : public Node
{

    Q_OBJECT
    ADD_INSTANCE_COUNTER

public: // constructors and destructors

    //!
    //! Constructor of the TableToGraphNode class.
    //!
    //! \param name The name for the new node.
    //! \param parameterRoot A copy of the parameter tree specific for the type of the node.
    //!
    TableToTreeNode ( const QString &name, ParameterGroup *parameterRoot );

    //!
    //! Destructor of the TableToGraphNode class.
    //!
    //! Defined virtual to guarantee that the destructor of a derived class
    //! will be called if the instance of the derived class is saved in a
    //! variable of its parent class type.
    //!
    virtual ~TableToTreeNode ();

protected slots: //

    //!
    //! Processes the node's input data to generate the node's output table.
    //!
    virtual void processOutputVTKTree ();

	//!
    //! Update the tree (called for example when parameters change)
    //!
    virtual void updateTree ();

    //!
    //! Update the input table 
    //! \return True if it succeeds, otherwise false
    //!
    bool updateTable ();

private: // data

    //!
    //! The name of the input vtk table parameter.
    //!
    QString m_inputVTKTableParameterName;

    //!
    //! The name of the output vtk tree parameter.
    //!
    QString m_outputVTKTreeName;

    //!
    //! The input table 
    //!
	vtkTable * m_inputTable;

	//!
    //! The resulting tree
    //!
	vtkTree* m_tree;


	//!
    //! The resulting tree
    //!
	vtkTableToTreeFilter * tableToTree;
};


#endif
