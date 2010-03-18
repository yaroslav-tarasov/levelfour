#ifndef OSG_DRAWABLE_VISNODE_IODATA_H
#define OSG_DRAWABLE_VISNODE_IODATA_H

#include "IVisSystemNodeIOData.h"
#include <osg/Drawable>

class OsgDrawableVisNodeIOData : virtual public IVisSystemNodeIOData
{
public:
	OsgDrawableVisNodeIOData ();
	~OsgDrawableVisNodeIOData ();

	void setDrawable(osg::Drawable * drawable);

	void * dataPointer();
	QString dataTypeName();

	bool queryInterface(QString iface, void** ifacePtr);
	bool destroySelf();

	osg::Drawable * getOsgDrawable() const;

private:
	osg::Drawable * m_osgDrawable;
};

#endif