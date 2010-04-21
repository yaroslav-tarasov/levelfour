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
    parameterRoot->addParameter(new EnumerationParameter("From", Parameter::getDefaultValue(Parameter::T_Enumeration)));
	
    // create the enumeration parameter with the list of columns representing the edgeTo
    parameterRoot->addParameter(new EnumerationParameter("To", Parameter::getDefaultValue(Parameter::T_Enumeration)));

    // create the enumeration parameter with the list of fields representing the vertexID
    parameterRoot->addParameter(new EnumerationParameter("VertexID", Parameter::getDefaultValue(Parameter::T_Enumeration)));
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
	// load the input vtk parameter 
	VTKTableParameter * inputParameter = dynamic_cast<VTKTableParameter*>(getParameter(m_inputVTKTableParameterName));
	if (!inputParameter->isConnected())
		return;

	// get the source parameter (output of source node) connected to the input parameter
	VTKTableParameter * sourceParameter = dynamic_cast<VTKTableParameter*>(inputParameter->getConnectedParameter());

	// get the vtk table that comes with the source parameter and set it into the input parameter of this node
	vtkTable * inputTable = sourceParameter->getVTKTable();
	inputParameter->setVTKTable(inputTable);

	if (!inputTable)
		return;

	// recreate the From parameter with the list of attributes
	EnumerationParameter * edgesFromParameter = dynamic_cast<EnumerationParameter*>(getParameter("From"));
	QStringList literals;
	for (int i = 0; i < inputTable->GetNumberOfColumns(); i++)
		literals << inputTable->GetColumnName(i);

	edgesFromParameter->setLiterals(literals);
	edgesFromParameter->setValues(literals);

	// recreate the To parameter with the list of attributes
	EnumerationParameter * edgesToParameter = dynamic_cast<EnumerationParameter*>(getParameter("To"));

	edgesToParameter->setLiterals(literals);
	edgesToParameter->setValues(literals);

	// recreate the vertextID parameter with the list of attributes
	EnumerationParameter * vertexIDParameter = dynamic_cast<EnumerationParameter*>(getParameter("VertexID"));

	vertexIDParameter->setLiterals(literals);
	vertexIDParameter->setValues(literals);

	// get the attributes from the table and update the columns parameter
//	vtkDataSetAttributes * attributes = inputTable->GetRowData();
	QString name(inputTable->GetColumnName(1));
	vtkTableToGraph * tableToGraph = vtkTableToGraph::New();
	tableToGraph->SetInput(inputTable);
	tableToGraph->AddLinkVertex("Author");
	// process the output vtk graph 
	VTKGraphParameter * outputParameter = dynamic_cast<VTKGraphParameter*>(getParameter(m_outputVTKGraphName));

	if (outputParameter) 
		outputParameter->setVTKGraph(tableToGraph->GetOutput());
	
	m_graph = outputParameter->getVTKGraph();
}
