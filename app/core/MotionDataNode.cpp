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
//! \file "MotionDataNode.cpp"
//! \brief Implementation file for MotionDataNode class.
//!
//! \author     Simon Spielmann <sspielma@filmakademie.de>
//! \author     Stefan Habel <stefan.habel@filmakademie.de>
//! \version    1.0
//! \date       03.07.2009 (last updated)
//!

#include "MotionDataNode.h"
Q_DECLARE_METATYPE(ParameterGroup *)

///
/// Constructors and Destructors
///


//!
//! Constructor of the MotionDataNode class.
//!
//! \param name The name for the new node.
//!
MotionDataNode::MotionDataNode ( const QString &name, ParameterGroup *parameterRoot ) :
    CurveEditorDataNode(name, parameterRoot),
    m_maxNumberParameters(0),
    m_maxNumberKeys(0),
    m_databaseGroup(new ParameterGroup("DataBase")),
    m_infoGroup(0),
    m_dataGroup(0)
{
    Parameter *outputDataParameter = Parameter::createGroupParameter("Out_DataBase", m_databaseGroup);
    outputDataParameter->setPinType(Parameter::PT_Output);
    outputDataParameter->setDirty(true);
    parameterRoot->addParameter(outputDataParameter);
	parameterRoot->addParameter(m_databaseGroup);
}


//!
//! Destructor of the MotionDataNode class.
//!
//! Defined virtual to guarantee that the destructor of a derived class
//! will be called if the instance of the derived class is saved in a
//! variable of its parent class type.
//!
MotionDataNode::~MotionDataNode ()
{
    // the m_databaseGroup does not have to be destroyed here, as it is contained
    // in the node's parameter tree, which is destroyed in Node::~Node
}


///
/// Public Functions
///


//!
//! Returns the maximum number of parameters in the motion data set.
//!
//! \return The maximum number of parameters in the motion data set.
//!
inline unsigned int MotionDataNode::getMaxNumberParameters () const
{
    return m_maxNumberParameters;
}

//!
//! The maximum number of parameters contained in the motion data set.
//!
unsigned int MotionDataNode::getMaxNumberKeys () const
{
    return m_maxNumberKeys;
}


///
/// Protected Functions
///


//!
//! Clears all data from the motion data database.
//!
void MotionDataNode::clearDatabase ()
{
	ParameterGroup *rootGroup = getParameterRoot();

	if (m_dataGroup && m_databaseGroup->containsGroup(m_dataGroup->getName())) {
		m_databaseGroup->removeParameterGroup(m_dataGroup->getName());
		m_dataGroup = 0;
	}

	if (m_infoGroup && m_databaseGroup->containsGroup(m_infoGroup->getName())) {
		m_databaseGroup->removeParameterGroup(m_infoGroup->getName());
		m_infoGroup = 0;
	}

    // create the database parameter group as a child of the parameter root
    m_dataGroup = new ParameterGroup("Data");
    m_infoGroup = new ParameterGroup("Info");
 
	m_databaseGroup->addParameter(m_dataGroup);
	m_databaseGroup->addParameter(m_infoGroup);

	setCurveEditorGoup(m_dataGroup);

    notifyChange();
}