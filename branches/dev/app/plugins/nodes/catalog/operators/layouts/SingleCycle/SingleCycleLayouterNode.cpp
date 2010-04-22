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
file "SingleCycleLayouterNode.cpp"
brief Implementation file for SingleCycleLayouterNode class.

version    1.0
date       21.04.2010 (last updated)

Description
-----------

Circular layout is the most prominent and oldest conventions used to draw graphs. Circular graph layout is a drawing scheme 
where all nodes are placed on the perimeter of a circle. In such a layout, the edges connecting these nodes are passing 
within the circle.

This node is based on the vtkCircularLayoutStrategy implementation.

Inputs
----------
      
	In: vtkGraph

Outputs
-------

	Out: vtkTable (used for data model)

Result
------

A vtkTable is produced with 4 columns: nodeId, posX, posY, posZ.

Properties
-------

	AngularRadius: max angular radius of circle (0-360)
	EdgeWeightField:

Notes
-----

Layouters are based on a generic pipeline of: 
	a.) vtkTable->vtkTableToGraph->vtkGraph->vtkGraphLayout->vtkTable

Given this pipeline, vtkCircularLayoutStrategy is set to the vtkGraphLayout.

***AngularRadius is not currently implemented in vtkCircularLayoutStrategy.

Reference
---------

M. Baur and U. Brandes, “Crossing Reduction in Circular
Layouts”, Proc. Graph-Theoretic Concepts in Computer-
Science (WG ’04), 2004, pp.332-343.

Dogrusoz, Ugur and Madden, Brendan and Madden, Patrick,
Circular layout in the Graph Layout Toolkit”, 
Proc. Graph Drawing (GD ’96), 1996, pp.92–100.

-----------------------------------------------------------------------------
*/

#include "SingleCycleLayouterNode.h"
#include "vtkGraphLayout.h"
#include "vtkCircularLayoutStrategy.h"
#include "vtkTable.h"
#include "vtkGraph.h"
#include "vtkVertexListIterator.h"
#include "vtkStringArray.h"
#include "vtkFloatArray.h"
#include "vtkVariant.h"

#include "VTKGraphParameter.h"
#include "VTKTableParameter.h"

INIT_INSTANCE_COUNTER(SingleCycleLayouterNode)
///
/// Constructors and Destructors
///

//!
//! Constructor of the SingleCycleLayouterNode class.
//!
//! \param name The name for the new node.
//! \param parameterRoot A copy of the parameter tree specific for the type of the node.
//!
SingleCycleLayouterNode::SingleCycleLayouterNode ( const QString &name, ParameterGroup *parameterRoot ) :
    ViewNode(name, parameterRoot),
	m_ouputVTKTableParameterName("VTKTableOutput"),
	m_inputVTKGraphName("VTKGraphInput"),
	m_outputTable(0),
	m_inGraph(0)
{
	setTypeName("SingleCycleLayouterNode");

    // create the mandatory vtk graph input parameter 
	VTKGraphParameter * inputVTKGraphParameter = new VTKGraphParameter(m_inputVTKGraphName);
	inputVTKGraphParameter->setMultiplicity(1);
    inputVTKGraphParameter->setPinType(Parameter::PT_Input);
    inputVTKGraphParameter->setSelfEvaluating(true);
    parameterRoot->addParameter(inputVTKGraphParameter);
	connect(inputVTKGraphParameter, SIGNAL(dirtied()), SLOT(processOutputVTKTable()));

    // create the mandatory vtk table output parameter 
	addOutputParameter(new VTKTableParameter(m_ouputVTKTableParameterName));

	// link the input parameter to the output processing
	Parameter * outputParameter = getParameter(m_ouputVTKTableParameterName);
    if (outputParameter) 
	{
		outputParameter->setProcessingFunction(SLOT(processOutputVTKTable()));
        outputParameter->addAffectingParameter(inputVTKGraphParameter);
	}
    INC_INSTANCE_COUNTER
}


