#ifndef OSG_PAT_VISNODE_IODATA_H
#define OSG_PAT_VISNODE_IODATA_H

#include "IVisSystemNodeIOData.h"
#include "OsgNodeVisNodeIOData.h"
#include <osg/PositionAttitudeTransform>

class OsgPATVisNodeIOData : public OsgNodeVisNodeIOData
{
public:
	OsgPATVisNodeIOData();
	~OsgPATVisNodeIOData();

	void addOsgNode(osg::ref_ptr<osg::Node> node);
};

#endif