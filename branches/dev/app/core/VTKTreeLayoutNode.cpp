/*
-----------------------------------------------------------------------------
file "VTKTreeLayoutNode.cpp"
brief Implementation file for VTKTreeLayoutNode class.

version    1.0
date       21.04.2010 (last updated)

This is a base class for layouts which are based on
tree structures (vtkTree).

*/

#include "VTKTreeLayoutNode.h"
#include "vtkGraphLayout.h"
#include "vtkTable.h"
#include "vtkGraph.h"
#include "vtkTree.h"
#include "vtkVertexListIterator.h"
#include "vtkStringArray.h"
#include "vtkDoubleArray.h"
#include "vtkVariant.h"
#include "VTKTableParameter.h"
#include "VTKTreeParameter.h"

INIT_INSTANCE_COUNTER(VTKTreeLayoutNode)

//!
//! Constructor of the VTKTreeLayoutNode class.
//!
//! \param name The name for the new node.
//! \param parameterRoot A copy of the parameter tree specific for the type of the node.
//!
VTKTreeLayoutNode::VTKTreeLayoutNode ( const QString &name, ParameterGroup *parameterRoot ) :
    VTKLayoutNode(name, parameterRoot),
	m_inputVTKTreeName("VTKTreeInput"),
	m_inputVTKTree(0)
{
	setTypeName("VTKTreeLayoutNode");

	// create the vtk tree input parameter 
	VTKTreeParameter * inputVTKTreeParameter = new VTKTreeParameter(m_inputVTKTreeName);
	inputVTKTreeParameter->setMultiplicity(1);
    inputVTKTreeParameter->setPinType(Parameter::PT_Input);
    inputVTKTreeParameter->setSelfEvaluating(true);
    parameterRoot->addParameter(inputVTKTreeParameter);
	connect(inputVTKTreeParameter, SIGNAL(dirtied()), SLOT(processParameters()));

	INC_INSTANCE_COUNTER
}


//!
//! Destructor of the VTKTreeLayoutNode class.
//!
//! Defined virtual to guarantee that the destructor of a derived class
//! will be called if the instance of the derived class is saved in a
//! variable of its parent class type.
//!
VTKTreeLayoutNode::~VTKTreeLayoutNode ()
{
	emit destroyed();
    DEC_INSTANCE_COUNTER
    Log::info(QString("VTKTreeLayoutNode destroyed."), "VTKTreeLayoutNode::~VTKTreeLayoutNode");
}

//!
//! Processes the node's input data to generate the node's output table.
//!
void VTKTreeLayoutNode::refreshOutput()
{
	if (!m_inputVTKTree)
	{
	    Log::info(QString("No tree defined."), "VTKTreeLayoutNode::~refreshOutput");
		return;
	}

	vtkGraphLayout *layout = vtkGraphLayout::New();
	layout->SetInput( m_inputVTKTree );

	if (!m_layoutInstance)
	{
	    Log::info(QString("No layout strategy defined."), "VTKGraphLayoutNode::~refreshOutput");
		return;
	}

	layout->SetLayoutStrategy(m_layoutInstance);
	layout->Update();

	m_outputTable = createTableFromGraph(layout->GetOutput(0));

	// process the output vtk table
	VTKTableParameter * outputParameter = dynamic_cast<VTKTableParameter*>(getParameter(m_ouputVTKTableParameterName));

	if (outputParameter && outputParameter->getVTKTable() != m_outputTable) 
	{
		outputParameter->setVTKTable(m_outputTable);
		outputParameter->propagateDirty(0);
	}
}

//!
//! Update the input graph
//!
int VTKTreeLayoutNode::updateInputTree()
{
	// load the input vtk parameter 
	VTKTreeParameter * inputParameter = dynamic_cast<VTKTreeParameter*>(getParameter(m_inputVTKTreeName));
	if (!inputParameter->isConnected())
		return false;

	// get the source parameter (output of source node) connected to the input parameter
	VTKTreeParameter * sourceParameter = dynamic_cast<VTKTreeParameter*>(inputParameter->getConnectedParameter());

	// get the vtk graph that comes with the source parameter and set it into the input parameter of this node
	m_inputVTKTree = sourceParameter->getVTKGraph();
	inputParameter->setVTKGraph(m_inputVTKTree);

	return (m_inputVTKTree != 0);
}
