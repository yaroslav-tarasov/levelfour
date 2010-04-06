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
		const double pi = 3.1415926, zStep = -1;
		double zVal = 0.; //z value incrementor. The root vertex will be placed at level z=0
		int radius = 0; // The radius will increase for each level in the cone

		//Declare tools for perusing the tree
		typedef typename boost::graph_traits<VertexListGraph>::vertex_descriptor vertex_descriptor;
		std::list<vertex_descriptor> *childList = new std::list<vertex_descriptor>();
		std::list<vertex_descriptor> *grandchildList;
		std::list<vertex_descriptor>::iterator childListIterator;
		typedef typename boost::graph_traits<VertexListGraph>::vertices_size_type vertices_size_type;
		vertices_size_type numChildren, i;

		//The first time around, childList1 has only one element, namely the root vertex
		(*childList).push_back(rootVertex);

		//Step through the tree one level at a time, where "level" is determined by distance to the root vertex
		while ( !(*childList).empty() )
		{				
			boost::graph_traits<VertexListGraph>::out_edge_iterator ei, edge_end;
			grandchildList = new std::list<vertex_descriptor>();

			numChildren = (*childList).size();
			i = 0;
			for(childListIterator = (*childList).begin(); childListIterator != (*childList).end(); childListIterator++)
			{
				//Distribute all this level's vertices on a circle at z=zVal
				vertex_descriptor v = *childListIterator;
				position[v][0] = radius * std::cos(i * 2*pi/numChildren);
				position[v][1] = radius * std::sin(i * 2*pi/numChildren);
				position[v][2] = zVal;
				++i;

				//Collect all next level's vertices, so that you have them available in the next loop
				for (boost::tie(ei,edge_end) = out_edges(v, g); ei != edge_end; ++ei)
				{
					vertex_descriptor *grandchild = new vertex_descriptor();
					*grandchild = target(*ei, g);
					(*grandchildList).push_back( *grandchild );
				}
			}

			//Take a step down to the next level
//			(*childList).clear(); //is this necessary?
			delete childList;
			childList = grandchildList;
			grandchildList = 0;

			radius++;
			zVal = zVal + zStep;
		} // end while loop

		delete childList;
	} // end function cone_graph_layout
} // end namespace graphdefs

#endif // CONE_LAYOUT_HPP
