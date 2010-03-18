#ifndef IBOOST_GRAPH_IO_DATA_H
#define IBOOST_GRAPH_IO_DATA_H

#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/topology.hpp>
#include "IVisSystemNodeIOData.h"

namespace sandra
{

	typedef boost::square_topology<>::point_type Point;
	struct VertexProperties
	{
		std::size_t index;
	    Point point;
	};

	struct EdgeProperty
	{
		EdgeProperty():weight(0.0) {}
		EdgeProperty(const std::size_t &w):weight(w) {}
		double weight;
	};
	
	typedef boost::adjacency_list<boost::listS,
			boost::listS, boost::undirectedS,
			VertexProperties, EdgeProperty> Graph;

	typedef boost::property_map<Graph, std::size_t VertexProperties::*>::type VertexIndexPropertyMap;
	typedef boost::property_map<Graph, Point VertexProperties::*>::type PositionMap;
	typedef boost::property_map<Graph, double EdgeProperty::*>::type WeightPropertyMap;
	typedef boost::graph_traits<Graph>::vertex_descriptor VertexDescriptor;
}

//class Graph;
class IBoostGraphIOData : virtual public IVisSystemNodeIOData
{
public:
	virtual sandra::Graph* getBoostGraph() = 0;
};

#endif