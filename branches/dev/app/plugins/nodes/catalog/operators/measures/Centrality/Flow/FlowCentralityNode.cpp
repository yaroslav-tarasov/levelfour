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
file "FlowCentralityNode.cpp"
brief Implementation file for FlowCentralityNode class.

version    1.0
date       18.05.2009 (last updated)

Description
-----------

The size of the influence of a node 'A' on the flow between two nodes 
is defined as 1 - [the maximum flow between two nodes when 'A' is removed 
from the network / the maximum flow between two nodes]. 

The flow betweenness centrality of 'A' is the sum of the values defined 
previously for all pair of nodes while none of them is 'A'.

The (“raw,” or unnormalized) flow betweenness of a vertex, v 2 V (G), is defined by Freeman et al.

it may be desirable to normalize the raw flow betweenness by the total maximum flow
among third parties (including v); this leads to the following normalized flow betweenness measure:

it may be noted that the above normalization (from Freeman et al. (1991)) is rather different
from that used in the definition of shortest-path betweenness, which normalizes within (rather than
across) third-party dyads.

Parameters
----------
    G : graph
      A graph 

	diag : boolean 
		indicating whether or not the diagonal should be treated as valid data.
		Set this true if and only if the data can contain loops. diag is FALSE by default.

	tmaxdev : boolean 
		indicating whether or not the theoretical maximum absolute deviation
		from the maximum nodal centrality should be returned. By default, tmaxdev==FALSE.

	rescale :
		if true, centrality scores are rescaled such that they sum to 1.

	ignore.eval : logical; 
		ignore edge values when computing maximum flow (alternately, edge
		values will be assumed to carry capacity information)

	mode :
	   One of rawflow, normflow, or fracflow

Returns
-------
nodes : dictionary
       Dictionary of nodes with flow centrality as the value.

	   A vector, matrix, or list containing the flow scores (depending on the number and size of the input
		graphs).

Notes
-----



Reference
---------
Freeman, L.C.; Borgatti, S.P.; and White, D.R. (1991). “Centrality in Valued Graphs: A Measure
of Betweenness Based on Network Flow.” Social Networks, 13(2), 141-154.

Koschutzki, D.; Lehmann, K.A.; Peeters, L.; Richter, S.; Tenfelde-Podehl, D.; Zlotowski, O. (2005).
“Centrality Indices.” In U. Brandes and T. Erlebach (eds.), Network Analysis: Methodological
Foundations. Berlin: Springer.

-----------------------------------------------------------------------------
*/

#include "FlowCentralityNode.h"


///
/// Constructors and Destructors
///


//!
//! Constructor of the FlowCentralityNode class.
//!
//! \param name The name for the new node.
//! \param parameterRoot A copy of the parameter tree specific for the type of the node.
//!
FlowCentralityNode::FlowCentralityNode ( const QString &name, ParameterGroup *parameterRoot ) :
    Node(name, parameterRoot)
{
}


//!
//! Destructor of the FlowCentralityNode class.
//!
//! Defined virtual to guarantee that the destructor of a derived class
//! will be called if the instance of the derived class is saved in a
//! variable of its parent class type.
//!
FlowCentralityNode::~FlowCentralityNode ()
{
}


