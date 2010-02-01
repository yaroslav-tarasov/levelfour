#include "OsgPATVisNodeIOData.h"

OsgPATVisNodeIOData::OsgPATVisNodeIOData()
{
	m_osgNode = new osg::PositionAttitudeTransform;
}

OsgPATVisNodeIOData::~OsgPATVisNodeIOData()
{
}

void OsgPATVisNodeIOData::addOsgNode(osg::ref_ptr<osg::Node> node)
{
	dynamic_cast<osg::PositionAttitudeTransform*>(m_osgNode)->addChild(node);
}

