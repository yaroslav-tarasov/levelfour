#include "OsgGroupVisNodeIOData.h"

OsgGroupVisNodeIOData::OsgGroupVisNodeIOData()
{
	m_osgNode = new osg::Group;
}

OsgGroupVisNodeIOData::~OsgGroupVisNodeIOData()
{
}

void OsgGroupVisNodeIOData::addOsgNode(osg::ref_ptr<osg::Node> node)
{
	dynamic_cast<osg::Group*>(m_osgNode)->addChild(node);
}

