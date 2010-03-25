#include "TestPlotter.h"

	void saveOsgFile(osg::ref_ptr<osg::Node> sceneGraph)
	{
		if (!sceneGraph.valid())
			osg::notify(osg::FATAL) << "Failed in saveOsgFile\n";

		bool result = osgDB::writeNodeFile( *(sceneGraph.get()), "SASN.osg" );
		if( !result )
			osg::notify(osg::FATAL) << "Failed in osgDB::writeNodeFile().\n";
	}

	osg::Cylinder *createCylinder(osg::Vec3 startPoint, osg::Vec3 endPoint, float weight)
	{
		const osg::Vec3 rotAxis = osg::Vec3(0.0f, 1.0f, 0.0f); //Axis around which the cylinder will be rotated
		float radius = weight/15;
		osg::Vec3 center = (startPoint + endPoint)/2;
		osg::Vec3 diff = endPoint - startPoint;
		float height = diff.length();
		float angle = std::atan( diff.x()/diff.z() );

		osg::Cylinder *cylinder = new osg::Cylinder( center, radius, height );
		cylinder->setRotation( osg::Quat( angle, rotAxis ) );
		return cylinder;
	}

	void TestPlotter::constructScene(GraphPropertyMapper::Graph &g, GraphPropertyMapper::PositionMap &positionMap, GraphPropertyMapper::WeightMap &weightMap)
	{
		//Create a cube for each vertex and a cylinder for each edge
		//Add all cubes and cylinders to a Geode
		float cubeSize = 1.f;
		float weight;
		osg::Vec3 position1;
		osg::Vec3 position2;
		osg::ref_ptr<osg::ShapeDrawable> cube;
		osg::ref_ptr<osg::ShapeDrawable> line;
		osg::ref_ptr<osg::Geode> geode = new osg::Geode;
		
		boost::graph_traits<GraphPropertyMapper::Graph>::vertex_iterator i, end;
		boost::graph_traits<GraphPropertyMapper::Graph>::out_edge_iterator ei, edge_end;
		for (boost::tie(i, end) = boost::vertices(g); i != end; ++i) {
			position1 = osg::Vec3( positionMap[*i][0], 0.f, positionMap[*i][1] );
			cube = new osg::ShapeDrawable( new osg::Box(position1, cubeSize) );
			geode->addDrawable(cube);
			for (boost::tie(ei,edge_end) = out_edges(*i, g); ei != edge_end; ++ei)
			{
				position2 = osg::Vec3( positionMap[target(*ei, g)][0], 0.f, positionMap[target(*ei, g)][1] );
				weight = weightMap[*ei];
				line = new osg::ShapeDrawable( createCylinder(position1, position2, weight) );
				geode->addDrawable(line);
			}
		}

		//Save the Geode in an osg file
		saveOsgFile(geode);
	}

