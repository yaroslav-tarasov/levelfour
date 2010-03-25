#ifndef TEST_PLOTTER_H
#define TEST_PLOTTER_H

#include "GraphPropertyMapper.h"

#include <windows.h>
#include <osg/ref_ptr>
#include <osg/Geode>
#include <osg/Notify>
#include <osg/ShapeDrawable>
#include <osgDB/WriteFile>

class TestPlotter
{

public:
	void constructScene(GraphPropertyMapper::Graph &g, GraphPropertyMapper::PositionMap &positionMap, GraphPropertyMapper::WeightMap &weightMap);

};

#endif
