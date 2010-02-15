#include "OsgLayerVisNodeIOData.h"

OsgLayerVisNodeIOData::OsgLayerVisNodeIOData()
{}

OsgLayerVisNodeIOData::~OsgLayerVisNodeIOData()
{}

void OsgLayerVisNodeIOData::setLayer(osgEarth::MapLayer* layer)
{
	m_layer = layer;
}

osgEarth::MapLayer* OsgLayerVisNodeIOData::getLayer() const
{
	return m_layer;
}

void * OsgLayerVisNodeIOData::dataPointer()
{
	return m_layer;
}

QString OsgLayerVisNodeIOData::dataTypeName()
{
	return "osg::ref_ptr<osgEarth::MapLayer>";
}

GCF_BEGIN_QUERY_TABLE(OsgLayerVisNodeIOData)
GCF_IMPLEMENTS(OsgLayerVisNodeIOData)
GCF_END_QUERY_TABLE(OsgLayerVisNodeIOData)
