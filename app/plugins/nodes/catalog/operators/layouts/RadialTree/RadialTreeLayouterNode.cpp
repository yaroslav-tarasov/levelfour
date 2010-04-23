//!
//! \file "RadialTreeLayouterNode.cpp"
//! \brief Implementation file for RadialTreeLayouterNode class.
//!

#include "RadialTreeLayouterNode.h"
#include "vtkGraphLayout.h"
#include "vtkTreeLayoutStrategy.h"
#include "vtkTable.h"
#include "vtkGraph.h"
#include "vtkTree.h"
#include "vtkVertexListIterator.h"
#include "vtkStringArray.h"
#include "vtkDoubleArray.h"
#include "vtkVariant.h"

#include "VTKGraphParameter.h"
#include "VTKTableParameter.h"
#include "VTKTreeParameter.h"

INIT_INSTANCE_COUNTER(RadialTreeLayouterNode)

//!
//! Constructor of the RadialTreeLayouterNode class.
//!
//! \param name The name for the new node.
//! \param parameterRoot A copy of the parameter tree specific for the type of the node.
//!
RadialTreeLayouterNode::RadialTreeLayouterNode ( const QString &name, ParameterGroup *parameterRoot ) :
    ViewNode(name, parameterRoot),
	m_ouputVTKTableParameterName("VTKTableOutput"),
	m_inputVTKGraphName("VTKGraphInput"),
	m_inputVTKTreeName("VTKTreeInput"),
	m_outputTable(0),
	m_inGraph(0),
	m_inTree(0)
{
	setTypeName("RadialTreeLayouterNode");

    // create the vtk graph input parameter 
	VTKGraphParameter * inputVTKGraphParameter = new VTKGraphParameter(m_inputVTKGraphName);
	inputVTKGraphParameter->setMultiplicity(1);
    inputVTKGraphParameter->setPinType(Parameter::PT_Input);
    inputVTKGraphParameter->setSelfEvaluating(true);
    parameterRoot->addParameter(inputVTKGraphParameter);
	connect(inputVTKGraphParameter, SIGNAL(dirtied()), SLOT(processOutputVTKTable()));

	// create the vtk tree input parameter 
	VTKTreeParameter * inputVTKTreeParameter = new VTKTreeParameter(m_inputVTKTreeName);
	inputVTKTreeParameter->setMultiplicity(1);
    inputVTKTreeParameter->setPinType(Parameter::PT_Input);
    inputVTKTreeParameter->setSelfEvaluating(true);
    parameterRoot->addParameter(inputVTKTreeParameter);
	connect(inputVTKTreeParameter, SIGNAL(dirtied()), SLOT(processOutputVTKTable()));

    // create the vtk table output parameter 
	addOutputParameter(new VTKTableParameter(m_ouputVTKTableParameterName));

	// link the input parameter to the output processing
	Parameter * outputParameter = getParameter(m_ouputVTKTableParameterName);
    if (outputParameter) 
	{
		outputParameter->setProcessingFunction(SLOT(processOutputVTKTable()));
        outputParameter->addAffectingParameter(inputVTKGraphParameter);
		outputParameter->addAffectingParameter(inputVTKTreeParameter);
	}
    INC_INSTANCE_COUNTER
}


//!
//! Destructor of the RadialTreeLayouterNode class.
//!
//! Defined virtual to guarantee that the destructor of a derived class
//! will be called if the instance of the derived class is saved in a
//! variable of its parent class type.
//!
RadialTreeLayouterNode::~RadialTreeLayouterNode ()
{
	emit destroyed();
    DEC_INSTANCE_COUNTER
    Log::info(QString("RadialLayouterNode destroyed."), "RadialLayouterNode::~RadialLayouterNode");
}

//!
//! Processes the node's input data to generate the node's output table.
//!
void RadialTreeLayouterNode::processOutputVTKTable()
{
	if (updateInputGraph() != 0)
		return;
	else if (updateInputTree() != 0)
		return;

	vtkGraphLayout *layout = vtkGraphLayout::New();
	
	// need a condition clause for whether input is graph or tree to set input
	if (updateInputGraph() == 0)
		layout->SetInput( m_inGraph );
	else if (updateInputTree() == 0)
		layout->SetInput( m_inTree);
	
	vtkTreeLayoutStrategy *radialTree = vtkTreeLayoutStrategy::New();

	// params for radial tree
	radialTree->SetRadial(true);
	// these are controlled by property editor
	// set angle
	radialTree->SetAngle(360); // 0 to 360
	// set log spacing value
	radialTree->SetLogSpacingValue(1); // 0 to 1
	// set leaf spacing
	radialTree->SetLeafSpacing(1); // 0 to 1
	// set distance array name
	// radialTree->SetDistanceArrayName(distance);

	layout->SetLayoutStrategy(radialTree);
	layout->Update();

	m_outputTable = createTableFromGraph(layout->GetOutput());

	// process the output vtk table
	VTKTableParameter * outputParameter = dynamic_cast<VTKTableParameter*>(getParameter(m_ouputVTKTableParameterName));

	if (outputParameter) 
		outputParameter->setVTKTable(m_outputTable);
}

vtkTable * RadialTreeLayouterNode::createTableFromGraph(vtkGraph *graph)
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
	Log::info(QString("Number rows \"%1\" created: ").arg(myTable->GetNumberOfRows()), "SingleCycleLayouterNode::createTableFromGraph");
	Log::info(QString("Number columns \"%1\" created: ").arg(myTable->GetNumberOfColumns()), "SingleCycleLayouterNode::createTableFromGraph");
	return myTable;
}

//!
//! Update the input graph
//!
int RadialTreeLayouterNode::updateInputGraph()
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

int RadialTreeLayouterNode::updateInputTree()
{
	// load the input vtk parameter 
	VTKTreeParameter * inputParameter = dynamic_cast<VTKTreeParameter*>(getParameter(m_inputVTKTreeName));
	if (!inputParameter->isConnected())
		return 1;

	// get the source parameter (output of source node) connected to the input parameter
	VTKTreeParameter * sourceParameter = dynamic_cast<VTKTreeParameter*>(inputParameter->getConnectedParameter());

	// get the vtk tree that comes with the source parameter and set it into the input parameter of this node
	m_inTree = sourceParameter->getVTKTree();
	inputParameter->setVTKTree(m_inTree);

	return (m_inTree == 0);
}



