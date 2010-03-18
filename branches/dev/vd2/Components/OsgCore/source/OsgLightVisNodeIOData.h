#ifndef OSG_LIGHT_VISNODE_IODATA_H
#define OSG_LIGHT_VISNODE_IODATA_H

#include "IVisSystemNodeIOData.h"
#include <osg/Light>
#include <osg/ref_ptr>

class OsgLightVisNodeIOData : virtual public IVisSystemNodeIOData
{
public:
	OsgLightVisNodeIOData();
	~OsgLightVisNodeIOData();

	void setOsgLight(osg::ref_ptr<osg::Light> light);

	void * dataPointer();
	QString dataTypeName();

	bool queryInterface(QString iface, void** ifacePtr);
	bool destroySelf();

	osg::ref_ptr<osg::Light> getOsgLight() const;

private:
	osg::ref_ptr<osg::Light> m_osgLight;
};

#endif