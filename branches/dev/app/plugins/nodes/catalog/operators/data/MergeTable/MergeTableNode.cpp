/*
file "MergeTableNode.cpp"
brief Implementation file for MergeTableNode class.

version    1.0
date       20.04.2010 (last updated)
*/


#include "MergeTableNode.h"
#include "VTKTableParameter.h"

#include <vtkMergeTables.h>
#include <vtkTable.h>

///
/// Constructors and Destructors
///

//!
//! Constructor of the MergeTableNode class.
//!
//! \param name The name for the new node.
//! \param parameterRoot A copy of the parameter tree specific for the type of the node.
//!
MergeTableNode::MergeTableNode ( const QString &name, ParameterGroup *parameterRoot ) :
	VTKTableNode(name, parameterRoot, "VTKTable"),
	m_inputVTKTableParameterName("VTKTableInput")
{
    // create the mandatory vtk table input parameter - multiplicity ONE OR MORE
	VTKTableParameter * inputVTKTableParameter = new VTKTableParameter(m_inputVTKTableParameterName);
	inputVTKTableParameter->setMultiplicity(Parameter::M_OneOrMore);
    inputVTKTableParameter->setPinType(Parameter::PT_Input);
    inputVTKTableParameter->setSelfEvaluating(true);
    parameterRoot->addParameter(inputVTKTableParameter);
    connect(inputVTKTableParameter, SIGNAL(dirtied()), SLOT(processOutputVTKTable()));

	// link the output parameter to the output processing
	Parameter * outputParameter = getParameter(m_outputVTKTableName);
    if (outputParameter) 
	{
		outputParameter->setProcessingFunction(SLOT(processOutputVTKTable()));
        outputParameter->addAffectingParameter(inputVTKTableParameter);
	}
}


//!
//! Destructor of the MergeTableNode class.
//!
//! Defined virtual to guarantee that the destructor of a derived class
//! will be called if the instance of the derived class is saved in a
//! variable of its parent class type.
//!
MergeTableNode::~MergeTableNode ()
{
}

//!
//! Processes the node's input data to generate the node's output table.
//!
void MergeTableNode::processOutputVTKTable ()
{
	// load the input vtk parameter 
	VTKTableParameter * inputParameter = dynamic_cast<VTKTableParameter*>(getParameter(m_inputVTKTableParameterName));
	if (!inputParameter->isConnected())
		return;

	// get the parameters connected to the input parameter
	QList<Connection *> connections = inputParameter->getConnectionMap().values();

	// merge each vtkTable for each source parameter
	vtkTable * mergeTable = vtkTable::New();
	vtkMergeTables * merge;
	for (int i = 0; i < connections.size(); ++i) {
        VTKTableParameter * sourceParameter = dynamic_cast<VTKTableParameter*>(connections.at(i)->getSourceParameter());
		if (sourceParameter && sourceParameter->getVTKTable())
		{
            // merge list of tables found in input parameters
			merge = vtkMergeTables::New();
			merge->SetInput(0, mergeTable);
			merge->SetInput(1, sourceParameter->getVTKTable());
			merge->MergeColumnsByNameOn();
			merge->PrefixAllButMergedOn();
			merge->Update();

			mergeTable = merge->GetOutput();
		}
    }

	// set the output vtk parameter exactly the same of the input one
	VTKTableParameter * outputParameter = dynamic_cast<VTKTableParameter*>(getParameter(m_outputVTKTableName));

	if (outputParameter)
		outputParameter->setVTKTable(mergeTable);
	
	m_table = outputParameter->getVTKTable();
}

