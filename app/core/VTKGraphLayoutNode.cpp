/*
-----------------------------------------------------------------------------
file "VTKGraphLayoutNode.cpp"
brief Implementation file for VTKGraphLayoutNode class.

version    1.0
date       21.04.2010 (last updated)

*/

#include "VTKGraphLayoutNode.h"
#include "vtkGraphLayout.h"
#include "vtkCircularLayoutStrategy.h"
#include "vtkTable.h"
#include "vtkGraph.h"
#include "vtkVertexListIterator.h"
#include "vtkStringArray.h"
#include "vtkDoubleArray.h"
#include "vtkVariant.h"

#include "VTKGraphParameter.h"
#include "VTKTableParameter.h"

INIT_INSTANCE_COUNTER(VTKGraphLayoutNode)
///
/// Constructors and Destructors
///

//!
//! Constructor of the VTKGraphLayoutNode class.
//!
//! \param name The name for the new node.
//! \param parameterRoot A copy of the parameter tree specific for the type of the node.
//!
VTKGraphLayoutNode::VTKGraphLayoutNode ( const QString &name, ParameterGroup *parameterRoot ) :
    Node(name, parameterRoot),
	m_ouputVTKTableParameterName("VTKTableOutput"),
	m_inputVTKGraphName("VTKGraphInput"),
	m_layoutInstance(0),
	m_outputTable(0),
	m_inGraph(0)
{
	setTypeName("VTKGraphLayoutNode");

    // create the mandatory vtk graph input parameter 
	VTKGraphParameter * inputVTKGraphParameter = new VTKGraphParameter(m_inputVTKGraphName);
	inputVTKGraphParameter->setMultiplicity(1);
    inputVTKGraphParameter->setPinType(Parameter::PT_Input);
    inputVTKGraphParameter->setSelfEvaluating(true);
    parameterRoot->addParameter(inputVTKGraphParameter);
	connect(inputVTKGraphParameter, SIGNAL(dirtied()), SLOT(processParameters()));

    // create the mandatory vtk table output parameter 
	VTKTableParameter * outputVTKTableParameter = new VTKTableParameter(m_ouputVTKTableParameterName);
    outputVTKTableParameter->setPinType(Parameter::PT_Output);
    parameterRoot->addParameter(outputVTKTableParameter);

	INC_INSTANCE_COUNTER
}


//!
//! Destructor of the VTKGraphLayoutNode class.
//!
//! Defined virtual to guarantee that the destructor of a derived class
//! will be called if the instance of the derived class is saved in a
//! variable of its parent class type.
//!
VTKGraphLayoutNode::~VTKGraphLayoutNode ()
{
	emit destroyed();
    DEC_INSTANCE_COUNTER
    Log::info(QString("VTKGraphLayoutNode destroyed."), "VTKGraphLayoutNode::~VTKGraphLayoutNode");
}

//!
//! Processes the node's input data to generate the node's output table.
//!
void VTKGraphLayoutNode::refreshOutput()
{
	if (!m_inGraph)
	{
	    Log::info(QString("No graph defined."), "VTKGraphLayoutNode::~refreshOutput");
		return;
	}

	vtkGraphLayout *layout = vtkGraphLayout::New();
	layout->SetInput( m_inGraph );

	if (!m_layoutInstance)
	{
	    Log::info(QString("No layout strategy defined."), "VTKGraphLayoutNode::~refreshOutput");
		return;
	}

	layout->SetLayoutStrategy(m_layoutInstance);
	layout->Update();

	m_outputTable = createTableFromGraph(layout->GetOutput());

	// process the output vtk table
	VTKTableParameter * outputParameter = dynamic_cast<VTKTableParameter*>(getParameter(m_ouputVTKTableParameterName));

	if (outputParameter && outputParameter->getVTKTable() != m_outputTable) 
	{
		outputParameter->setVTKTable(m_outputTable);
		outputParameter->propagateDirty(0);
	}
}

vtkTable * VTKGraphLayoutNode::createTableFromGraph(vtkGraph *graph)
{
	vtkTable *myTable = vtkTable::New();
	//Create a column named "NodeId"
	vtkStringArray *colNodeId = vtkStringArray::New();
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
	Log::info(QString("Number rows \"%1\" created: ").arg(myTable->GetNumberOfRows()), "VTKGraphLayoutNode::createTableFromGraph");
	Log::info(QString("Number columns \"%1\" created: ").arg(myTable->GetNumberOfColumns()), "VTKGraphLayoutNode::createTableFromGraph");
	return myTable;
}

//!
//! Update the input graph
//!
bool VTKGraphLayoutNode::updateInputGraph()
{
	// load the input vtk parameter 
	VTKGraphParameter * inputParameter = dynamic_cast<VTKGraphParameter*>(getParameter(m_inputVTKGraphName));
	if (!inputParameter->isConnected())
		return false;

	// get the source parameter (output of source node) connected to the input parameter
	VTKGraphParameter * sourceParameter = dynamic_cast<VTKGraphParameter*>(inputParameter->getConnectedParameter());

	// get the vtk graph that comes with the source parameter and set it into the input parameter of this node
	m_inGraph = sourceParameter->getVTKGraph();
	inputParameter->setVTKGraph(m_inGraph);

	return (m_inGraph != 0);
}
