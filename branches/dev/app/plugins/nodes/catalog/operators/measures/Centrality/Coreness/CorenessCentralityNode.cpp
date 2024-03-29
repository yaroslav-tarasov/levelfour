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

/*
-----------------------------------------------------------------------------
file "CorenessCentralityNode.cpp"
brief Implementation file for CorenessCentralityNode class.

version    1.0
date       18.05.2009 (last updated)

Description
-----------
 Find the k-cores of a graph.
 The k-core is found by recursively pruning nodes with degrees less than k.

Parameters
----------
    G : graph
      A graph 

Returns
-------
  core_number : dictionary 
       A dictionary keyed by node to the core number. 
    
Reference
---------
  [1] An O(m) Algorithm for Cores Decomposition of Networks
       Vladimir Batagelj and Matjaz Zaversnik,  2003
       http://arxiv.org/abs/cs.DS/0310049 

-----------------------------------------------------------------------------
*/

#include "CorenessCentralityNode.h"


///
/// Constructors and Destructors
///


//!
//! Constructor of the CorenessCentralityNode class.
//!
//! \param name The name for the new node.
//! \param parameterRoot A copy of the parameter tree specific for the type of the node.
//!
CorenessCentralityNode::CorenessCentralityNode ( const QString &name, ParameterGroup *parameterRoot ) :
    Node(name, parameterRoot)
{
}


//!
//! Destructor of the CorenessCentralityNode class.
//!
//! Defined virtual to guarantee that the destructor of a derived class
//! will be called if the instance of the derived class is saved in a
//! variable of its parent class type.
//!
CorenessCentralityNode::~CorenessCentralityNode ()
{
}


