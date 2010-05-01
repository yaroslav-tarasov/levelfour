/*
-----------------------------------------------------------------------------
file "TableToGraphNode.cpp"
brief Implementation file for TableToGraphNode class.

version    1.0
date       18.05.2009 (last updated)

-----------------------------------------------------------------------------
*/


#include "VTKGraphParameter.h"
#include "VTKTableParameter.h"
#include "VTKTreeParameter.h"
#include "TableToGraphNode.h"
#include "vtkTableToGraph.h"
#include "vtkMutableDirectedGraph.h"
#include "vtkGraph.h"
#include "vtkTree.h"
#include "vtkVertexListIterator.h"

INIT_INSTANCE_COUNTER(TableToGraphNode)

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
	Node(name, parameterRoot),
	m_inputVTKTableParameterName("VTKTableInput"),
	m_outputVTKGraphName("VTKGraphOutput"),
	m_outputVTKTreeName("VTKTreeOutput"),
	m_inputTable(0),
	m_tree(0),
	m_graph(0)
{
    // create the mandatory vtk table input parameter - multiplicity *
	VTKTableParameter * inputVTKTableParameter = new VTKTableParameter(m_inputVTKTableParameterName);
	inputVTKTableParameter->setMultiplicity(1);
    inputVTKTableParameter->setPinType(Parameter::PT_Input);
    inputVTKTableParameter->setSelfEvaluating(true);
    parameterRoot->addParameter(inputVTKTableParameter);

    // create the mandatory vtk graph output parameter 
	VTKGraphParameter * outputVTKGraphParameter = new VTKGraphParameter(m_outputVTKGraphName);
    outputVTKGraphParameter->setPinType(Parameter::PT_Output);
    parameterRoot->addParameter(outputVTKGraphParameter);

	// create the optional vtk graph output parameter
	// A tree is created in cases where the graph is Directed with shallowcopy; see vtkGraph documentation
	VTKTreeParameter * outputVTKTreeParameter = new VTKTreeParameter(m_outputVTKTreeName);
    outputVTKTreeParameter->setPinType(Parameter::PT_Output);
    parameterRoot->addParameter(outputVTKTreeParameter);

	// link the input parameter to the output processing
    if (outputVTKGraphParameter) 
	{
		outputVTKGraphParameter->setProcessingFunction(SLOT(processOutputVTKGraph()));
        outputVTKGraphParameter->addAffectingParameter(inputVTKTableParameter);
	}

	if (outputVTKTreeParameter) 
	{
		outputVTKTreeParameter->setProcessingFunction(SLOT(processOutputVTKTree()));
        outputVTKTreeParameter->addAffectingParameter(inputVTKTableParameter);
	}

    // create the enumeration parameter with the list of columns representing the edgeFrom
	edgesFromParameter = new EnumerationParameter("From", Parameter::getDefaultValue(Parameter::T_Enumeration));
	parameterRoot->addParameter(edgesFromParameter);
	
    // create the enumeration parameter with the list of columns representing the edgeTo
	edgesToParameter = new EnumerationParameter("To", Parameter::getDefaultValue(Parameter::T_Enumeration));
    parameterRoot->addParameter(edgesToParameter);

    // create the enumeration parameter with the list of fields representing the vertexID
	vertexIDParameter = new EnumerationParameter("VertexID", Parameter::getDefaultValue(Parameter::T_Enumeration));
    parameterRoot->addParameter(vertexIDParameter);

    connect(inputVTKTableParameter, SIGNAL(dirtied()), SLOT(processOutputVTKGraph()));
	connect(inputVTKTableParameter, SIGNAL(dirtied()), SLOT(processOutputVTKTree()));
	connect(edgesFromParameter, SIGNAL(dirtied()), this, SLOT(updateGraph()));
	connect(edgesToParameter, SIGNAL(dirtied()), this, SLOT(updateGraph()));
	connect(vertexIDParameter, SIGNAL(dirtied()), this, SLOT(updateGraph()));

	INC_INSTANCE_COUNTER
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
	emit destroyed();
    Log::info(QString("TableToGraphNode destroyed."), "TableToGraphNode::~TableToGraphNode");
    DEC_INSTANCE_COUNTER
}


