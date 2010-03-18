#include "LayerVisNodeIOData.h"

LayerVisNodeIOData::LayerVisNodeIOData()
{}

LayerVisNodeIOData::~LayerVisNodeIOData()
{}

void LayerVisNodeIOData::setLayer(osgEarth::MapLayer* layer)
{
	m_layer = layer;
}

osgEarth::MapLayer* LayerVisNodeIOData::getLayer() const
{
	return m_layer;
}

void * LayerVisNodeIOData::dataPointer()
{
	return m_layer;
}

QString LayerVisNodeIOData::dataTypeName()
{
	return "osg::ref_ptr<osgEarth::MapLayer>";
}

GCF_BEGIN_QUERY_TABLE(LayerVisNodeIOData)
GCF_IMPLEMENTS(LayerVisNodeIOData)
GCF_END_QUERY_TABLE(LayerVisNodeIOData)
