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

    The degree centrality for a node v is the fraction of nodes it
    is connected to.
	
	The in-degree centrality for a node v is the fraction of nodes its 
    incoming edges are connected to.

	The out-degree centrality for a node v is the fraction of nodes its 
    outgoing edges are connected to.


Parameters
----------
    G : graph
      A graph 

    modes:
      	OUT: out-degree centrality for a node v is the fraction of nodes its outgoing edges are connected to.

		IN: in-degree centrality for a node v is the fraction of nodes its incoming edges are conne

		ALL: The degree centrality for a node v is the fraction of nodes it is connected to. 
      
Returns
-------
nodes : dictionary
 Dictionary of nodes with degree centrality as the value.

 A vector, matrix, or list containing the degree scores (depending on the number and size of the input
  graphs).
    
Notes
-----
 The degree centrality is normalized to the maximum possible degree in the graph G. That is, G.degree(v)/(G.order()-1).

 The degree centrality values are normalized by dividing by the maximum 
 possible degree in a simple graph n-1 where n is the number of nodes in G.

 For multigraphs or graphs with self loops the maximum degree might
 be higher than n-1 and values of degree centrality greater than 1
 are possible.

Reference
---------
 Freeman, L.C. (1979). “Centrality in Social Networks I: Conceptual Clarification.” Social Networks, 1, 215-239. 


-----------------------------------------------------------------------------
*/

#include "DegreeCentralityNode.h"


///
/// Constructors and Destructors
///


//!
//! Constructor of the DegreeCentralityNode class.
//!
//! \param name The name for the new node.
//! \param parameterRoot A copy of the parameter tree specific for the type of the node.
//!
DegreeCentralityNode::DegreeCentralityNode ( const QString &name, ParameterGroup *parameterRoot ) :
    Node(name, parameterRoot)
{
}


//!
//! Destructor of the DegreeCentralityNode class.
//!
//! Defined virtual to guarantee that the destructor of a derived class
//! will be called if the instance of the derived class is saved in a
//! variable of its parent class type.
//!
DegreeCentralityNode::~DegreeCentralityNode ()
{
}


