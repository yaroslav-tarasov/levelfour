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
file "DegreeCentralityNode.cpp"
brief Implementation file for DegreeCentralityNode class.

version    1.0
date       18.05.2009 (last updated)

Description
-----------

Total, immediate, and mediative effect centralities. 

Parameters
----------
    G : graph
      A graph 

    modes:
      	IMMEDIATE: Immediate effect centrality is the measure how immediate one’s effect to others. Similar to closeness centrality.

		MEDIATIVE: Mediative effect centrality is the measure the degree of mediation. Similar to betweenness centrality.

		TOTAL: A node's total effect centrality is the measure for the effect strength from the given node to other nodes 
		through every walks between them.  Similar to Status and Power Centrality.
      
Returns
-------
nodes : dictionary
 Dictionary of nodes with effects centrality as the value.

 A vector, matrix, or list containing the immediate, mediative, and total effects scores (depending on the number and size of the input
  graphs).
    
Notes
-----


Reference
---------
 Noah E. Friedkin, 1991. Theoretical Foundations for Centrality Measures. AJS 96 Number 6, 1478-1504 


-----------------------------------------------------------------------------
*/

#include "EffectsCentralityNode.h"


///
/// Constructors and Destructors
///


//!
//! Constructor of the EffectsCentralityNode class.
//!
//! \param name The name for the new node.
//! \param parameterRoot A copy of the parameter tree specific for the type of the node.
//!
EffectsCentralityNode::EffectsCentralityNode ( const QString &name, ParameterGroup *parameterRoot ) :
    Node(name, parameterRoot)
{
}


//!
//! Destructor of the EffectsCentralityNode class.
//!
//! Defined virtual to guarantee that the destructor of a derived class
//! will be called if the instance of the derived class is saved in a
//! variable of its parent class type.
//!
EffectsCentralityNode::~EffectsCentralityNode ()
{
}


