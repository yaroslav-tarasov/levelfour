/*
-----------------------------------------------------------------------------
file "ConeTreeLayouterNode.cpp"
brief Implementation file for ConeTreeLayouterNode class.
*/

#include "ConeTreeLayouterNode.h"
#include "VTKTableParameter.h"

INIT_INSTANCE_COUNTER(ConeTreeLayouterNode)

//!
//! Constructor of the ConeTreeLayouterNode class.
//!
//! \param name The name for the new node.
//! \param parameterRoot A copy of the parameter tree specific for the type of the node.
//!
ConeTreeLayouterNode::ConeTreeLayouterNode ( const QString &name, ParameterGroup *parameterRoot ) :
    VTKTreeLayoutNode(name, parameterRoot)
{
	setTypeName("ConeTreeLayouterNode");

	m_layoutInstance = vtkConeLayoutStrategy::New();

	setChangeFunction("Set Compactness", SLOT(processParameters()));
    setCommandFunction("Set Compactness", SLOT(processParameters()));

	setChangeFunction("Set Compression", SLOT(processParameters()));
    setCommandFunction("Set Compression", SLOT(processParameters()));

	setChangeFunction("Set Spacing", SLOT(processParameters()));
    setCommandFunction("Set Spacing", SLOT(processParameters()));

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
    Log::info(QString("ConeTreeLayouterNode destroyed."), "ConeTreeLayouterNode::~ConeTreeLayouterNode");
}

void ConeTreeLayouterNode::processParameters()
{
	if (!updateInputTree())
		return;
	
	refreshOutput();
}
//!
//! Set the various parameters from property editor
//!
void ConeTreeLayouterNode::setCompactness ()
{
	double compactness = getDoubleValue(m_CompactnessNameParameter);
	if (m_Compactness == compactness)
		return;

	m_Compactness = compactness;

	M_LAYOUT->SetCompactness(m_Compactness);
}

void ConeTreeLayouterNode::setCompression ()
{
	double compression = getIntValue(m_CompressionNameParameter);
	if (m_Compression == compression)
		return;

	m_Compression = compression;

	M_LAYOUT->SetCompression(m_Compression);
}

void ConeTreeLayouterNode::setSpacing ()
{
	double spacing = getDoubleValue(m_SpacingNameParameter);
	if (m_Spacing == spacing)
		return;

	m_Spacing = spacing;

	M_LAYOUT->SetSpacing(m_Spacing);
}

