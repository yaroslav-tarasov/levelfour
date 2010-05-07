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

/*
-----------------------------------------------------------------------------
file "TableToTreeNode.cpp"
brief Implementation file for TableToTreeNode class.

version    1.0
date       18.05.2009 (last updated)

Description
-----------


Parameters
----------
      
Returns
-------

    
Notes
-----

Reference
---------

-----------------------------------------------------------------------------
*/


#include "VTKTreeParameter.h"
#include "VTKTableParameter.h"
#include "TableToTreeNode.h"
#include "vtkTableToTreeFilter.h"
#include "vtkTree.h"

INIT_INSTANCE_COUNTER(TableToTreeNode)

///
/// Constructors and Destructors
///

//!
//! Constructor of the TableToTreeNode class.
//!
//! \param name The name for the new node.
//! \param parameterRoot A copy of the parameter tree specific for the type of the node.
//!
TableToTreeNode::TableToTreeNode ( const QString &name, ParameterGroup *parameterRoot ) :
	Node(name, parameterRoot),
	m_inputVTKTableParameterName("VTKTableInput"),
	m_outputVTKTreeName("VTKTreeOutput"),
	m_inputTable(0),
	tableToTree(0),
	m_tree(0)
{
	setTypeName("TableToTreeNode");

	// create the mandatory vtk table input parameter - multiplicity *
	VTKTableParameter * inputVTKTableParameter = new VTKTableParameter(m_inputVTKTableParameterName);
	inputVTKTableParameter->setMultiplicity(1);
    inputVTKTableParameter->setPinType(Parameter::PT_Input);
    inputVTKTableParameter->setSelfEvaluating(true);
    parameterRoot->addParameter(inputVTKTableParameter);

    // create the mandatory vtk graph output parameter 
	VTKTreeParameter * outputVTKTreeParameter = new VTKTreeParameter(m_outputVTKTreeName);
	outputVTKTreeParameter->setPinType(Parameter::PT_Output);
    parameterRoot->addParameter(outputVTKTreeParameter);

    connect(inputVTKTableParameter, SIGNAL(dirtied()), SLOT(processOutputVTKTree()));

	INC_INSTANCE_COUNTER
}


//!
//! Destructor of the TableToTreeNode class.
//!
//! Defined virtual to guarantee that the destructor of a derived class
//! will be called if the instance of the derived class is saved in a
//! variable of its parent class type.
//!
TableToTreeNode::~TableToTreeNode ()
{
	emit destroyed();

	if (tableToTree)
		tableToTree->Delete();

	Log::info(QString("TableToTreeNode destroyed."), "TableToTreeNode::~TableToTreeNode");
    DEC_INSTANCE_COUNTER
}

//!
//! Processes the node's input data to generate the node's output table.
//!
void TableToTreeNode::processOutputVTKTree ()
{
	if (!updateTable())
		return;

	updateTree();
}

//!
//! Update the input table 
//!
bool TableToTreeNode::updateTable ()
{
	// load the input vtk parameter 
	VTKTableParameter * inputParameter = dynamic_cast<VTKTableParameter*>(getParameter(m_inputVTKTableParameterName));
	if (!inputParameter->isConnected())
		return false;

	// get the source parameter (output of source node) connected to the input parameter
	VTKTableParameter * sourceParameter = dynamic_cast<VTKTableParameter*>(inputParameter->getConnectedParameter());

	// get the vtk table that comes with the source parameter and set it into the input parameter of this node
	m_inputTable = sourceParameter->getVTKTable();
	inputParameter->setVTKTable(m_inputTable);

	return (m_inputTable != 0);
}

//!
//! Update the Tree (called for example when parameters change)
//!
void TableToTreeNode::updateTree ()
{
	if (!m_inputTable)
		return;

	if (!tableToTree)
		tableToTree = vtkTableToTreeFilter::New();

	tableToTree->SetInput(m_inputTable);

	tableToTree->Update();
	m_tree = tableToTree->GetOutput();

	int i = m_tree->GetNumberOfVertices();

	VTKTreeParameter * outputParameter = dynamic_cast<VTKTreeParameter*>(getParameter(m_outputVTKTreeName));

	if (outputParameter) 
	{
		outputParameter->setVTKGraph(m_tree);
		outputParameter->propagateDirty(0);
	}
}
