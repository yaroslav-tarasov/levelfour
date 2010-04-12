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
file "EigenvectorCentralityNode.cpp"
brief Implementation file for EigenvectorCentralityNode class.

version    1.0
date       18.05.2009 (last updated)

Description
-----------

    Uses the power method to find the eigenvector for the 
    largest eigenvalue of the adjacency matrix of G.

	Eigenvector Centrality, as defined by Bonacich (1972), 
	of a node is (recursively) proportional to the sum of 
	eigenvector centralities of the nodes to which it is connected. 
	It is calculated by computing principal eigenvector 
	(which has the biggest eigenvalue among every eigenvectors.) 
	of input 1-mode Network.

Parameters
----------
    G : graph
      A graph 

    max_iter : interger, optional
      Maximum number of iterations in power method.

    tol : float, optional
      Error tolerance used to check convergence in power method iteration.

    nstart : dictionary, optional
      Starting value of eigenvector iteration for each node. 
      
Returns
-------
nodes : dictionary
       Dictionary of nodes with eigenvector centrality as the value.

	   A vector, matrix, or list containing the eigenvector scores (depending on the number and size of the input
		graphs).
    
Notes
-----
    The eigenvector calculation is done by the power iteration method
    and has no guarantee of convergence.  The iteration will stop
    after max_iter iterations or an error tolerance of
    number_of_nodes(G)*tol has been reached.

    For directed graphs this is "right" eigevector centrality.  For
    "left" eigenvector centrality, first reverse the graph with
    G.reverse().


Reference
---------
Bonacich P (1972). Factoring and Weighting Approaches to status scores and clique identification. Journal of Mathematical Sociology 2, 113-120.
Tony Tam. Demarcating the boundaries between self and the social: The anatomy of centrality in social networks. Social Networks Volume 11, Issue 4, Pages 315-403 (December 1989). Pages 387-401
Mizruchi, M.S., P. Mariolis, M. Schwartz and B. Mintz. “Techniques for disaggregating centrality scores in social networks.” In Tuma, N.B. (ed.), Sociological Methodology 1986, pp. 26-48. Washington, DC: American Sociological Association.

-----------------------------------------------------------------------------
*/

#include "EigenvectorCentralityNode.h"


///
/// Constructors and Destructors
///


//!
//! Constructor of the EigenvectorCentralityNode class.
//!
//! \param name The name for the new node.
//! \param parameterRoot A copy of the parameter tree specific for the type of the node.
//!
EigenvectorCentralityNode::EigenvectorCentralityNode ( const QString &name, ParameterGroup *parameterRoot ) :
    Node(name, parameterRoot)
{
}


//!
//! Destructor of the EigenvectorCentralityNode class.
//!
//! Defined virtual to guarantee that the destructor of a derived class
//! will be called if the instance of the derived class is saved in a
//! variable of its parent class type.
//!
EigenvectorCentralityNode::~EigenvectorCentralityNode ()
{
}


