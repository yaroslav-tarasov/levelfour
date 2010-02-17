#ifndef FEATURES_VISNODE_IODATA_H
#define FEATURES_VISNODE_IODATA_H

#include "IVisSystemNodeIOData.h"
#include <osgEarth/Config>

class FeaturesVisNodeIOData : virtual public IVisSystemNodeIOData
{
public:
	FeaturesVisNodeIOData();
	~FeaturesVisNodeIOData();

	void setFeatures(osgEarth::Config * features);

	void * dataPointer();
	QString dataTypeName();

	bool queryInterface(QString iface, void** ifacePtr);
	bool destroySelf();

	osgEarth::Config * getFeatures() const;

protected:
	osgEarth::Config * m_features;
};

#endif