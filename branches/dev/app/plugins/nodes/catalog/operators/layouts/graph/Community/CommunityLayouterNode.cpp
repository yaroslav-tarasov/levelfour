/*
-----------------------------------------------------------------------------
file "CommunityLayouterNode.cpp"
brief Implementation file for CommunityLayouterNode class.
*/

#include "CommunityLayouterNode.h"
#include "vtkCommunity2DLayoutStrategy.h"
#include "VTKTableParameter.h"

INIT_INSTANCE_COUNTER(CommunityLayouterNode)

//!
//! Constructor of the CommunityLayouterNode class.
//!
//! \param name The name for the new node.
//! \param parameterRoot A copy of the parameter tree specific for the type of the node.
//!
CommunityLayouterNode::CommunityLayouterNode ( const QString &name, ParameterGroup *parameterRoot ) :
    VTKGraphLayoutNode(name, parameterRoot)
{
	setTypeName("CommunityLayouterNode");

	m_layoutInstance = vtkCommunity2DLayoutStrategy::New();

	// These properties relate to graph layout
	// Community layout is a 2d layout, therefore Z range is given as an option
	setChangeFunction("Set Z Range", SLOT(setZRange()));
    setCommandFunction("Set Z Range", SLOT(setZRange()));

	// These properties relate specifically to the layout strategy
	setChangeFunction("Set Community Array Name", SLOT(setCommunityArrayName()));
    setCommandFunction("Set Community Array Name", SLOT(setCommunityArrayName()));

	setChangeFunction("Set Community Strength", SLOT(setCommunityStrength()));
    setCommandFunction("Set Community Strength", SLOT(setCommunityStrength()));

	setChangeFunction("Set Random Seed", SLOT(setRandomSeed()));
    setCommandFunction("Set Random Seed", SLOT(setRandomSeed()));

	setChangeFunction("Set Max Number Of Iterations", SLOT(setMaxNumberOfIterations()));
    setCommandFunction("Set Max Number Of Iterations", SLOT(setMaxNumberOfIterations()));

	setChangeFunction("Set Iterations Per Layout", SLOT(setIterationsPerLayout()));
    setCommandFunction("Set Iterations Per Layout", SLOT(setIterationsPerLayout()));

	setChangeFunction("Set Initial Temperature", SLOT(setInitialTemperature()));
    setCommandFunction("Set Initial Temperature", SLOT(setInitialTemperature()));

	setChangeFunction("Set Cool Down Rate", SLOT(setCoolDownRate()));
    setCommandFunction("Set Cool Down Rate", SLOT(setCoolDownRate()));

	setChangeFunction("Set Rest Distance", SLOT(setRestDistance()));
    setCommandFunction("Set Rest Distance", SLOT(setRestDistance()));

	INC_INSTANCE_COUNTER
}


//!
//! Destructor of the CommunityLayouterNode class.
//!
//! Defined virtual to guarantee that the destructor of a derived class
//! will be called if the instance of the derived class is saved in a
//! variable of its parent class type.
//!
CommunityLayouterNode::~CommunityLayouterNode ()
{
	emit destroyed();
    DEC_INSTANCE_COUNTER
    Log::info(QString("CommunityLayouterNode destroyed."), "CommunityLayouterNode::~CommunityLayouterNode");
}

//!
//! Set the layout properties
//!
void CommunityLayouterNode::setZRange ()
{
	m_layoutInstance->SetZRange(m_zRange);
}

void CommunityLayouterNode::setRandomSeed ()
{
	m_layoutInstance->SetRandomSeed(m_randomSeed);
}

void CommunityLayouterNode::setMaxNumberOfIterations ()
{
	m_layoutInstance->SetMaxNumberOfIterations(m_maxIterations);
}

void CommunityLayouterNode::setIterationsPerLayout ()
{
	m_layoutInstance->SetIterationsPerLayout(m_layoutIterations);
}

void CommunityLayouterNode::setInitialTemperature ()
{
	m_layoutInstance->SetInitialTemperature(m_initialTemperature);
}

void CommunityLayouterNode::setCoolDownRate ()
{
	m_layoutInstance->SetCoolDownRate(m_coolDownRate);
}

void CommunityLayouterNode::setRestDistance ()
{
	m_layoutInstance->SetRestDistance(m_restDistance);
}

void CommunityLayouterNode::setCommunityStrength ()
{
	m_layoutInstance->SetCommunityStrength(m_communityStrength);
}

void CommunityLayouterNode::setCommunityArrayName ()
{
	QString newfieldValue = getStringValue("Set Community Array Name");
	if (QString::compare(m_communityArrayName, newfieldValue) == 0)
		return;

	m_communityArrayName = newfieldValue;

    if (m_communityArrayName == "") {
        Log::debug(QString("Community Array Name has not been set yet. (\"%1\")").arg(m_name), "CommunityLayouterNode::setCommunityArrayName");
        return;
    }

	m_layoutInstance->SetCommunityArrayName(m_communityArrayName.toLatin1());
	
	processOutputVTKTable();

	// propagate dirtying 
	VTKTableParameter * outputParameter = dynamic_cast<VTKTableParameter*>(getParameter(m_ouputVTKTableParameterName));
	outputParameter->propagateDirty(0);
}