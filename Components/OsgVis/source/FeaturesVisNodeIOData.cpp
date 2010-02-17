#include "FeaturesVisNodeIOData.h"

FeaturesVisNodeIOData::FeaturesVisNodeIOData()
{}

FeaturesVisNodeIOData::~FeaturesVisNodeIOData()
{}

void FeaturesVisNodeIOData::setFeatures(osgEarth::Config * features)
{
	m_features = features;
}

osgEarth::Config* FeaturesVisNodeIOData::getFeatures() const
{
	return m_features;
}

void * FeaturesVisNodeIOData::dataPointer()
{
	return m_features;
}

QString FeaturesVisNodeIOData::dataTypeName()
{
	return "osg::ref_ptr<osgEarth::Config>";
}

GCF_BEGIN_QUERY_TABLE(FeaturesVisNodeIOData)
GCF_IMPLEMENTS(FeaturesVisNodeIOData)
GCF_END_QUERY_TABLE(FeaturesVisNodeIOData)
