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

    The HITS algorithm computes two numbers for a node. 
    Authorities estimates the node value based on the incoming links.
    Hubs estimates the node value based on outgoing links.

	Implements Kleinberg's hub and authority scores.

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
    (hubs,authorities) : two-tuple of dictionaries
       Two dictionaries keyed by node containing the hub and authority
       values.

Notes
-----
    The eigenvector calculation is done by the power iteration method
    and has no guarantee of convergence.  The iteration will stop
    after max_iter iterations or an error tolerance of
    number_of_nodes(G)*tol has been reached.

    The HITS algorithm was designed for directed graphs but this
    algorithm does not check if the input graph is directed and will
    execute on undirected graphs.



Reference
---------
    .. [1] A. Langville and C. Meyer, 
       "A survey of eigenvector methods of web information retrieval."  
       http://citeseer.ist.psu.edu/713792.html
    .. [2] Jon Kleinberg, 
       Authoritative sources in a hyperlinked environment
       Journal of the ACM 46 (5): 604-32, 1999. 
       doi:10.1145/324133.324140. 
       http://www.cs.cornell.edu/home/kleinber/auth.pdf.


-----------------------------------------------------------------------------
*/

#include "HITSCentralityNode.h"


///
/// Constructors and Destructors
///


//!
//! Constructor of the HITSCentralityNode class.
//!
//! \param name The name for the new node.
//! \param parameterRoot A copy of the parameter tree specific for the type of the node.
//!
HITSCentralityNode::HITSCentralityNode ( const QString &name, ParameterGroup *parameterRoot ) :
    Node(name, parameterRoot)
{
}


//!
//! Destructor of the HITSCentralityNode class.
//!
//! Defined virtual to guarantee that the destructor of a derived class
//! will be called if the instance of the derived class is saved in a
//! variable of its parent class type.
//!
HITSCentralityNode::~HITSCentralityNode ()
{
}


