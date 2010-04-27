/*
-----------------------------------------------------------------------------
This source file is part of FRAPPER
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

//!
//! \file "MultiplierNode.h"
//! \brief Implementation file for MultiplierNode class.
//!
//! \author     Nils Zweiling <nils.zweiling@filmakademie.de>
//! \author     Stefan Habel <stefan.habel@filmakademie.de>
//! \version    1.0
//! \date       29.04.2009 (last updated)
//!

#include "MultiplierNode.h"
#include <QtCore/QFile>
#include <QtCore/QDir>
#include <iostream>


///
/// Constructors and Destructors
///


//!
//! Constructor of the MultiplierNode class.
//!
//! \param name The name for the new node.
//! \param parameterRoot A copy of the parameter tree specific for the type of the node.
//!
MultiplierNode::MultiplierNode ( QString name, ParameterGroup *parameterRoot ) :
    Node(name, parameterRoot)
{
    // set affections and callback functions
    addAffection("input1", "output1");
    addAffection("input2", "output2");
    addAffection("input3", "output3");
    addAffection("input4", "output4");
    setProcessingFunction("output1", SLOT(processOutputParameter()));
    setProcessingFunction("output2", SLOT(processOutputParameter()));
    setProcessingFunction("output3", SLOT(processOutputParameter()));
    setProcessingFunction("output4", SLOT(processOutputParameter()));

}


//!
//! Destructor of the MultiplierNode class.
//!
//! Defined virtual to guarantee that the destructor of a derived class
//! will be called if the instance of the derived class is saved in a
//! variable of its parent class type.
//!
MultiplierNode::~MultiplierNode ()
{
}


///
/// Public Methods
///


//!
//! Processes the output parameter depending on its name .
//!
void MultiplierNode::processOutputParameter ()
{
    Parameter *parameter = dynamic_cast<Parameter *>(sender());
    if (!parameter)
        return;

    const QString &parameterName = parameter->getName();
    
 

    if (parameterName == "output1") {
		setValue("output1", getDoubleValue("input1") * getDoubleValue("mult1"));
	}
	else if (parameterName == "output2") {
		setValue("output2", getDoubleValue("input2") * getDoubleValue("mult2"));
	}
	else if (parameterName == "output3") {
		setValue("output3", getDoubleValue("input3") * getDoubleValue("mult3"));
	}
	else if (parameterName == "output4") {
		setValue("output4", getDoubleValue("input4") * getDoubleValue("mult4"));
	}
}

