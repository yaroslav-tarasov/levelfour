// Copyright 2004 The Trustees of Indiana University.

// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

//  Authors: Douglas Gregor
//           Andrew Lumsdaine
#ifndef CONE_LAYOUT_HPP
#define CONE_LAYOUT_HPP
#include <boost/config/no_tr1/cmath.hpp>
#include <boost/math/constants/constants.hpp>
#include <utility>
#include <boost/graph/graph_traits.hpp>
#include <boost/graph/iteration_macros.hpp>
#include <boost/graph/topology.hpp>
#include <boost/static_assert.hpp>

namespace graphdefs {
	/** 
	 * \brief Layout the graph with the vertices at the points of a regular
	 * n-polygon. 
	 *
	 * The distance from the center of the polygon to each point is
	 * determined by the @p radius parameter. The @p position parameter
	 * must be an Lvalue Property Map whose value type is a class type
	 * containing @c x and @c y members that will be set to the @c x and
	 * @c y coordinates.
	 */

	template<typename VertexListGraph, typename PositionMap>
	void cone_graph_layout(const VertexListGraph& g, PositionMap position, typename boost::graph_traits<VertexListGraph>::vertex_descriptor rootVertex)
	{
		typedef typename boost::graph_traits<VertexListGraph>::vertex_descriptor vertex_descriptor;
		const double pi = 3.1415926, zStep = -1;
		double zVal = 0.;
		int radius = 0;

		std::list<vertex_descriptor> *childList1 = new std::list<vertex_descriptor>();
		std::list<vertex_descriptor> *childList2;
		std::list<vertex_descriptor>::iterator childListIterator;
		typedef typename boost::graph_traits<VertexListGraph>::vertices_size_type vertices_size_type;
		vertices_size_type numChildren;
		vertices_size_type i;

		(*childList1).push_back(rootVertex);
		while ( !(*childList1).empty() )
		{				
			boost::graph_traits<VertexListGraph>::out_edge_iterator ei, edge_end;
			childList2 = new std::list<vertex_descriptor>();

			numChildren = (*childList1).size();
			i = 0;
			for(childListIterator = (*childList1).begin(); childListIterator != (*childList1).end(); childListIterator++)
			{
				//Distribute all vertices of this level on a circle
				vertex_descriptor v = *childListIterator;
				position[v][0] = radius * std::cos(i * 2*pi/numChildren);
				position[v][1] = radius * std::sin(i * 2*pi/numChildren);
				position[v][2] = zVal;
				++i;

				//Collect all vertices of the next level in the tree
				for (boost::tie(ei,edge_end) = out_edges(v, g); ei != edge_end; ++ei)
				{
					vertex_descriptor grandchild = target(*ei, g);
					(*childList2).push_back( grandchild );
				}
			}

			//Take a step down to the next level
//			(*childList1).clear(); //is this necessary?
			delete childList1;
			childList1 = childList2;
			childList2 = 0;

			radius++;
			zVal = zVal + zStep;
		}
		delete childList1;
	} // end function cone_graph_layout
} // end namespace graphdefs

#endif // CONE_LAYOUT_HPP
