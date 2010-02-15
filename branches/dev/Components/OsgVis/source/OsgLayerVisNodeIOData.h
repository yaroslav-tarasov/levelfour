#ifndef OSG_LAYER_VISNODE_IODATA_H
#define OSG_LAYER_VISNODE_IODATA_H

#include "IVisSystemNodeIOData.h"
#include <osgEarth/MapLayer>

class OsgLayerVisNodeIOData : virtual public IVisSystemNodeIOData
{
public:
	OsgLayerVisNodeIOData();
	~OsgLayerVisNodeIOData();

	void setLayer(osgEarth::MapLayer * layer);

	void * dataPointer();
	QString dataTypeName();

	bool queryInterface(QString iface, void** ifacePtr);
	bool destroySelf();

	osgEarth::MapLayer * getLayer() const;

protected:
	osgEarth::MapLayer * m_layer;
};

#endif