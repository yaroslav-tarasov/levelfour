#ifndef OSG_GEODEFROMACTOR_VISNODE_IODATA_H
#define OSG_GEODEFROMACTOR_VISNODE_IODATA_H

#include "IVisSystemNodeIOData.h"
#include "QOSGGraphics.h"
#include <osg/Geode>
#include <osg/ref_ptr>

class OsgGeodeFromActorVisNodeIOData : virtual public IVisSystemNodeIOData
{
public:
	OsgGeodeFromActorVisNodeIOData();
	~OsgGeodeFromActorVisNodeIOData();

	void setOsgGeode(osg::ref_ptr<osg::Geode> geode);

	void * dataPointer();
	QString dataTypeName();

	bool queryInterface(QString iface, void** ifacePtr);
	bool destroySelf();

	osg::ref_ptr<osg::Geode> getOsgGeode() const;

private:
	osg::ref_ptr<osg::Geode> m_osgGeode;
};

#endif