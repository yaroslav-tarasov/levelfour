#ifndef SPHERE_LAYOUT_HPP
#define SPHERE_LAYOUT_HPP

#include <boost/graph/graph_traits.hpp>

namespace graphdefs {
					  
	template<typename VertexListGraph, typename PositionMap>
	void sphere_graph_layout(const VertexListGraph& g, PositionMap position)
	{
		// Algorithm taken from the paper "Distributing many points on a sphere"
		// by E.B. Saff and A.B.J. Kuijlaars
		typedef typename boost::graph_traits<VertexListGraph>::vertices_size_type vertices_size_type;
		const double pi = boost::math::constants::pi<double>();

		double h, oldFmod, x, y, z; //variables for storing intermediate calculation values
		vertices_size_type i=0, noOfVertices=num_vertices(g); //i is a vertex counter

		//Temporarily use the position map for storing intermediate angle values
		boost::graph_traits<VertexListGraph>::vertex_iterator vi, vi_end;
		for (boost::tie(vi, vi_end) = boost::vertices(g); vi != vi_end; ++vi)
		{
			if (i == 0) //do this only for the first vertex
			{
				position[*vi][0] = pi;
				position[*vi][1] = 0;

			} else if (i < noOfVertices-1) //do this for all vertices but the first and the last one
			{
				h = -1 + 2*i/(double)(noOfVertices-1);
				position[*vi][0] = std::acos(h);
				position[*vi][1] = fmod(( oldFmod + 3.6/std::sqrt(noOfVertices*(1-h*h)) ), 2*pi);

			} else if (noOfVertices >=2) //do this only for the last vertex
			{
				position[*vi][0] = 0;
				position[*vi][1] = 0;
			}
			oldFmod = position[*vi][1];
			i++;
		}

		//Now convert the intermediate angle values into final position values
		for (boost::tie(vi, vi_end) = boost::vertices(g); vi != vi_end; ++vi)
		{
			x=cos(position[*vi][1])*sin(position[*vi][0]);
			y=sin(position[*vi][1])*sin(position[*vi][0]);
			z=cos(position[*vi][0]);
			position[*vi][0]=x;
			position[*vi][1]=y;
			position[*vi][2]=z;
		}

	} // end function sphere_graph_layout

} // end namespace graphdefs

#endif // SPHERE_LAYOUT_HPP