//!
//! Processes the node's input data to generate the node's output table.
//!
void TableToGraphNode::processOutputVTKGraph ()
{
	if (!updateTable())
		return;

	// recreate the From/To and vertices parameter with the list of the input table attributes
	QStringList literals;
	for (int i = 0; i < m_inputTable->GetNumberOfColumns(); i++)
		literals << m_inputTable->GetColumnName(i);

	edgesFromParameter->setLiterals(literals);
	edgesFromParameter->setValues(literals);

	edgesToParameter->setLiterals(literals);
	edgesToParameter->setValues(literals);

	vertexIDParameter->setLiterals(literals);
	vertexIDParameter->setValues(literals);

	vtkTableToGraph * tableToGraph = vtkTableToGraph::New();
	tableToGraph->SetInput(m_inputTable);
	QString vertexID = vertexIDParameter->getCurrentLiteral();
	QString edgeFrom = edgesFromParameter->getCurrentLiteral();
	QString edgeTo = edgesToParameter->getCurrentLiteral();
	tableToGraph->AddLinkVertex(vertexID.toLatin1());
	tableToGraph->AddLinkEdge(edgeFrom.toLatin1(), edgeTo.toLatin1());

	m_graph = tableToGraph->GetOutput();

	vtkVertexListIterator *vertices = vtkVertexListIterator::New();
	m_graph->GetVertices(vertices);

	// process the output vtk graph 
	VTKGraphParameter * outputParameter = dynamic_cast<VTKGraphParameter*>(getParameter(m_outputVTKGraphName));

	if (outputParameter) 
		outputParameter->setVTKGraph(m_graph);
}

void TableToGraphNode::processOutputVTKTree ()
{
	/* 
	To construct a tree, use vtkMutableDirectedGraph, 
	with directed edges which point from the parent to the child, 
	then use CheckedShallowCopy to set the structure to a vtkTree.

	When the outcome of a conversion is unknown (i.e. setting a graph to a tree), 
	CheckedShallowCopy()  and CheckedDeepCopy()  exist which are identical to 
	ShallowCopy()  and DeepCopy(), except that instead of emitting an error for 
	an incompatible structure, the function returns false. This allows you to 
	programmatically check structure compatibility without causing error messages.

	This is the basic code...
	*/
	m_graph = vtkMutableDirectedGraph::New();
	m_tree->ShallowCopy(m_graph);

}

void TableToGraphNode::updateTree ()
{

}

//!
//! Update the graph (called for example when parameters change)
//!
void TableToGraphNode::updateGraph ()
{
	if (!m_inputTable)
		return;

	vtkTableToGraph * tableToGraph = vtkTableToGraph::New();
	tableToGraph->SetInput(m_inputTable);
	QString vertexID = vertexIDParameter->getCurrentLiteral();
	QString edgeFrom = edgesFromParameter->getCurrentLiteral();
	QString edgeTo = edgesToParameter->getCurrentLiteral();
	tableToGraph->AddLinkVertex(vertexID.toLatin1());
	tableToGraph->AddLinkEdge(edgeFrom.toLatin1(), edgeTo.toLatin1());

	m_graph = tableToGraph->GetOutput();

	vtkVertexListIterator *vertices = vtkVertexListIterator::New();
	m_graph->GetVertices(vertices);

	// process the output vtk graph 
	VTKGraphParameter * outputParameter = dynamic_cast<VTKGraphParameter*>(getParameter(m_outputVTKGraphName));

	if (outputParameter) 
	{
		outputParameter->setVTKGraph(m_graph);
		outputParameter->propagateDirty(0);
	}
}

//!
//! Update the input table 
//!
bool TableToGraphNode::updateTable ()
{
	// load the input vtk parameter 
	VTKTableParameter * inputParameter = dynamic_cast<VTKTableParameter*>(getParameter(m_inputVTKTableParameterName));
	if (!inputParameter->isConnected())
		return false;

	// get the source parameter (output of source node) connected to the input parameter
	VTKTableParameter * sourceParameter = dynamic_cast<VTKTableParameter*>(inputParameter->getConnectedParameter());

	// get the vtk table that comes with the source parameter and set it into the input parameter of this node
	m_inputTable = sourceParameter->getVTKTable();
	inputParameter->setVTKTable(m_inputTable);

	return (m_inputTable != 0);
}
