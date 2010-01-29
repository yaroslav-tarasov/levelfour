#ifndef OSG_NODE_VISNODE_IODATA_H
#define OSG_NODE_VISNODE_IODATA_H

#include "IVisSystemNodeIOData.h"
#include <osg/Node>
#include <osg/ref_ptr>

class OsgNodeVisNodeIOData : virtual public IVisSystemNodeIOData
{
public:
	OsgNodeVisNodeIOData();
	~OsgNodeVisNodeIOData();

	void setOsgNode(osg::Node * node);

	void * dataPointer();
	QString dataTypeName();

	bool queryInterface(QString iface, void** ifacePtr);
	bool destroySelf();

	osg::Node * getOsgNode() const;

protected:
	osg::Node * m_osgNode;
};

#endif