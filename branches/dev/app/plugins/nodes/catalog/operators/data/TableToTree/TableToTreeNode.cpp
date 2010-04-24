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
#include "vtkVertexListIterator.h"

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
	m_Tree(0)
{
    // create the mandatory vtk table input parameter - multiplicity *
	VTKTableParameter * inputVTKTableParameter = new VTKTableParameter(m_inputVTKTableParameterName);
	inputVTKTableParameter->setMultiplicity(1);
    inputVTKTableParameter->setPinType(Parameter::PT_Input);
    inputVTKTableParameter->setSelfEvaluating(true);
    parameterRoot->addParameter(inputVTKTableParameter);
    connect(inputVTKTableParameter, SIGNAL(dirtied()), SLOT(processOutputVTKTree()));

    // create the mandatory vtk Tree output parameter 
	VTKTreeParameter * outputVTKTreeParameter = new VTKTreeParameter(m_outputVTKTreeName);
    outputVTKTreeParameter->setPinType(Parameter::PT_Output);
    parameterRoot->addParameter(outputVTKTreeParameter);

	// link the input parameter to the output processing
    if (outputVTKTreeParameter) 
	{
		outputVTKTreeParameter->setProcessingFunction(SLOT(processOutputVTKTree()));
        outputVTKTreeParameter->addAffectingParameter(inputVTKTableParameter);
	}

    // create the enumeration parameter with the list of columns representing the edgeFrom
	// edgesFromParameter = new EnumerationParameter("From", Parameter::getDefaultValue(Parameter::T_Enumeration));
	// parameterRoot->addParameter(edgesFromParameter);
	
    // create the enumeration parameter with the list of columns representing the edgeTo
	// edgesToParameter = new EnumerationParameter("To", Parameter::getDefaultValue(Parameter::T_Enumeration));
    // parameterRoot->addParameter(edgesToParameter);

    // create the enumeration parameter with the list of fields representing the vertexID
	// vertexIDParameter = new EnumerationParameter("VertexID", Parameter::getDefaultValue(Parameter::T_Enumeration));
    // parameterRoot->addParameter(vertexIDParameter);

	// connect(edgesFromParameter, SIGNAL(dirtied()), this, SLOT(updateTree()));
	// connect(edgesToParameter, SIGNAL(dirtied()), this, SLOT(updateTree()));
	// connect(vertexIDParameter, SIGNAL(dirtied()), this, SLOT(updateTree()));

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
    Log::info(QString("TableToTreeNode destroyed."), "TableToTreeNode::~TableToTreeNode");
    DEC_INSTANCE_COUNTER
}


//!
//! Processes the node's input data to generate the node's output table.
//!
void TableToTreeNode::processOutputVTKTree ()
{
	if (updateTable() != 0)
		return;

	// recreate the From/To and vertices parameter with the list of the input table attributes
	QStringList literals;
	for (int i = 0; i < m_inputTable->GetNumberOfColumns(); i++)
		literals << m_inputTable->GetColumnName(i);

	// edgesFromParameter->setLiterals(literals);
	// edgesFromParameter->setValues(literals);

	// edgesToParameter->setLiterals(literals);
	// edgesToParameter->setValues(literals);

	// vertexIDParameter->setLiterals(literals);
	// vertexIDParameter->setValues(literals);

}

//!
//! Update the Tree (called for example when parameters change)
//!
void TableToTreeNode::updateTree ()
{
	if (updateTable() != 0)
		return;

	vtkTableToTreeFilter * tableToTree = vtkTableToTreeFilter::New();
	tableToTree->SetInput(m_inputTable);
	// QString vertexID = vertexIDParameter->getCurrentLiteral();
	// QString edgeFrom = edgesFromParameter->getCurrentLiteral();
	// QString edgeTo = edgesToParameter->getCurrentLiteral();
	// tableToTree->AddLinkVertex(vertexID.toLatin1());
	// tableToTree->AddLinkEdge(edgeFrom.toLatin1(), edgeTo.toLatin1());

	m_Tree = tableToTree->GetOutput();

	// vtkVertexListIterator *vertices = vtkVertexListIterator::New();
	// m_Tree->GetVertices(vertices);

	// process the output vtk Tree 
	VTKTreeParameter * outputParameter = dynamic_cast<VTKTreeParameter*>(getParameter(m_outputVTKTreeName));

	if (outputParameter) 
	{
		outputParameter->setVTKTree(m_Tree);
		outputParameter->propagateDirty(0);
	}

	tableToTree->Delete();
}

//!
//! Update the input table 
//!
int TableToTreeNode::updateTable ()
{
	// load the input vtk parameter 
	VTKTableParameter * inputParameter = dynamic_cast<VTKTableParameter*>(getParameter(m_inputVTKTableParameterName));
	if (!inputParameter->isConnected())
		return 1;

	// get the source parameter (output of source node) connected to the input parameter
	VTKTableParameter * sourceParameter = dynamic_cast<VTKTableParameter*>(inputParameter->getConnectedParameter());

	// get the vtk table that comes with the source parameter and set it into the input parameter of this node
	m_inputTable = sourceParameter->getVTKTable();
	inputParameter->setVTKTable(m_inputTable);

	return (m_inputTable == 0);
}
