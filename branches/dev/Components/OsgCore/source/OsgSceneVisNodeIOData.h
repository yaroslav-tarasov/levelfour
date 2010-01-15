#ifndef OSG_SCENE_VISNODE_IODATA_H
#define OSG_SCENE_VISNODE_IODATA_H

#include "IVisSystemNodeIOData.h"
#include "QOSGGraphics.h"

class OsgSceneVisNodeIOData : virtual public IVisSystemNodeIOData
{
public:
	OsgSceneVisNodeIOData();
	~OsgSceneVisNodeIOData();

	void setOsgScene(osg::QOSGScene * scene);

	void * dataPointer();
	QString dataTypeName();

	bool queryInterface(QString iface, void** ifacePtr);
	bool destroySelf();

	osg::QOSGScene* getOsgScene() const;

private:
	osg::QOSGScene* m_osgScene;
};

#endif