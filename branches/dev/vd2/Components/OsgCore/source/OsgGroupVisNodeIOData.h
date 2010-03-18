#ifndef OSG_GROUP_VISNODE_IODATA_H
#define OSG_GROUP_VISNODE_IODATA_H

#include "IVisSystemNodeIOData.h"
#include "OsgNodeVisNodeIOData.h"
#include <osg/Group>

class OsgGroupVisNodeIOData : public OsgNodeVisNodeIOData
{
public:
	OsgGroupVisNodeIOData();
	~OsgGroupVisNodeIOData();

	void addOsgNode(osg::ref_ptr<osg::Node> node);
};

#endif