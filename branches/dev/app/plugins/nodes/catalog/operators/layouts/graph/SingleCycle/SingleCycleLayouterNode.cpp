/*
-----------------------------------------------------------------------------
file "SingleCycleLayouterNode.cpp"
brief Implementation file for SingleCycleLayouterNode class.
*/

#include "SingleCycleLayouterNode.h"
#include "vtkCircularLayoutStrategy.h"
#include "VTKTableParameter.h"

INIT_INSTANCE_COUNTER(SingleCycleLayouterNode)
///
/// Constructors and Destructors
///

//!
//! Constructor of the SingleCycleLayouterNode class.
//!
//! \param name The name for the new node.
//! \param parameterRoot A copy of the parameter tree specific for the type of the node.
//!
SingleCycleLayouterNode::SingleCycleLayouterNode ( const QString &name, ParameterGroup *parameterRoot ) :
    VTKGraphLayoutNode(name, parameterRoot)
{
	setTypeName("SingleCycleLayouterNode");

	m_layoutInstance = vtkCircularLayoutStrategy::New();

	setChangeFunction("Angular Radius", SLOT(setAngularRadius()));
    setCommandFunction("Angular Radius", SLOT(setAngularRadius()));

	setChangeFunction("Edge Weight Field", SLOT(setEdgeWeightField()));
    setCommandFunction("Edge Weight Field", SLOT(setEdgeWeightField()));

	INC_INSTANCE_COUNTER
}


//!
//! Destructor of the SingleCycleLayouterNode class.
//!
//! Defined virtual to guarantee that the destructor of a derived class
//! will be called if the instance of the derived class is saved in a
//! variable of its parent class type.
//!
SingleCycleLayouterNode::~SingleCycleLayouterNode ()
{
	emit destroyed();
    DEC_INSTANCE_COUNTER
    Log::info(QString("SingleCycleLayouterNode destroyed."), "SingleCycleLayouterNode::~SingleCycleLayouterNode");
}

//!
//! Set the single layout angular radius property
//!
void SingleCycleLayouterNode::setAngularRadius ()
{
	// not implemented yet
//	m_layoutInstance->SetAngularRadius(m_angularRadius);
}


//!
//! Set the single layout edge weight field property
//!
void SingleCycleLayouterNode::setEdgeWeightField ()
{
	QString newfieldValue = getStringValue("Edge Weight Field");
	if (QString::compare(m_edgeWeightField, newfieldValue) == 0)
		return;

	m_edgeWeightField = newfieldValue;

    if (m_edgeWeightField == "") {
        Log::debug(QString("Edge Weight Field has not been set yet. (\"%1\")").arg(m_name), "SingleCycleLayouterNode::setEdgeWeightField");
        return;
    }

	m_layoutInstance->SetEdgeWeightField(m_edgeWeightField.toLatin1());
	
	processOutputVTKTable();

	// propagate dirtying 
	VTKTableParameter * outputParameter = dynamic_cast<VTKTableParameter*>(getParameter(m_ouputVTKTableParameterName));
	outputParameter->propagateDirty(0);
}

