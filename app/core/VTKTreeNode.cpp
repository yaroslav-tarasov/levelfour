#include "vtkTree.h"
#include "ParameterGroup.h"
#include "NumberParameter.h"
#include "SceneNodeParameter.h"
#include "FilenameParameter.h"
#include "VTKTreeNode.h"
#include "VTKTreeParameter.h"

///
/// Constructors and Destructors
///


//!
//! Constructor of the VTKTreeNode class.
//!
//! \param name The name to give the new mesh node.
//! \param parameterRoot A copy of the parameter tree specific for the type of the node.
//!
VTKTreeNode::VTKTreeNode ( const QString &name, ParameterGroup *parameterRoot, const QString &outputVTKTreeName /* = "VTKTree" */ ) :
    ViewNode (name, parameterRoot),
	m_outputVTKTreeName(outputVTKTreeName),
	m_Tree(0)
{
	setTypeName("VTKTreeNode");
	// add output Tree
    addOutputParameter(new VTKTreeParameter(m_outputVTKTreeName));
    INC_INSTANCE_COUNTER
}


//!
//! Destructor of the VTKTreeNode class.
//!
VTKTreeNode::~VTKTreeNode ()
{
	if (m_Tree)
		m_Tree->Delete();
	emit destroyed();
    Log::info(QString("VTKTreeNode destroyed."), "VTKTreeNode::~VTKTreeNode");
    DEC_INSTANCE_COUNTER
}

///
/// Private Functions
///

vtkTree * VTKTreeNode::getTree() const
{
	return m_Tree;
}