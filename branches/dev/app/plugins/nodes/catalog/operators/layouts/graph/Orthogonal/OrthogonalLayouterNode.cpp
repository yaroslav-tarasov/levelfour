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
//! \file "OrthogonalLayouterNode.cpp"
//! \brief Implementation file for OrthogonalLayouterNode class.
//!
//! \author     Stefan Habel <stefan.habel@filmakademie.de>
//! \version    1.0
//! \date       18.05.2009 (last updated)
//!

#include "OrthogonalLayouterNode.h"


///
/// Constructors and Destructors
///


//!
//! Constructor of the OrthogonalLayouterNode class.
//!
//! \param name The name for the new node.
//! \param parameterRoot A copy of the parameter tree specific for the type of the node.
//!
OrthogonalLayouterNode::OrthogonalLayouterNode ( const QString &name, ParameterGroup *parameterRoot ) :
    Node(name, parameterRoot)
{
}


//!
//! Destructor of the OrthogonalLayouterNode class.
//!
//! Defined virtual to guarantee that the destructor of a derived class
//! will be called if the instance of the derived class is saved in a
//! variable of its parent class type.
//!
OrthogonalLayouterNode::~OrthogonalLayouterNode ()
{
}


