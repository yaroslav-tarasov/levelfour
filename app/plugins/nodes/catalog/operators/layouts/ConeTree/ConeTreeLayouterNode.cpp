//!
//! \file "ConeTreeLayouterNode.cpp"
//! \brief Implementation file for ConeTreeLayouterNode class.
//!

#include "ConeTreeLayouterNode.h"
#include "vtkGraphLayout.h"
#include "vtkConeLayoutStrategy.h"
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

INIT_INSTANCE_COUNTER(ConeTreeLayouterNode)

//!
//! Constructor of the ConeTreeLayouterNode class.
//!
//! \param name The name for the new node.
//! \param parameterRoot A copy of the parameter tree specific for the type of the node.
//!
ConeTreeLayouterNode::ConeTreeLayouterNode ( const QString &name, ParameterGroup *parameterRoot ) :
    Node(name, parameterRoot),
	m_ouputVTKTableParameterName("VTKTableOutput"),
	m_inputVTKTreeName("VTKTreeInput"),
	m_outputTable(0),
	m_inTree(0)
{
	setTypeName("ConeTreeLayouterNode");

	// create the vtk tree input parameter 
	VTKTreeParameter * inputVTKTreeParameter = new VTKTreeParameter(m_inputVTKTreeName);
	inputVTKTreeParameter->setMultiplicity(1);
    inputVTKTreeParameter->setPinType(Parameter::PT_Input);
    inputVTKTreeParameter->setSelfEvaluating(true);
    parameterRoot->addParameter(inputVTKTreeParameter);
	connect(inputVTKTreeParameter, SIGNAL(dirtied()), SLOT(processOutputVTKTable()));

    // create the vtk table output parameter 
	VTKTableParameter * outputVTKTableParameter = new VTKTableParameter(m_ouputVTKTableParameterName);
    outputVTKTableParameter->setPinType(Parameter::PT_Output);
    parameterRoot->addParameter(outputVTKTableParameter);

	/*
	// set affections and callback functions
	// Set Compactness
	setChangeFunction("SetCompactness", SLOT(compactnessChanged()));
    setCommandFunction("SetCompactness", SLOT(compactnessChanged()));
	// ConeTree->SetCompactness(float);

	// Set Compression
	setChangeFunction("SetCompression", SLOT(compressionChanged()));
    setCommandFunction("SetCompression", SLOT(compressionChanged()));
	// ConeTree->SetCompression(int);

	// Set Spacing
	setChangeFunction("SetSpacing", SLOT(spacingChanged()));
    setCommandFunction("SetSpacing", SLOT(spacingChanged()));
	// ConeTree->SetSpacing(float);

	*/

	// link the input parameter to the output processing
    if (outputVTKTableParameter) 
	{
		outputVTKTableParameter->setProcessingFunction(SLOT(processOutputVTKTable()));
        // outputVTKTableParameter->addAffectingParameter(inputVTKGraphParameter);
		outputVTKTableParameter->addAffectingParameter(inputVTKTreeParameter);
	}

    INC_INSTANCE_COUNTER
}


//!
//! Destructor of the ConeTreeLayouterNode class.
//!
//! Defined virtual to guarantee that the destructor of a derived class
//! will be called if the instance of the derived class is saved in a
//! variable of its parent class type.
//!
ConeTreeLayouterNode::~ConeTreeLayouterNode ()
{
	emit destroyed();
    DEC_INSTANCE_COUNTER
    Log::info(QString("RadialLayouterNode destroyed."), "RadialLayouterNode::~RadialLayouterNode");
}

//!
//! Processes the node's input data to generate the node's output table.
//!
void ConeTreeLayouterNode::processOutputVTKTable()
{
	if (updateInputTree() != 0)
		return;

	vtkGraphLayout *layout = vtkGraphLayout::New();
	
	// this layout should only take tree as input
	layout->SetInput( m_inTree);
	
	vtkConeLayoutStrategy *ConeTree = vtkConeLayoutStrategy::New();
	layout->SetLayoutStrategy(ConeTree);
	layout->Update();

	m_outputTable = createTableFromGraph(layout->GetOutput());

	// process the output vtk table
	VTKTableParameter * outputParameter = dynamic_cast<VTKTableParameter*>(getParameter(m_ouputVTKTableParameterName));

	if (outputParameter) 
		outputParameter->setVTKTable(m_outputTable);
}

vtkTable * ConeTreeLayouterNode::createTableFromGraph(vtkGraph *graph)
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
//! Update the input tree
//!

int ConeTreeLayouterNode::updateInputTree()
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



