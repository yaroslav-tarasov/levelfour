#include "OsgSceneVisNodeIOData.h"

OsgSceneVisNodeIOData::OsgSceneVisNodeIOData()
{
	m_osgScene = 0;
}

OsgSceneVisNodeIOData::~OsgSceneVisNodeIOData()
{
}

void OsgSceneVisNodeIOData::setOsgScene(osg::QOSGScene* scene)
{
	m_osgScene = scene;
}

void * OsgSceneVisNodeIOData::dataPointer()
{
	return m_osgScene;
}

QString OsgSceneVisNodeIOData::dataTypeName()
{
	return "osg::QOSGScene";
}

GCF_BEGIN_QUERY_TABLE(OsgSceneVisNodeIOData)
GCF_IMPLEMENTS(OsgSceneVisNodeIOData)
GCF_END_QUERY_TABLE(OsgSceneVisNodeIOData)

osg::QOSGScene* OsgSceneVisNodeIOData::getOsgScene() const
{
	return m_osgScene;
}