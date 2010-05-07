/*
-----------------------------------------------------------------------------
file "VTKLayoutNode.cpp"
brief Implementation file for VTKLayoutNode class.

version    1.0
date       21.04.2010 (last updated)

*/

#include "VTKLayoutNode.h"
#include "vtkGraphLayout.h"
#include "vtkCircularLayoutStrategy.h"
#include "vtkTable.h"
#include "vtkGraph.h"
#include "vtkVertexListIterator.h"
#include "vtkIdTypeArray.h"
#include "vtkDoubleArray.h"
#include "vtkVariant.h"

#include "VTKGraphParameter.h"
#include "VTKTableParameter.h"

INIT_INSTANCE_COUNTER(VTKLayoutNode)
///
/// Constructors and Destructors
///

//!
//! Constructor of the VTKLayoutNode class.
//!
//! \param name The name for the new node.
//! \param parameterRoot A copy of the parameter tree specific for the type of the node.
//!
VTKLayoutNode::VTKLayoutNode ( const QString &name, ParameterGroup *parameterRoot ) :
    Node(name, parameterRoot),
	m_ouputVTKTableParameterName("VTKTableOutput"),
	m_layoutInstance(0),
	m_outputTable(0)
{
	setTypeName("VTKLayoutNode");

    // create the mandatory vtk table output parameter 
	VTKTableParameter * outputVTKTableParameter = new VTKTableParameter(m_ouputVTKTableParameterName);
    outputVTKTableParameter->setPinType(Parameter::PT_Output);
    parameterRoot->addParameter(outputVTKTableParameter);

	INC_INSTANCE_COUNTER
}


//!
//! Destructor of the VTKLayoutNode class.
//!
//! Defined virtual to guarantee that the destructor of a derived class
//! will be called if the instance of the derived class is saved in a
//! variable of its parent class type.
//!
VTKLayoutNode::~VTKLayoutNode ()
{
	emit destroyed();
    DEC_INSTANCE_COUNTER
    Log::info(QString("VTKLayoutNode destroyed."), "VTKLayoutNode::~VTKLayoutNode");
}


vtkTable * VTKLayoutNode::createTableFromGraph(vtkGraph *graph)
{
	if (!graph)
		return 0;

	vtkTable *myTable = vtkTable::New();
	//Create a column named "NodeId"
	vtkIdTypeArray *colNodeId = vtkIdTypeArray::New();
	colNodeId->SetName("NodeId");
	myTable->AddColumn(colNodeId);
	//Create columns named "X", "Y" and "Z"
	vtkDoubleArray *colX = vtkDoubleArray::New();
	colX->SetName("X");
	vtkDoubleArray *colY = vtkDoubleArray::New();
	colY->SetName("Y");
	vtkDoubleArray *colZ = vtkDoubleArray::New();
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
		colNodeId->InsertNextValue( i );
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
	Log::info(QString("Number rows \"%1\" created: ").arg(myTable->GetNumberOfRows()), "VTKLayoutNode::createTableFromGraph");
	Log::info(QString("Number columns \"%1\" created: ").arg(myTable->GetNumberOfColumns()), "VTKLayoutNode::createTableFromGraph");
	return myTable;
}
