#ifndef GRAPH_PROPERTY_MAPPER_H
#define GRAPH_PROPERTY_MAPPER_H

#include "TestPlotter.h"
#include "cone_layout3D.h"
#include "phyllo_layout3D.h"
#include "kamada_layout3D.h"
#include "sphere_layout3D.h"

#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/topology.hpp>
#include <boost/graph/circle_layout.hpp>
#include <boost/graph/random_layout.hpp>

namespace graphdefs
{
	typedef boost::cube_topology<>::point_type Point;

	struct VertexProperties
	{
		std::size_t id;
	    Point position;
	};

	struct EdgeProperties
	{
		EdgeProperties():weight(0.0) {}
		EdgeProperties(const std::size_t &w):weight(w) {}
		double weight;
	};

	enum layout {
		circle2D,
		sphere3D,
		random3D,
		kamada_kawai2D,
		kamada_kawai3D,
		cone3D,
		phyllo3D
	};
}

template<class Directed = boost::directedS>
class GraphPropertyMapper
{

public:
	//typedef the graph
	typedef boost::adjacency_list<boost::listS,
		boost::listS, Directed, 
		graphdefs::VertexProperties,graphdefs::EdgeProperties> Graph;

	typedef typename boost::property_map<Graph, std::size_t graphdefs::VertexProperties::*>::type VertexIdMap;
	typedef typename boost::property_map<Graph, graphdefs::Point graphdefs::VertexProperties::*>::type PositionMap;
	typedef typename boost::property_map<Graph, double graphdefs::EdgeProperties::*>::type WeightMap;
	typedef typename boost::graph_traits<Graph>::vertex_descriptor VertexDescriptor;

	Graph g;
	VertexIdMap vertexIdMap;
	PositionMap positionMap;
	WeightMap weightMap;
	graphdefs::layout positionTransform;

	GraphPropertyMapper()
	{
		// Create property map for vertex ID
		vertexIdMap = boost::get(&graphdefs::VertexProperties::id, g);
		// Create property map for vertex position 
		positionMap = boost::get(&graphdefs::VertexProperties::position, g);
		// Create property map for edge weight
		weightMap = boost::get(&graphdefs::EdgeProperties::weight, g);
	}

    ~GraphPropertyMapper()
	{
	}

	void addVertex(int i)
	{
		VertexDescriptor vd = boost::add_vertex(g);
		vertexIdMap[vd] = i;
	}

	void addEdge (int startVertex, int endVertex, const size_t weight)
	{
		boost::add_edge(vertex(startVertex,g), vertex(endVertex,g), graphdefs::EdgeProperties(weight), g);
	}

	void setPositionTransform(graphdefs::layout selectedLayout)
	{
		positionTransform = selectedLayout;
	}

	void layoutCircle2D()
	{
		boost::circle_graph_layout(g, positionMap, 10);
	}

	void layoutSphere3D()
	{
		graphdefs::sphere_graph_layout(g, positionMap);
	}

	void layoutRandom3D()
	{
		boost::minstd_rand gen;
		gen.seed (static_cast<unsigned int>(std::time(0)));
		boost::cube_topology<boost::minstd_rand> topo = boost::cube_topology<boost::minstd_rand>(gen);
		boost::random_graph_layout(g, positionMap, topo);
	}

	void layoutKamadaKawai2D()
	{
		// First apply boost's circle layout as a starting point
		boost::circle_graph_layout(g, positionMap, 100);

		// Then apply our kamada layout
		bool retval = graphdefs::kamada_kawai_spring_layout(g, positionMap, weightMap, boost::cube_topology<>(), 
															graphdefs::detail::graph::edge_or_side<true, double>(10),
															graphdefs::layout_tolerance<>(), 1, vertexIdMap);

		if (!retval)
		{
			 std::cout << "kamada_kawai_spring_layout returned false";
		}
	}

	void layoutKamadaKawai3D()
	{
		// First apply our sphere layout as a starting point
		graphdefs::sphere_graph_layout(g, positionMap);

		// Then apply our kamada layout
		bool retval = graphdefs::kamada_kawai_spring_layout(g, positionMap, weightMap, boost::cube_topology<>(), 
															graphdefs::detail::graph::edge_or_side<true, double>(10),
															graphdefs::layout_tolerance<>(), 1, vertexIdMap);

		if (!retval)
		{
			 std::cout << "kamada_kawai_spring_layout returned false";
		}
	}


	void layoutCone3D()
	{
		boost::graph_traits<Graph>::vertex_descriptor rootVertex;
		rootVertex=*(vertices(g).first);
		graphdefs::cone_tree_layout(g, positionMap, rootVertex);
	}

	void layoutPhyllo3D()
	{
		boost::graph_traits<Graph>::vertex_descriptor rootVertex;
		rootVertex=*(vertices(g).first);
		float angle = 0.3;
		float spacing = 15.;
		graphdefs::phyllo_tree_layout(g, positionMap, rootVertex, angle, spacing);
	}

	void transformPosition()
	{
		switch( positionTransform )
		{
		case graphdefs::circle2D:
			layoutCircle2D();
			break;
		case graphdefs::sphere3D:
			layoutSphere3D();
			break;
		case graphdefs::random3D:
			layoutRandom3D();
			break;
		case graphdefs::kamada_kawai2D:
			layoutKamadaKawai2D();
			break;
		case graphdefs::kamada_kawai3D:
			layoutKamadaKawai3D();
			break;
		case graphdefs::cone3D:
			layoutCone3D();
			break;
		case graphdefs::phyllo3D:
			layoutPhyllo3D();
			break;
		}
	}

	void plotScene()
	{
		TestPlotter<Graph, PositionMap, WeightMap> tp;
		tp.constructScene(g, positionMap, weightMap);  // tp.constructScene(g, positionMap); //TODO
	}

protected:
   

private:

};

#endif

