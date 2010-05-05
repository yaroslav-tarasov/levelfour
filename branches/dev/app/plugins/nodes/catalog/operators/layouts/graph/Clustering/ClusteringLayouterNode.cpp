/*
-----------------------------------------------------------------------------
file "ClusteringLayouterNode.cpp"
brief Implementation file for ClusteringLayouterNode class.
*/

#include "ClusteringLayouterNode.h"
#include "VTKTableParameter.h"

INIT_INSTANCE_COUNTER(ClusteringLayouterNode)
///
/// Constructors and Destructors
///

//!
//! Constructor of the ClusteringLayouterNode class.
//!
//! \param name The name for the new node.
//! \param parameterRoot A copy of the parameter tree specific for the type of the node.
//!
ClusteringLayouterNode::ClusteringLayouterNode ( const QString &name, ParameterGroup *parameterRoot ) :
    VTKGraphLayoutNode(name, parameterRoot)
{
	setTypeName("ClusteringLayouterNode");

	m_layoutInstance = vtkClustering2DLayoutStrategy::New();

	setChangeFunction("Set Random Seed", SLOT(processParameters()));
    setCommandFunction("Set Random Seed", SLOT(processParameters()));

	setChangeFunction("Set Max Number Of Iterations", SLOT(processParameters()));
    setCommandFunction("Set Max Number Of Iterations", SLOT(processParameters()));

	setChangeFunction("Set Iterations Per Layout", SLOT(processParameters()));
    setCommandFunction("Set Iterations Per Layout", SLOT(processParameters()));

	setChangeFunction("Set Initial Temperature", SLOT(processParameters()));
    setCommandFunction("Set Initial Temperature", SLOT(processParameters()));

	setChangeFunction("Set Cool Down Rate", SLOT(processParameters()));
    setCommandFunction("Set Cool Down Rate", SLOT(processParameters()));

	setChangeFunction("Set Rest Distance", SLOT(processParameters()));
    setCommandFunction("Set Rest Distance", SLOT(processParameters()));

	INC_INSTANCE_COUNTER
}


//!
//! Destructor of the ClusteringLayouterNode class.
//!
//! Defined virtual to guarantee that the destructor of a derived class
//! will be called if the instance of the derived class is saved in a
//! variable of its parent class type.
//!
ClusteringLayouterNode::~ClusteringLayouterNode ()
{
	emit destroyed();
    DEC_INSTANCE_COUNTER
    Log::info(QString("ClusteringLayouterNode destroyed."), "ClusteringLayouterNode::~ClusteringLayouterNode");
}

void ClusteringLayouterNode::processParameters()
{
	if (!updateInputGraph())
		return;
	
	setRandomSeed();
	setMaxNumberOfIterations();
	setIterationsPerLayout();
	setInitialTemperature();
	setCoolDownRate();
	setRestDistance();

	refreshOutput();
}

//!
//! Set the layout properties
//!
void ClusteringLayouterNode::setRandomSeed ()
{
	int randomSeed = getIntValue("Set Random Seed");
	if (randomSeed == m_randomSeed)
		return;

	m_randomSeed = randomSeed;

	M_LAYOUT->SetRandomSeed(m_randomSeed);
}

void ClusteringLayouterNode::setMaxNumberOfIterations ()
{
	int maxIterations = getIntValue("Set Max Number Of Iterations");
	if (m_maxIterations == maxIterations)
		return;

	m_maxIterations = maxIterations;

	M_LAYOUT->SetMaxNumberOfIterations(m_maxIterations);
}

void ClusteringLayouterNode::setIterationsPerLayout ()
{
	int layoutIterations = getIntValue("Set Iterations Per Layout");
	if (m_layoutIterations == layoutIterations)
		return;

	m_layoutIterations = layoutIterations;

	M_LAYOUT->SetIterationsPerLayout(m_layoutIterations);
}

void ClusteringLayouterNode::setInitialTemperature ()
{
	int initialTemperature = getDoubleValue("Set Initial Temperature");
	if (m_initialTemperature == initialTemperature)
		return;

	m_initialTemperature = initialTemperature;

	M_LAYOUT->SetInitialTemperature(m_initialTemperature);
}

void ClusteringLayouterNode::setCoolDownRate ()
{
	int coolDownRate = getIntValue("Set Cool Down Rate");
	if (m_coolDownRate == coolDownRate)
		return;

	m_coolDownRate = coolDownRate;

	M_LAYOUT->SetCoolDownRate(m_coolDownRate);
}

void ClusteringLayouterNode::setRestDistance ()
{
	int restDistance = getDoubleValue("Set Rest Distance");
	if (m_restDistance == restDistance)
		return;

	m_restDistance = restDistance;

	M_LAYOUT->SetRestDistance(m_restDistance);
}

