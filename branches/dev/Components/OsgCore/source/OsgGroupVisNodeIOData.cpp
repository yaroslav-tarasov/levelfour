#include "OsgGroupVisNodeIOData.h"

OsgGroupVisNodeIOData::OsgGroupVisNodeIOData()
{
	m_osgGroup = new osg::Group;
}

OsgGroupVisNodeIOData::~OsgGroupVisNodeIOData()
{
}

void OsgGroupVisNodeIOData::addOsgNode(osg::ref_ptr<osg::Node> node)
{
	m_osgGroup->addChild(node);
}

void OsgGroupVisNodeIOData::setOsgGroup(osg::ref_ptr<osg::Group> group)
{
	m_osgGroup = group;
}

void * OsgGroupVisNodeIOData::dataPointer()
{
	return m_osgGroup;
}

QString OsgGroupVisNodeIOData::dataTypeName()
{
	return "osg::ref_ptr<osg::Group>";
}

GCF_BEGIN_QUERY_TABLE(OsgGroupVisNodeIOData)
GCF_IMPLEMENTS(OsgGroupVisNodeIOData)
GCF_END_QUERY_TABLE(OsgGroupVisNodeIOData)

osg::ref_ptr<osg::Group> OsgGroupVisNodeIOData::getOsgGroup() const
{
	return m_osgGroup;
}