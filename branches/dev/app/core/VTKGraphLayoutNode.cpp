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
#include "vtkIdTypeArray.h"
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
    VTKLayoutNode(name, parameterRoot),
	m_inputVTKGraphName("VTKGraphInput"),
	m_zRangeParameterName("ZRange"),
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

    // create the mandatory SetZRange parameter 
	NumberParameter * zRangeParameter = new NumberParameter(m_zRangeParameterName, Parameter::T_Float, QVariant::fromValue<double>(0));
    parameterRoot->addParameter(zRangeParameter);
	connect(zRangeParameter, SIGNAL(dirtied()), SLOT(processParameters()));

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
	int zRange = getDoubleValue(m_zRangeParameterName);
	layout->SetZRange(zRange);
	layout->SetInput( m_inGraph );

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
