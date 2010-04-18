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
date       18.05.2009 (last updated)

Description
-----------

Circular layout is the most prominent and oldest conventions used to draw graphs. Circular graph layout is a drawing scheme 
where all nodes are placed on the perimeter of a circle. In such a layout, the edges connecting these nodes are passing 
within the circle.

This node is based on the vtkCircularLayoutStrategy implementation.

Inputs
----------
      
	In: vtkTree
	InVertex: vtkTable
	InEdge: vtkTable

Outputs
-------

	Out: vtkTable (used for data model)
	Out: vtkGraph (used for measure processing)

Result
------

A vtkTable is produced with 4 columns: nodeId, posX, posY, posZ.
A vtkGraph is produced for use in measures (e.g., centrality)

Properties
-------

	AngularRadius: max angular radius of circle (0-360)
	EdgeWeightField:

Notes
-----

Layouters are based on a generic pipeline of: 
	a.) vtkTable->vtkTableToGraph->vtkGraphLayout->vtkGraph->vtkTable
	b.) vtkTree->vtkGraphLayout->vtkGraph->vtkTable

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
    Node(name, parameterRoot)
		/*
		m_nTable(0),
		m_nGraph(0)
		*/
{

	// Set source
	// if source is vtkTable(s), apply vtkTableToGraph

	// Create layout (vtkGraphLayout)

	// Set layout strategy (vtkCircularLayoutStrategy)

	// Create graph ouput (m_oGraph)

	// Create table ouput (m_oTable)

	// Set affections and callback functions

	// Create output vtkGraph parameter

	// Create output vtkTable parameter

	setTypeName("SingleCycleLayouterNode");

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

bool SingleCycleLayouterNode::loadVertexTable()
{
	return true;
}

bool SingleCycleLayouterNode::loadEdgeTable()
{
	return true;
}

bool SingleCycleLayouterNode::loadGraphTree()
{
	return true;
}

vtkTable *SingleCycleLayouterNode::createTableFromGraph(vtkGraph *graph)
{
	m_nTable = vtkTable::New();
	//Create a column named "NodeId"
	vtkStringArray *colNodeId = vtkStringArray::New();
	colNodeId->SetName("NodeId");
	m_nTable->AddColumn(colNodeId);
	//Create columns named "X", "Y" and "Z"
	vtkFloatArray *posX = vtkFloatArray::New();
	posX->SetName("X");
	m_nTable->AddColumn(posX);
	vtkFloatArray *posY = vtkFloatArray::New();
	posY->SetName("Y");
	m_nTable->AddColumn(posY);
	vtkFloatArray *posZ = vtkFloatArray::New();
	posZ->SetName("Z");
	m_nTable->AddColumn(posZ);

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
		posX->InsertNextValue( position[0] );
		posY->InsertNextValue( position[1] );
		posZ->InsertNextValue( position[2] );
		++i;
	}
	return m_nTable;
}

vtkGraph *SingleCycleLayouterNode::createGraphFromTable(vtkTable *table)
{
	// vtkTableToGraph filter applied here
	// This is where table columns are mapped to "from" and "to"
	// AddLinkVertex
	// AddLinkEdge

	return m_nGraph;
}

vtkGraph *SingleCycleLayouterNode::createGraphFromTree(vtkTree *tree)
{
	return m_nGraph;
}