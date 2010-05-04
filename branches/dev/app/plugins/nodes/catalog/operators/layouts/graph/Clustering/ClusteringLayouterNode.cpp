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

//!
//! Set the layout properties
//!
void ClusteringLayouterNode::setRandomSeed ()
{
	M_LAYOUT->SetRandomSeed(m_randomSeed);
}

void ClusteringLayouterNode::setMaxNumberOfIterations ()
{
	M_LAYOUT->SetMaxNumberOfIterations(m_maxIterations);
}

void ClusteringLayouterNode::setIterationsPerLayout ()
{
	M_LAYOUT->SetIterationsPerLayout(m_layoutIterations);
}

void ClusteringLayouterNode::setInitialTemperature ()
{
	M_LAYOUT->SetInitialTemperature(m_initialTemperature);
}

void ClusteringLayouterNode::setCoolDownRate ()
{
	M_LAYOUT->SetCoolDownRate(m_coolDownRate);
}

void ClusteringLayouterNode::setRestDistance ()
{
	M_LAYOUT->SetRestDistance(m_restDistance);
}

