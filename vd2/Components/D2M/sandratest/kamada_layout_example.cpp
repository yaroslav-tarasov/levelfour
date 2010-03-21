#include <boost/property_map/property_map.hpp>
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/circle_layout.hpp>
#include <boost/graph/kamada_kawai_spring_layout.hpp>
#include <boost/graph/graph_utility.hpp>
#include <boost/graph/topology.hpp>

#include <windows.h>
#include <osg/ref_ptr>
#include <osg/Geode>
#include <osg/Notify>
#include <osg/ShapeDrawable>
#include <osgDB/WriteFile>

#include <iostream>

//#include <osg/PositionAttitudeTransform>

	typedef boost::square_topology<>::point_type Point;
	struct VertexProperties
	{
		std::size_t index;
	    Point point;
	};

	struct EdgeProperty
	{
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


	void saveOsgFile(osg::ref_ptr<osg::Node> sceneGraph)
	{
		if (!sceneGraph.valid())
			osg::notify(osg::FATAL) << "Failed in saveOsgFile\n";

		bool result = osgDB::writeNodeFile( *(sceneGraph.get()), "SASN.osg" );
		if( !result )
			osg::notify(osg::FATAL) << "Failed in osgDB::writeNodeFile().\n";
	}

	osg::Cylinder *createCylinder(osg::Vec3 startPoint, osg::Vec3 endPoint)
	{
		const osg::Vec3 rotAxis = osg::Vec3(0.0f, 1.0f, 0.0f); //Axis around which the cylinder will be rotated
		float radius = 0.2f;
		osg::Vec3 center = (startPoint + endPoint)/2;
		osg::Vec3 diff = endPoint - startPoint;
		float height = diff.length();
		float angle = std::atan( diff.x()/diff.z() );

		osg::Cylinder *cylinder = new osg::Cylinder( center, radius, height );
		cylinder->setRotation( osg::Quat( angle, rotAxis ) );
		return cylinder;
	}

	void constructScene(Graph &g, PositionMap &positionMap)
	{
		//Create a cube for each vertex and a cylinder for each edge
		//Add all cubes and cylinders to a Geode
		float cubeSize = 1.f;
		osg::Vec3 position1;
		osg::Vec3 position2;
		osg::ref_ptr<osg::ShapeDrawable> cube;
		osg::ref_ptr<osg::ShapeDrawable> line;
		osg::ref_ptr<osg::Geode> geode = new osg::Geode;
		
		boost::graph_traits<Graph>::vertex_iterator i, end;
		boost::graph_traits<Graph>::out_edge_iterator ei, edge_end;
		for (boost::tie(i, end) = boost::vertices(g); i != end; ++i) {
			position1 = osg::Vec3( positionMap[*i][0], 0.f, positionMap[*i][1] );
			cube = new osg::ShapeDrawable( new osg::Box(position1, cubeSize) );
			geode->addDrawable(cube);
			for (boost::tie(ei,edge_end) = out_edges(*i, g); ei != edge_end; ++ei)
			{
				position2 = osg::Vec3( positionMap[target(*ei, g)][0], 0.f, positionMap[target(*ei, g)][1] );
				line = new osg::ShapeDrawable( createCylinder(position1, position2) );
				geode->addDrawable(line);
			}
		}

		//Save the Geode in an osg file
		saveOsgFile(geode);
	}

	int main(int,char*[])
	{
		// declare a graph object
		Graph g;
		VertexIndexPropertyMap vertexIdPropertyMap = boost::get(&VertexProperties::index, g);

		// add vertices to the graph object
		for (int i = 0; i < 8; ++i) {
			VertexDescriptor vd = boost::add_vertex(g);
			vertexIdPropertyMap[vd] = i;
		}

		// add edges to the graph object
		add_edge(vertex(0,g), vertex(1,g), EdgeProperty(1), g);
		add_edge(vertex(1,g), vertex(2,g), EdgeProperty(1), g);
		add_edge(vertex(2,g), vertex(3,g), EdgeProperty(5), g);
		add_edge(vertex(3,g), vertex(4,g), EdgeProperty(5), g);
		add_edge(vertex(4,g), vertex(5,g), EdgeProperty(1), g);
		add_edge(vertex(5,g), vertex(6,g), EdgeProperty(1), g);
		add_edge(vertex(6,g), vertex(7,g), EdgeProperty(1), g);
		add_edge(vertex(7,g), vertex(0,g), EdgeProperty(1), g);


		// Create property maps for vertex position and for edge weight
		PositionMap positionMap = boost::get(&VertexProperties::point, g);
		WeightPropertyMap weightPropertyMap = boost::get(&EdgeProperty::weight, g);

		// apply Boost's circle layout
		boost::circle_graph_layout(g, positionMap, 100);

		// apply Boost's kamada layout
		bool retval = boost::kamada_kawai_spring_layout(g, positionMap, weightPropertyMap, 
							boost::square_topology<>(), boost::side_length<double>(10),	
							boost::layout_tolerance<>(), 1, vertexIdPropertyMap);
		if (!retval)
		{
			 std::cout << "kamada_kawai_spring_layout returned false";
		}

		// create an OSG scene and save it as an osg file
		constructScene(g, positionMap);
		return 0;
	}