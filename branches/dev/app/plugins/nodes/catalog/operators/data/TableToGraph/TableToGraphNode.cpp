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
file "TableToGraphNode.cpp"
brief Implementation file for TableToGraphNode class.

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


#include "VTKGraphParameter.h"
#include "VTKTableParameter.h"
#include "TableToGraphNode.h"
#include "vtkDataSetAttributes.h"
#include "vtkTableToGraph.h"
#include "vtkGraph.h"

///
/// Constructors and Destructors
///

//!
//! Constructor of the TableToGraphNode class.
//!
//! \param name The name for the new node.
//! \param parameterRoot A copy of the parameter tree specific for the type of the node.
//!
TableToGraphNode::TableToGraphNode ( const QString &name, ParameterGroup *parameterRoot ) :
	ViewNode(name, parameterRoot),
	m_inputVTKTableParameterName("VTKTableInput"),
	m_outputVTKGraphName("VTKGraphOutput"),
	m_inputTable(0),
	m_graph(0)
{
    // create the mandatory vtk table input parameter - multiplicity *
	VTKTableParameter * inputVTKTableParameter = new VTKTableParameter(m_inputVTKTableParameterName);
	inputVTKTableParameter->setMultiplicity(1);
    inputVTKTableParameter->setPinType(Parameter::PT_Input);
    inputVTKTableParameter->setSelfEvaluating(true);
    parameterRoot->addParameter(inputVTKTableParameter);
    connect(inputVTKTableParameter, SIGNAL(dirtied()), SLOT(processOutputVTKGraph()));

    // create the mandatory vtk graph output parameter 
	addOutputParameter(new VTKGraphParameter(m_outputVTKGraphName));

	// link the input parameter to the output processing
	Parameter * outputParameter = getParameter(m_outputVTKGraphName);
    if (outputParameter) 
	{
		outputParameter->setProcessingFunction(SLOT(processOutputVTKGraph()));
        outputParameter->addAffectingParameter(inputVTKTableParameter);
	}

    // create the enumeration parameter with the list of columns representing the edgeFrom
	edgesFromParameter = new EnumerationParameter("From", Parameter::getDefaultValue(Parameter::T_Enumeration));
	parameterRoot->addParameter(edgesFromParameter);
	
    // create the enumeration parameter with the list of columns representing the edgeTo
	edgesToParameter = new EnumerationParameter("To", Parameter::getDefaultValue(Parameter::T_Enumeration));
    parameterRoot->addParameter(edgesToParameter);

    // create the enumeration parameter with the list of fields representing the vertexID
	vertexIDParameter = new EnumerationParameter("VertexID", Parameter::getDefaultValue(Parameter::T_Enumeration));
    parameterRoot->addParameter(vertexIDParameter);

	connect(edgesFromParameter, SIGNAL(dirtied()), this, SLOT(updateGraph()));
	connect(edgesToParameter, SIGNAL(dirtied()), this, SLOT(updateGraph()));
	connect(vertexIDParameter, SIGNAL(dirtied()), this, SLOT(updateGraph()));
}


//!
//! Destructor of the TableToGraphNode class.
//!
//! Defined virtual to guarantee that the destructor of a derived class
//! will be called if the instance of the derived class is saved in a
//! variable of its parent class type.
//!
TableToGraphNode::~TableToGraphNode ()
{
}


//!
//! Processes the node's input data to generate the node's output table.
//!
void TableToGraphNode::processOutputVTKGraph ()
{
	if (updateTable() != 0)
		return;

	// recreate the From/To and vertices parameter with the list of the input table attributes
	QStringList literals;
	for (int i = 0; i < m_inputTable->GetNumberOfColumns(); i++)
		literals << m_inputTable->GetColumnName(i);

	edgesFromParameter->setLiterals(literals);
	edgesFromParameter->setValues(literals);

	edgesToParameter->setLiterals(literals);
	edgesToParameter->setValues(literals);

	vertexIDParameter->setLiterals(literals);
	vertexIDParameter->setValues(literals);

}

//!
//! Update the graph (called for example when parameters change)
//!
void TableToGraphNode::updateGraph ()
{
	if (updateTable() != 0)
		return;

	vtkTableToGraph * tableToGraph = vtkTableToGraph::New();
	tableToGraph->SetInput(m_inputTable);
	QString vertexID = vertexIDParameter->getCurrentLiteral();
	QString edgeFrom = edgesFromParameter->getCurrentLiteral();
	QString edgeTo = edgesToParameter->getCurrentLiteral();
	tableToGraph->AddLinkVertex(vertexID.toLatin1());
	tableToGraph->AddLinkEdge(edgeFrom.toLatin1(), edgeTo.toLatin1());

	m_graph = tableToGraph->GetOutput();
	// process the output vtk graph 
	VTKGraphParameter * outputParameter = dynamic_cast<VTKGraphParameter*>(getParameter(m_outputVTKGraphName));

	if (outputParameter) 
		outputParameter->setVTKGraph(m_graph);

	tableToGraph->Delete();
}

//!
//! Update the input table 
//!
int TableToGraphNode::updateTable ()
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
