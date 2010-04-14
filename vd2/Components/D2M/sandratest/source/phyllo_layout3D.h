#ifndef PHYLLO_LAYOUT_HPP
#define PHYLLO_LAYOUT_HPP

#include <boost/graph/graph_traits.hpp>

namespace graphdefs {

	extern float convertPolarToX(float phi, float radius);
	extern float convertPolarToY(float phi, float radius);

	template<typename VertexListGraph, typename PositionMap>
	void placePhylloChildren(const VertexListGraph& g, PositionMap position, typename boost::graph_traits<VertexListGraph>::vertex_descriptor parentVertex, float angle, float spacing, double centreX, double centreY, double zVal)
	{
		typedef typename boost::graph_traits<VertexListGraph>::vertex_descriptor vertex_descriptor;
		const float cStep = -7; // spacing increment. The spacing will decrease for each level of the tree
		const double zStep = -5; // z value increment. The root vertex is placed at level z=0, the root's children at level z=zStep*1, etc.
		int n = 1; //child counter
		float phi; //angle in polar coordinates
		float radius; //distance from origo in polar coordinates
		double xVal; //x value in cartesian coordinates
		double yVal; //y value in cartesian coordinates

		zVal = zVal + zStep; //New level of the tree, new z value
		spacing = spacing + cStep; //New level of the tree, new spacing constant

		//For all children of parentVertex
		boost::graph_traits<VertexListGraph>::out_edge_iterator ei, edge_end;
		for (boost::tie(ei,edge_end) = out_edges(parentVertex, g); ei != edge_end; ++ei)
		{
			vertex_descriptor *child = new vertex_descriptor();
			*child = target(*ei, g);

			//Position the child
			phi = n * angle;
			radius = spacing * std::sqrt( (float)n );
			xVal = convertPolarToX(phi, radius);
			yVal = convertPolarToY(phi, radius);
			position[*child][0] = xVal + centreX;
			position[*child][1] = yVal + centreY;
			position[*child][2] = zVal;

			//Recursively position all grandchildren and then all their descendants
			placePhylloChildren(g, position, *child, angle, spacing, xVal, yVal, zVal);

			//Count up n for the next sibling
			n++;
		}
	} // end function placePhylloChildren

	template<typename VertexListGraph, typename PositionMap>
	void phyllo_tree_layout(const VertexListGraph& g, PositionMap position, typename boost::graph_traits<VertexListGraph>::vertex_descriptor rootVertex, float angle, float spacing)
	{
		//Position the root vertex
		position[rootVertex][0] = 0.;
		position[rootVertex][1] = 0.;
		position[rootVertex][2] = 0.;

		//Position the root vertex's children and then, recursively, their descendants
		graphdefs::placePhylloChildren(g, position, rootVertex, angle, spacing, 0., 0., 0.);
	} // end function phyllo_tree_layout

} // end namespace graphdefs

#endif // PHYLLO_LAYOUT_HPP
