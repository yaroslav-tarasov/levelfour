#include "OsgNodeVisNodeIOData.h"

OsgNodeVisNodeIOData::OsgNodeVisNodeIOData()
{
}

OsgNodeVisNodeIOData::~OsgNodeVisNodeIOData()
{
}

void OsgNodeVisNodeIOData::setOsgNode(osg::Node * node)
{
	m_osgNode = node;
}

void * OsgNodeVisNodeIOData::dataPointer()
{
	return m_osgNode;
}

QString OsgNodeVisNodeIOData::dataTypeName()
{
	return "osg::ref_ptr<osg::Node>";
}

GCF_BEGIN_QUERY_TABLE(OsgNodeVisNodeIOData)
GCF_IMPLEMENTS(OsgNodeVisNodeIOData)
GCF_END_QUERY_TABLE(OsgNodeVisNodeIOData)

osg::Node * OsgNodeVisNodeIOData::getOsgNode() const
{
	return m_osgNode;
}