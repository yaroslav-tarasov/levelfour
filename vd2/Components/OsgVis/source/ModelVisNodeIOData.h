#ifndef MODEL_VISNODE_IODATA_H
#define MODEL_VISNODE_IODATA_H

#include "IVisSystemNodeIOData.h"
#include <osgEarth/ModelLayer>

class ModelVisNodeIOData : virtual public IVisSystemNodeIOData
{
public:
	ModelVisNodeIOData();
	~ModelVisNodeIOData();

	void setModel(osgEarth::ModelLayer * model);

	void * dataPointer();
	QString dataTypeName();

	bool queryInterface(QString iface, void** ifacePtr);
	bool destroySelf();

	osgEarth::ModelLayer * getModel() const;

protected:
	osgEarth::ModelLayer * m_model;
};

#endif