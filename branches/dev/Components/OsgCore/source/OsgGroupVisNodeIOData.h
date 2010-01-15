#ifndef OSG_GROUP_VISNODE_IODATA_H
#define OSG_GROUP_VISNODE_IODATA_H

#include "IVisSystemNodeIOData.h"
#include <osg/Group>
#include <osg/ref_ptr>

class OsgGroupVisNodeIOData : virtual public IVisSystemNodeIOData
{
public:
	OsgGroupVisNodeIOData();
	~OsgGroupVisNodeIOData();

	void addOsgNode(osg::ref_ptr<osg::Node> node);
	void setOsgGroup(osg::ref_ptr<osg::Group> group);

	void * dataPointer();
	QString dataTypeName();

	bool queryInterface(QString iface, void** ifacePtr);
	bool destroySelf();

	osg::ref_ptr<osg::Group> getOsgGroup() const;

private:
	osg::ref_ptr<osg::Group> m_osgGroup;
};

#endif