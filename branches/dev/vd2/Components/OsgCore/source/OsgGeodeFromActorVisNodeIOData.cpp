#include "OsgGeodeFromActorVisNodeIOData.h"

OsgGeodeFromActorVisNodeIOData::OsgGeodeFromActorVisNodeIOData()
{
	m_osgGeode = 0;
}

OsgGeodeFromActorVisNodeIOData::~OsgGeodeFromActorVisNodeIOData()
{
}

void OsgGeodeFromActorVisNodeIOData::setOsgGeode(osg::ref_ptr<osg::Geode> geode)
{
	m_osgGeode = geode;
}

void * OsgGeodeFromActorVisNodeIOData::dataPointer()
{
	return m_osgGeode;
}

QString OsgGeodeFromActorVisNodeIOData::dataTypeName()
{
	return "osg::ref_ptr<osg::Geode>";
}

GCF_BEGIN_QUERY_TABLE(OsgGeodeFromActorVisNodeIOData)
GCF_IMPLEMENTS(OsgGeodeFromActorVisNodeIOData)
GCF_END_QUERY_TABLE(OsgGeodeFromActorVisNodeIOData)

osg::ref_ptr<osg::Geode> OsgGeodeFromActorVisNodeIOData::getOsgGeode() const
{
	return m_osgGeode;
}