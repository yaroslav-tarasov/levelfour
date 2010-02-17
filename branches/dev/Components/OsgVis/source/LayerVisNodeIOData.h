#ifndef LAYER_VISNODE_IODATA_H
#define LAYER_VISNODE_IODATA_H

#include "IVisSystemNodeIOData.h"
#include <osgEarth/MapLayer>

class LayerVisNodeIOData : virtual public IVisSystemNodeIOData
{
public:
	LayerVisNodeIOData();
	~LayerVisNodeIOData();

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