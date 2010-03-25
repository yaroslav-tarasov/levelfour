#ifndef GRAPH_PROPERTY_MAPPER_H
#define GRAPH_PROPERTY_MAPPER_H

#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/topology.hpp>
#include <boost/graph/circle_layout.hpp>
#include <boost/graph/random_layout.hpp>
#include <boost/graph/kamada_kawai_spring_layout.hpp>

namespace graphdefs
{
	typedef boost::square_topology<>::point_type Point;
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
		random2D,
		kamada_kawai2D
	};

}

class GraphPropertyMapper
{

public:
	//typedef the graph
	typedef boost::adjacency_list<boost::listS,
			boost::listS, boost::undirectedS,
			graphdefs::VertexProperties, graphdefs::EdgeProperties> Graph;

	typedef boost::property_map<Graph, std::size_t graphdefs::VertexProperties::*>::type VertexIdMap;
	typedef boost::property_map<Graph, graphdefs::Point graphdefs::VertexProperties::*>::type PositionMap;
	typedef boost::property_map<Graph, double graphdefs::EdgeProperties::*>::type WeightMap;
	typedef boost::graph_traits<Graph>::vertex_descriptor VertexDescriptor;

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

	void addEdge (int startVertex, int endVertex, double weight)
	{
		boost::add_edge(vertex(startVertex,g), vertex(endVertex,g), graphdefs::EdgeProperties(weight), g);
	}

	void setPositionTransform(graphdefs::layout selectedLayout)
	{
		positionTransform = selectedLayout;
	}

	void layoutCircle2D()
	{
		boost::circle_graph_layout(g, positionMap, 100);
	}

	void layoutRandom2D()
	{
		boost::minstd_rand gen;
		gen.seed (static_cast<unsigned int>(std::time(0)));
		boost::rectangle_topology<boost::minstd_rand> topo = boost::rectangle_topology<boost::minstd_rand>(gen, -50.f,-50.f,50.f,50.f);
		boost::random_graph_layout(g, positionMap, topo);
	}

	void layoutKamadaKawai2D()
	{
		// First apply boost's circle layout as a starting point
		boost::circle_graph_layout(g, positionMap, 100);

		// Then apply boost's kamada layout
		bool retval = boost::kamada_kawai_spring_layout(g, positionMap, weightMap, 
					  boost::square_topology<>(), boost::side_length<double>(10),	
					  boost::layout_tolerance<>(), 1, vertexIdMap);
		if (!retval)
		{
			 std::cout << "kamada_kawai_spring_layout returned false";
		}
	}

	void transformPosition()
	{
		switch( positionTransform )
		{
		case graphdefs::circle2D:
			layoutCircle2D();
			break;
		case graphdefs::random2D:
			layoutRandom2D();
			break;
		case graphdefs::kamada_kawai2D:
			layoutKamadaKawai2D();
			break;
		}
	}

protected:
   

private:

};

#endif

