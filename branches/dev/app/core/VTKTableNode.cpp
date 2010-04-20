#include "vtkTable.h"
#include "ParameterGroup.h"
#include "NumberParameter.h"
#include "SceneNodeParameter.h"
#include "FilenameParameter.h"
#include "VTKTableNode.h"
#include "VTKTableParameter.h"

///
/// Constructors and Destructors
///


//!
//! Constructor of the VTKTableNode class.
//!
//! \param name The name to give the new mesh node.
//! \param parameterRoot A copy of the parameter tree specific for the type of the node.
//!
VTKTableNode::VTKTableNode ( const QString &name, ParameterGroup *parameterRoot, const QString &outputVTKTableName /* = "VTKTable" */ ) :
    ViewNode (name, parameterRoot),
	m_outputVTKTableName(outputVTKTableName),
	m_table(0)
{
	setTypeName("VTKTableNode");
	// add output table
    addOutputParameter(new VTKTableParameter(m_outputVTKTableName));
    INC_INSTANCE_COUNTER
}


//!
//! Destructor of the TextDelimiterSourceNode class.
//!
VTKTableNode::~VTKTableNode ()
{
	if (m_table)
		m_table->Delete();
	emit destroyed();
    Log::info(QString("VTKTableNode destroyed."), "VTKTableNode::~VTKTableNode");
    DEC_INSTANCE_COUNTER
}

///
/// Private Functions
///

vtkTable * VTKTableNode::getTable() const
{
	return m_table;
}