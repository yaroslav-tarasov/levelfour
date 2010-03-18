#include "ModelVisNodeIOData.h"

ModelVisNodeIOData::ModelVisNodeIOData()
{}

ModelVisNodeIOData::~ModelVisNodeIOData()
{}

void ModelVisNodeIOData::setModel(osgEarth::ModelLayer * model)
{
	m_model = model;
}

osgEarth::ModelLayer * ModelVisNodeIOData::getModel() const
{
	return m_model;
}

void * ModelVisNodeIOData::dataPointer()
{
	return m_model;
}

QString ModelVisNodeIOData::dataTypeName()
{
	return "osg::ref_ptr<osgEarth::ModelLayer>";
}

GCF_BEGIN_QUERY_TABLE(ModelVisNodeIOData)
GCF_IMPLEMENTS(ModelVisNodeIOData)
GCF_END_QUERY_TABLE(ModelVisNodeIOData)
