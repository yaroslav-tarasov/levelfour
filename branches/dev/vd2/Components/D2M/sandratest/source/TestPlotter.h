#ifndef TEST_PLOTTER_H
#define TEST_PLOTTER_H

#include "GraphPropertyMapper.h"

#include <windows.h>
#include <osg/ref_ptr>
#include <osg/Geode>
#include <osg/Notify>
#include <osg/ShapeDrawable>
#include <osgDB/WriteFile>
#include <osgText/Text>

template<typename VertexListGraph, typename PositionMap, typename VertexIdMap, typename WeightMap>
class TestPlotter
{

public:

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
		const osg::Vec3 zAxis = osg::Vec3f(0.0f, 0.0f, 1.0f); //This is the original orientation of an OSG cylinder
		float radius = weight/15;
		osg::Vec3 center = (startPoint + endPoint)/2;
		osg::Vec3 diff = endPoint - startPoint; //Imagined vector exactely where the cylinder is to be placed
		float height = diff.length(); //The cylinder length is to be the same as the length of diff
		osg::Vec3 rotAxis = zAxis ^ diff; //Axis around which the cylinder will be rotated, perpendicular both to diff and to the z axis
		float cosAngle = zAxis * diff / height; //Project a normalized diff on the z axis
		float angle = std::acos( cosAngle ); //The cylinder should be rotated so that it becomes parallel with diff

		osg::Cylinder *cylinder = new osg::Cylinder( center, radius, height );
		cylinder->setRotation( osg::Quat( angle, rotAxis ) );
		return cylinder;
	}

	void constructScene(VertexListGraph &g, PositionMap &positionMap, VertexIdMap vertexIdMap, WeightMap &weightMap)
	{
		//Create a cube and a label for each vertex and a cylinder for each edge
		//Add all cubes, labels and cylinders to a Geode
		float cubeSize = 0.3f;
		float weight;
		osg::Vec3 position1;
		osg::Vec3 position2;
		osg::ref_ptr<osg::ShapeDrawable> cube;
		osg::ref_ptr<osg::ShapeDrawable> line;
		osg::ref_ptr<osg::Geode> geode = new osg::Geode;
		
		boost::graph_traits<VertexListGraph>::vertex_iterator i, end;
		boost::graph_traits<VertexListGraph>::out_edge_iterator ei, edge_end;
		for (boost::tie(i, end) = boost::vertices(g); i != end; ++i) {

			//Add a cube representing this vertex
			position1 = osg::Vec3( positionMap[*i][0], positionMap[*i][1], positionMap[*i][2] );
			cube = new osg::ShapeDrawable( new osg::Box(position1, cubeSize) );
			geode->addDrawable(cube);

			//Add a cylinder representing each edge from this vertex
			for (boost::tie(ei,edge_end) = out_edges(*i, g); ei != edge_end; ++ei)
			{
				position2 = osg::Vec3( positionMap[target(*ei, g)][0], positionMap[target(*ei, g)][1], positionMap[target(*ei, g)][2] );
				weight = weightMap[*ei];
				line = new osg::ShapeDrawable( createCylinder(position1, position2, weight) );
				geode->addDrawable(line);
			}

			//Add a label with the vertex ID
			osgText::Text* vertexLabel = new osgText::Text(); 
			vertexLabel->setCharacterSize(0.5);
			vertexLabel->setColor( osg::Vec4(1.0, 0.0, 0.0, 1.0) ); //The color red
			char vertexId[10];
			sprintf(vertexId, "%d",vertexIdMap[*i]); //Convert the vertex ID to a string
			vertexLabel->setText( vertexId );
			const osg::Vec3 labelDisplacement = osg::Vec3f(-cubeSize/4, -cubeSize/4, cubeSize/1.9); //Move the label slightly so that it isn't embedded inside the cube
			vertexLabel->setPosition( position1 + labelDisplacement ); 
			geode->addDrawable(vertexLabel);
		}

		//Save the Geode in an osg file
		saveOsgFile(geode);
	}

	void constructScene(VertexListGraph &g, PositionMap &positionMap)
	{
		//Create a cube for each vertex and a cylinder for each edge
		//Add all cubes and cylinders to a Geode
		float cubeSize = 1.f;
		osg::Vec3 position1;
		osg::Vec3 position2;
		osg::ref_ptr<osg::ShapeDrawable> cube;
		osg::ref_ptr<osg::ShapeDrawable> line;
		osg::ref_ptr<osg::Geode> geode = new osg::Geode;
		
		boost::graph_traits<VertexListGraph>::vertex_iterator i, end;
		boost::graph_traits<VertexListGraph>::out_edge_iterator ei, edge_end;
		for (boost::tie(i, end) = boost::vertices(g); i != end; ++i) {
			position1 = osg::Vec3( positionMap[*i][0], positionMap[*i][1], positionMap[*i][2] );
			cube = new osg::ShapeDrawable( new osg::Box(position1, cubeSize) );
			geode->addDrawable(cube);
			for (boost::tie(ei,edge_end) = out_edges(*i, g); ei != edge_end; ++ei)
			{
				position2 = osg::Vec3( positionMap[target(*ei, g)][0], positionMap[target(*ei, g)][1], positionMap[target(*ei, g)][2] );
				line = new osg::ShapeDrawable( createCylinder(position1, position2, 1) );
				geode->addDrawable(line);
			}
		}

		//Save the Geode in an osg file
		saveOsgFile(geode);
	}
};

#endif
