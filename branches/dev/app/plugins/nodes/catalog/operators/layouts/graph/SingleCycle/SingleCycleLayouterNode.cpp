/*
-----------------------------------------------------------------------------
This source file is part of Acuity4D
research.animationsinstitut.de
sourceforge.net/projects/frapper

Copyright (c) 2008-2009 Filmakademie Baden-Wuerttemberg, Institute of Animation 

This program is free software; you can redistribute it and/or modify it under
the terms of the GNU Lesser General Public License as published by the Free Software
Foundation; version 2.1 of the License.

This program is distributed in the hope that it will be useful, but WITHOUT
ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
FOR A PARTICULAR PURPOSE. See the GNU Lesser General Public License for more details.

You should have received a copy of the GNU Lesser General Public License along with
this program; if not, write to the Free Software Foundation, Inc., 59 Temple
Place - Suite 330, Boston, MA 02111-1307, USA, or go to
http://www.gnu.org/licenses/old-licenses/lgpl-2.1.html
-----------------------------------------------------------------------------
*/

/*
-----------------------------------------------------------------------------
file "SingleCycleLayouterNode.cpp"
brief Implementation file for SingleCycleLayouterNode class.

version    1.0
date       21.04.2010 (last updated)

Description
-----------

Circular layout is the most prominent and oldest conventions used to draw graphs. Circular graph layout is a drawing scheme 
where all nodes are placed on the perimeter of a circle. In such a layout, the edges connecting these nodes are passing 
within the circle.

This node is based on the vtkCircularLayoutStrategy implementation.

Inputs
----------
      
	In: vtkGraph

Outputs
-------

	Out: vtkTable (used for data model)

Result
------

A vtkTable is produced with 4 columns: nodeId, posX, posY, posZ.

Properties
-------

	AngularRadius: max angular radius of circle (0-360)
	EdgeWeightField:

Notes
-----

Layouters are based on a generic pipeline of: 
	a.) vtkTable->vtkTableToGraph->vtkGraph->vtkGraphLayout->vtkTable

Given this pipeline, vtkCircularLayoutStrategy is set to the vtkGraphLayout.

***AngularRadius is not currently implemented in vtkCircularLayoutStrategy.

Reference
---------

M. Baur and U. Brandes, �Crossing Reduction in Circular
Layouts�, Proc. Graph-Theoretic Concepts in Computer-
Science (WG �04), 2004, pp.332-343.

Dogrusoz, Ugur and Madden, Brendan and Madden, Patrick,
Circular layout in the Graph Layout Toolkit�, 
Proc. Graph Drawing (GD �96), 1996, pp.92�100.

-----------------------------------------------------------------------------
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
