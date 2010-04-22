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
file "DataTableNode.cpp"
brief Implementation file for DataTableNode class.

version    1.0
date       18.05.2009 (last updated)

Description
-----------


Parameters
----------
      
Returns
-------

    
Notes
-----

Reference
---------

-----------------------------------------------------------------------------
*/


#include "DataTableNode.h"
#include "VTKTableParameter.h"
#include "vtkTable.h"

INIT_INSTANCE_COUNTER(DataTableNode)

///
/// Constructors and Destructors
///

//!
//! Constructor of the DataTableNode class.
//!
//! \param name The name for the new node.
//! \param parameterRoot A copy of the parameter tree specific for the type of the node.
//!
DataTableNode::DataTableNode ( const QString &name, ParameterGroup *parameterRoot ) :
	VTKTableNode(name, parameterRoot, "VTKTable"),
	m_inputVTKTableParameterName("VTKTableInput")
{
    // create the mandatory vtk table input parameter - multiplicity *
	VTKTableParameter * inputVTKTableParameter = new VTKTableParameter(m_inputVTKTableParameterName);
	inputVTKTableParameter->setMultiplicity(1);
    inputVTKTableParameter->setPinType(Parameter::PT_Input);
    inputVTKTableParameter->setSelfEvaluating(true);
    parameterRoot->addParameter(inputVTKTableParameter);
    connect(inputVTKTableParameter, SIGNAL(dirtied()), SLOT(processOutputVTKTable()));

	// link the input parameter to the output processing
	Parameter * outputParameter = getParameter(m_outputVTKTableName);
    if (outputParameter) 
	{
		outputParameter->setProcessingFunction(SLOT(processOutputVTKTable()));

        outputParameter->addAffectingParameter(inputVTKTableParameter);
	}
    INC_INSTANCE_COUNTER
}


//!
//! Destructor of the DataTableNode class.
//!
//! Defined virtual to guarantee that the destructor of a derived class
//! will be called if the instance of the derived class is saved in a
//! variable of its parent class type.
//!
DataTableNode::~DataTableNode ()
{
	emit destroyed();
    Log::info(QString("DataTableNode destroyed."), "DataTableNode::~DataTableNode");
    DEC_INSTANCE_COUNTER
}


//!
//! Processes the node's input data to generate the node's output table.
//!
void DataTableNode::processOutputVTKTable ()
{
	// load the input vtk parameter 
	VTKTableParameter * inputParameter = dynamic_cast<VTKTableParameter*>(getParameter(m_inputVTKTableParameterName));
	if (!inputParameter->isConnected())
		return;

	// get the source parameter (output of source node) connected to the input parameter
	VTKTableParameter * sourceParameter = dynamic_cast<VTKTableParameter*>(inputParameter->getConnectedParameter());

	// get the vtk table that comes with the source parameter and set it into the input parameter of this node
	inputParameter->setVTKTable(sourceParameter->getVTKTable());	

	// set the output vtk parameter exactly the same of the input one
	VTKTableParameter * outputParameter = dynamic_cast<VTKTableParameter*>(getParameter(m_outputVTKTableName));

	if (outputParameter)
		outputParameter->setVTKTable(inputParameter->getVTKTable());
	
	m_table = outputParameter->getVTKTable();

	emit tableChanged(m_table);

	if (m_table)
	{
		Log::info(QString("Number rows \"%1\" created: ").arg(m_table->GetNumberOfRows()), "DataTableNode::processOutputVTKTable");
		Log::info(QString("Number columns \"%1\" created: ").arg(m_table->GetNumberOfColumns()), "DataTableNode::processOutputVTKTable");
	}
}