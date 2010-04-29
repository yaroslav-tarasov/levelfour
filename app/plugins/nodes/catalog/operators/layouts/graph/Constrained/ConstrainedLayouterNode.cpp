/*
-----------------------------------------------------------------------------
file "ConstrainedLayouterNode.cpp"
brief Implementation file for ConstrainedLayouterNode class.
*/

#include "ConstrainedLayouterNode.h"
#include "vtkConstrained2DLayoutStrategy.h"
#include "VTKTableParameter.h"

INIT_INSTANCE_COUNTER(ConstrainedLayouterNode)

//!
//! Constructor of the ConstrainedLayouterNode class.
//!
//! \param name The name for the new node.
//! \param parameterRoot A copy of the parameter tree specific for the type of the node.
//!
ConstrainedLayouterNode::ConstrainedLayouterNode ( const QString &name, ParameterGroup *parameterRoot ) :
    VTKGraphLayoutNode(name, parameterRoot)
{
	setTypeName("ConstrainedLayouterNode");

	m_layoutInstance = vtkConstrained2DLayoutStrategy::New();

	// These properties relate to graph layout
	// Constrained layout is a 2d layout, therefore Z range is given as an option
	setChangeFunction("Set Z Range", SLOT(setZRange()));
    setCommandFunction("Set Z Range", SLOT(setZRange()));

	// These properties relate specifically to the layout strategy
	setChangeFunction("Set Input Array Name", SLOT(setInputArrayName()));
    setCommandFunction("Set Input Array Name", SLOT(setInputArrayName()));

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
//! Destructor of the ConstrainedLayouterNode class.
//!
//! Defined virtual to guarantee that the destructor of a derived class
//! will be called if the instance of the derived class is saved in a
//! variable of its parent class type.
//!
ConstrainedLayouterNode::~ConstrainedLayouterNode ()
{
	emit destroyed();
    DEC_INSTANCE_COUNTER
    Log::info(QString("ConstrainedLayouterNode destroyed."), "ConstrainedLayouterNode::~ConstrainedLayouterNode");
}

//!
//! Set the layout properties
//!
void ConstrainedLayouterNode::setZRange ()
{
	m_layoutInstance->SetZRange(m_zRange);
}

void ConstrainedLayouterNode::setRandomSeed ()
{
	m_layoutInstance->SetRandomSeed(m_randomSeed);
}

void ConstrainedLayouterNode::setMaxNumberOfIterations ()
{
	m_layoutInstance->SetMaxNumberOfIterations(m_maxIterations);
}

void ConstrainedLayouterNode::setIterationsPerLayout ()
{
	m_layoutInstance->SetIterationsPerLayout(m_layoutIterations);
}

void ConstrainedLayouterNode::setInitialTemperature ()
{
	m_layoutInstance->SetInitialTemperature(m_initialTemperature);
}

void ConstrainedLayouterNode::setCoolDownRate ()
{
	m_layoutInstance->SetCoolDownRate(m_coolDownRate);
}

void ConstrainedLayouterNode::setRestDistance ()
{
	m_layoutInstance->SetRestDistance(m_restDistance);
}

void ConstrainedLayouterNode::setInputArrayName ()
{
	QString newfieldValue = getStringValue("Set Input Array Name");
	if (QString::compare(m_inputArrayName, newfieldValue) == 0)
		return;

	m_inputArrayName = newfieldValue;

    if (m_inputArrayName == "") {
        Log::debug(QString("Input Array Name has not been set yet. (\"%1\")").arg(m_name), "ConstrainedLayouterNode::setInputArrayName");
        return;
    }

	m_layoutInstance->SetInputArrayName(m_inputArrayName.toLatin1());
	
	processOutputVTKTable();

	// propagate dirtying 
	VTKTableParameter * outputParameter = dynamic_cast<VTKTableParameter*>(getParameter(m_ouputVTKTableParameterName));
	outputParameter->propagateDirty(0);
}