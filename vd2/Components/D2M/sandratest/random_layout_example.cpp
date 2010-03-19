#include <boost/property_map/property_map.hpp>
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/circle_layout.hpp>
#include <boost/graph/random_layout.hpp>
#include <boost/graph/graph_utility.hpp>
#include <boost/graph/topology.hpp>

#include <windows.h>
#include <osg/ref_ptr>
#include <osg/Geode>
#include <osg/Notify>
#include <osg/ShapeDrawable>
#include <osgDB/WriteFile>

//#include <osg/PositionAttitudeTransform>

	typedef boost::square_topology<>::point_type Point;
	struct VertexProperties
	{
	    std::size_t index;
	    Point point;
	};

	typedef boost::adjacency_list<boost::listS,
            boost::listS, boost::undirectedS,
            VertexProperties> Graph;

	typedef boost::property_map<Graph, std::size_t VertexProperties::*>::type VertexIndexPropertyMap;
	typedef boost::property_map<Graph, Point VertexProperties::*>::type PositionMap;

	typedef boost::graph_traits<Graph>::vertex_descriptor VertexDescriptor;


	void saveOsgFile(osg::ref_ptr<osg::Node> sceneGraph) {
		if (!sceneGraph.valid())
			osg::notify(osg::FATAL) << "Failed in saveOsgFile\n";

		bool result = osgDB::writeNodeFile( *(sceneGraph.get()), "SASN2.osg" );
		if( !result )
			osg::notify(osg::FATAL) << "Failed in osgDB::writeNodeFile().\n";
	}

	void constructScene(Graph g, PositionMap positionMap) {
		
		//Create a cube for each vertex and add all cubes to a Geode
		float cubeSize = 4.f;
		osg::Vec3 position;
		osg::ref_ptr<osg::ShapeDrawable> cube;
		osg::ref_ptr<osg::Geode> geode = new osg::Geode;
		
		boost::graph_traits<Graph>::vertex_iterator i, end;
		for (boost::tie(i, end) = boost::vertices(g); i != end; ++i) {
			position = osg::Vec3( positionMap[*i][0], 0.f, positionMap[*i][1] );
			cube = new osg::ShapeDrawable( new osg::Box(position, cubeSize) );
		    geode->addDrawable(cube);
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
		for (int i = 0; i < 7; ++i) {
			VertexDescriptor vd = boost::add_vertex(g);
			vertexIdPropertyMap[vd] = i;
		}
		
		// add edges to the graph object
		add_edge(vertex(1,g), vertex(0,g), g);
		add_edge(vertex(2,g), vertex(6,g), g);
		
		// apply Boost's circle layout
		PositionMap positionMap = boost::get(&VertexProperties::point, g);
		boost::minstd_rand gen;
		gen.seed (static_cast<unsigned int>(std::time(0)));
		boost::rectangle_topology<boost::minstd_rand> topo = boost::rectangle_topology<boost::minstd_rand>(gen, -50.f,-50.f,50.f,50.f);
		boost::random_graph_layout(g, positionMap, topo);

		// create an OSG scene and save it as an osg file
		constructScene(g, positionMap);
		return 0;
	}