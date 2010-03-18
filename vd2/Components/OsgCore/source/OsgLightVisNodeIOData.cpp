#include "OsgLightVisNodeIOData.h"

OsgLightVisNodeIOData::OsgLightVisNodeIOData()
{
	m_osgLight = new osg::Light;
}

OsgLightVisNodeIOData::~OsgLightVisNodeIOData()
{
}

void OsgLightVisNodeIOData::setOsgLight(osg::ref_ptr<osg::Light> light)
{
	m_osgLight = light;
}

void * OsgLightVisNodeIOData::dataPointer()
{
	return m_osgLight;
}

QString OsgLightVisNodeIOData::dataTypeName()
{
	return "osg::ref_ptr<osg::Light>";
}

GCF_BEGIN_QUERY_TABLE(OsgLightVisNodeIOData)
GCF_IMPLEMENTS(OsgLightVisNodeIOData)
GCF_END_QUERY_TABLE(OsgLightVisNodeIOData)

osg::ref_ptr<osg::Light> OsgLightVisNodeIOData::getOsgLight() const
{
	return m_osgLight;
}