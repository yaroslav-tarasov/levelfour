/*
-----------------------------------------------------------------------------
file "ThreeDCirclesLayouterNode.cpp"
brief Implementation file for ThreeDCirclesLayouterNode class.
*/

#include "ThreeDCirclesLayouterNode.h"
#include "vtkSimple3DCirclesStrategy.h"
#include "VTKTableParameter.h"

INIT_INSTANCE_COUNTER(ThreeDCirclesLayouterNode)

//!
//! Constructor of the ThreeDCirclesLayouterNode class.
//!
//! \param name The name for the new node.
//! \param parameterRoot A copy of the parameter tree specific for the type of the node.
//!
ThreeDCirclesLayouterNode::ThreeDCirclesLayouterNode ( const QString &name, ParameterGroup *parameterRoot ) :
    VTKGraphLayoutNode(name, parameterRoot)
{
	setTypeName("ThreeDCirclesLayouterNode");

	m_layoutInstance = vtkSimple3DCirclesStrategy::New();

	// These properties relate to graph layout
	// none for this layout

	// These properties relate specifically to the layout strategy
	setChangeFunction("Set Method", SLOT(setMethod()));
    setCommandFunction("Set Method", SLOT(setMethod()));

	setChangeFunction("Set Radius", SLOT(setRadius()));
    setCommandFunction("Set Radius", SLOT(setRadius()));

	setChangeFunction("Set Height", SLOT(setHeight()));
    setCommandFunction("Set Height", SLOT(setHeight()));

	setChangeFunction("Set Marked Start Vertices", SLOT(setMarkedStartVertices()));
    setCommandFunction("Set Marked Start Vertices", SLOT(setMarkedStartVertices()));

	setChangeFunction("Set Marked Value", SLOT(setMarkedValue()));
    setCommandFunction("Set Marked Value", SLOT(setMarkedValue()));

	setChangeFunction("Set Force To Use Universal Start Points Finder", SLOT(setForceToUseUniversalStartPointsFinder()));
    setCommandFunction("Set Force To Use Universal Start Points Finder", SLOT(setForceToUseUniversalStartPointsFinder()));

	setChangeFunction("Set Auto Height", SLOT(setAutoHeight()));
    setCommandFunction("Set Auto Height", SLOT(setAutoHeight()));

	setChangeFunction("Set Minimum Radian", SLOT(setMinimumRadian()));
    setCommandFunction("Set Minimum Radian", SLOT(setMinimumRadian()));

	setChangeFunction("Set Minimum Degree", SLOT(setMinimumDegree()));
    setCommandFunction("Set Minimum Degree", SLOT(setMinimumDegree()));

	setChangeFunction("Set Hierarchical Layers", SLOT(setHierarchicalLayers()));
    setCommandFunction("Set Hierarchical Layers", SLOT(setHierarchicalLayers()));

	INC_INSTANCE_COUNTER
}


//!
//! Destructor of the ThreeDCirclesLayouterNode class.
//!
//! Defined virtual to guarantee that the destructor of a derived class
//! will be called if the instance of the derived class is saved in a
//! variable of its parent class type.
//!
ThreeDCirclesLayouterNode::~ThreeDCirclesLayouterNode ()
{
	emit destroyed();
    DEC_INSTANCE_COUNTER
    Log::info(QString("ThreeDCirclesLayouterNode destroyed."), "ThreeDCirclesLayouterNode::~ThreeDCirclesLayouterNode");
}

//!
//! Set the layout properties
//!
void ThreeDCirclesLayouterNode::setMethod ()
{
	m_layoutInstance->SetMethod(m_setMethod);
}

void ThreeDCirclesLayouterNode::setRadius ()
{
	m_layoutInstance->SetRadius(m_setRadius);
}

void ThreeDCirclesLayouterNode::setHeight ()
{
	m_layoutInstance->SetHeight(m_setHeight);
}

void ThreeDCirclesLayouterNode::setMarkedStartVertices ()
{
	m_layoutInstance->SetMarkedStartVertices(m_setMarkedStartVertices);
}

void ThreeDCirclesLayouterNode::setMarkedValue ()
{
	m_layoutInstance->SetMarkedValue(m_setMarkedValue);
}

void ThreeDCirclesLayouterNode::setForceToUseUniversalStartPointsFinder ()
{
	m_layoutInstance->SetForceToUseUniversalStartPointsFinder(m_setForceToUseUniversalStartPointsFinder);
}

void ThreeDCirclesLayouterNode::setAutoHeight ()
{
	m_layoutInstance->SetAutoHeight(m_setAutoHeight);
}

void ThreeDCirclesLayouterNode::setMinimumRadian ()
{
	m_layoutInstance->SetMinimumRadian(m_setMinimumRadian);
}

void ThreeDCirclesLayouterNode::setMinimumDegree ()
{
	m_layoutInstance->SetMinimumDegree(m_setMinimumDegree);
}

void ThreeDCirclesLayouterNode::setHierarchicalLayers ()
{
	m_layoutInstance->SetHierarchicalLayers(m_setHierarchicalLayers);

}