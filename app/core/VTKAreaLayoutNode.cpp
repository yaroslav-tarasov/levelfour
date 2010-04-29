/*
-----------------------------------------------------------------------------
file "VTKAreaLayoutNode.cpp"
brief Implementation file for VTKAreaLayoutNode class.

version    1.0
date       21.04.2010 (last updated)

This is a base class for layouts which are based on
tree structures (vtkTree) AND have space-filling properties.

*/

#include "VTKAreaLayoutNode.h"
#include "vtkAreaLayout.h"
#include "vtkTable.h"
#include "vtkTree.h"
#include "vtkStringArray.h"
#include "vtkDoubleArray.h"
#include "vtkVariant.h"
#include "VTKTableParameter.h"
#include "VTKTreeParameter.h"

INIT_INSTANCE_COUNTER(VTKAreaLayoutNode)

//!
//! Constructor of the VTKAreaLayoutNode class.
//!
//! \param name The name for the new node.
//! \param parameterRoot A copy of the parameter tree specific for the type of the node.
//!
VTKAreaLayoutNode::VTKAreaLayoutNode ( const QString &name, ParameterGroup *parameterRoot ) :
    Node(name, parameterRoot),
	m_ouputVTKTableParameterName("VTKTableOutput"),
	m_inputVTKTreeName("VTKTreeInput"),
	m_layoutInstance(0),
	m_outputTable(0),
	m_inputVTKTree(0)
{
	setTypeName("VTKAreaLayoutNode");

	// create the vtk tree input parameter 
	VTKTreeParameter * inputVTKTreeParameter = new VTKTreeParameter(m_inputVTKTreeName);
	inputVTKTreeParameter->setMultiplicity(1);
    inputVTKTreeParameter->setPinType(Parameter::PT_Input);
    inputVTKTreeParameter->setSelfEvaluating(true);
    parameterRoot->addParameter(inputVTKTreeParameter);
	connect(inputVTKTreeParameter, SIGNAL(dirtied()), SLOT(processOutputVTKTable()));

    // create the mandatory vtk table output parameter 
	VTKTableParameter * outputVTKTableParameter = new VTKTableParameter(m_ouputVTKTableParameterName);
    outputVTKTableParameter->setPinType(Parameter::PT_Output);
    parameterRoot->addParameter(outputVTKTableParameter);

	// link the input parameter to the output processing
    if (outputVTKTableParameter) 
	{
		outputVTKTableParameter->setProcessingFunction(SLOT(processOutputVTKTable()));
        outputVTKTableParameter->addAffectingParameter(inputVTKTreeParameter);
	}
    INC_INSTANCE_COUNTER
}


//!
//! Destructor of the VTKAreaLayoutNode class.
//!
//! Defined virtual to guarantee that the destructor of a derived class
//! will be called if the instance of the derived class is saved in a
//! variable of its parent class type.
//!
VTKAreaLayoutNode::~VTKAreaLayoutNode ()
{
	emit destroyed();
    DEC_INSTANCE_COUNTER
    Log::info(QString("VTKAreaLayoutNode destroyed."), "VTKAreaLayoutNode::~VTKAreaLayoutNode");
}

//!
//! Processes the node's input data to generate the node's output table.
//!
void VTKAreaLayoutNode::processOutputVTKTable()
{
	if (updateInputTree() != 0)
		return;

	vtkAreaLayout *layout = vtkAreaLayout::New();
	layout->SetInput( m_inputVTKTree );

	if (!m_layoutInstance)
	{
	    Log::info(QString("No layout strategy defined."), "VTKAreaLayoutNode::~processOutputVTKTable");
		return;
	}

	layout->SetLayoutStrategy(m_layoutInstance);
	layout->Update();

	m_outputTable = createTableFromTree(layout->GetOutput());

	// process the output vtk table
	VTKTableParameter * outputParameter = dynamic_cast<VTKTableParameter*>(getParameter(m_ouputVTKTableParameterName));

	if (outputParameter) 
		outputParameter->setVTKTable(m_outputTable);
}

vtkTable * VTKAreaLayoutNode::createTableFromTree(vtkTree *tree)
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

	//Fill the table with data from the tree

	Log::info(QString("Number rows \"%1\" created: ").arg(myTable->GetNumberOfRows()), "VTKAreaLayoutNode::createTableFromGraph");
	Log::info(QString("Number columns \"%1\" created: ").arg(myTable->GetNumberOfColumns()), "VTKAreaLayoutNode::createTableFromGraph");
	return myTable;
}

//!
//! Update the input graph
//!
int VTKAreaLayoutNode::updateInputTree()
{
	// load the input vtk parameter 
	VTKTreeParameter * inputParameter = dynamic_cast<VTKTreeParameter*>(getParameter(m_inputVTKTreeName));
	if (!inputParameter->isConnected())
		return 1;

	// get the source parameter (output of source node) connected to the input parameter
	VTKTreeParameter * sourceParameter = dynamic_cast<VTKTreeParameter*>(inputParameter->getConnectedParameter());

	// get the vtk graph that comes with the source parameter and set it into the input parameter of this node
	m_inputVTKTree = sourceParameter->getVTKTree();
	inputParameter->setVTKTree(m_inputVTKTree);

	return (m_inputVTKTree == 0);
}