//!
//! Destructor of the SingleCycleLayouterNode class.
//!
//! Defined virtual to guarantee that the destructor of a derived class
//! will be called if the instance of the derived class is saved in a
//! variable of its parent class type.
//!
SingleCycleLayouterNode::~SingleCycleLayouterNode ()
{
	emit destroyed();
    DEC_INSTANCE_COUNTER
    Log::info(QString("SingleCycleLayouterNode destroyed."), "SingleCycleLayouterNode::~SingleCycleLayouterNode");
}

//!
//! Processes the node's input data to generate the node's output table.
//!
void SingleCycleLayouterNode::processOutputVTKTable()
{
	if (updateInputGraph() != 0)
		return;

	vtkGraphLayout *layout = vtkGraphLayout::New();
	layout->SetInput( m_inGraph );
	vtkCircularLayoutStrategy *circular = vtkCircularLayoutStrategy::New();
	layout->SetLayoutStrategy(circular);
	layout->Update();

	m_outputTable = createTableFromGraph(layout->GetOutput());

	// process the output vtk table
	VTKTableParameter * outputParameter = dynamic_cast<VTKTableParameter*>(getParameter(m_ouputVTKTableParameterName));

	if (outputParameter) 
		outputParameter->setVTKTable(m_outputTable);
}

vtkTable * SingleCycleLayouterNode::createTableFromGraph(vtkGraph *graph)
{
	vtkTable *myTable = vtkTable::New();
	//Create a column named "NodeId"
	vtkStringArray *colNodeId = vtkStringArray::New();
	colNodeId->SetName("NodeId");
	myTable->AddColumn(colNodeId);
	//Create columns named "X", "Y" and "Z"
	vtkFloatArray *colX = vtkFloatArray::New();
	colX->SetName("X");
	vtkFloatArray *colY = vtkFloatArray::New();
	colY->SetName("Y");
	vtkFloatArray *colZ = vtkFloatArray::New();
	colZ->SetName("Z");

	//Fill the table with data from the graph vertices
	vtkVertexListIterator *vertices = vtkVertexListIterator::New();
	graph->GetVertices(vertices);
	double position[3] = {0.0, 0.0, 0.0};
	vtkIdType i = 0;
	while (vertices->HasNext())
	{
		vertices->Next();
		//Add the vertex ID to the "NodeId" column
		char vertexId[10];
		sprintf(vertexId, "%d",i); //Convert the vertex ID to a string
		colNodeId->InsertNextValue( vertexId );
		//Add the position values to columns "X", "Y" and "Z"
		graph->GetPoint(i, position);
		colX->InsertNextValue( position[0] );
		colY->InsertNextValue( position[1] );
		colZ->InsertNextValue( position[2] );
		++i;
	}
	myTable->AddColumn(colX);
	myTable->AddColumn(colY);
	myTable->AddColumn(colZ);
	Log::info(QString("Number rows \"%1\" created: ").arg(myTable->GetNumberOfRows()), "SingleCycleLayouterNode::createTableFromGraph");
	Log::info(QString("Number columns \"%1\" created: ").arg(myTable->GetNumberOfColumns()), "SingleCycleLayouterNode::createTableFromGraph");
	return myTable;
}

//!
//! Update the input graph
//!
int SingleCycleLayouterNode::updateInputGraph()
{
	// load the input vtk parameter 
	VTKGraphParameter * inputParameter = dynamic_cast<VTKGraphParameter*>(getParameter(m_inputVTKGraphName));
	if (!inputParameter->isConnected())
		return 1;

	// get the source parameter (output of source node) connected to the input parameter
	VTKGraphParameter * sourceParameter = dynamic_cast<VTKGraphParameter*>(inputParameter->getConnectedParameter());

	// get the vtk graph that comes with the source parameter and set it into the input parameter of this node
	m_inGraph = sourceParameter->getVTKGraph();
	inputParameter->setVTKGraph(m_inGraph);

	return (m_inGraph == 0);
}
