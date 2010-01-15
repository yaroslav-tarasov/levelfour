#ifndef IOSG_SCENE_FROM_ACTOR_IO_DATA_H
#define IOSG_SCENE_FROM_ACTOR_IO_DATA_H

#include "IVisSystemNodeIOData.h"
#include "QOSGGraphics.h"

class IOsgSceneFromActorIOData : virtual public IVisSystemNodeIOData
{
public:
	virtual osg::QOSGScene* getOsgSceneFromActor() = 0;
};

#endif