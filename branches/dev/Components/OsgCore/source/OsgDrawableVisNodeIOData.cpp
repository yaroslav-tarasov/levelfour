#include "OsgDrawableVisNodeIOData.h"

OsgDrawableVisNodeIOData::OsgDrawableVisNodeIOData()
{
}

OsgDrawableVisNodeIOData::~OsgDrawableVisNodeIOData()
{
}

void OsgDrawableVisNodeIOData::setDrawable(osg::Drawable * drawable)
{
	m_osgDrawable = drawable;
}

void * OsgDrawableVisNodeIOData::dataPointer()
{
	return m_osgDrawable;
}

QString OsgDrawableVisNodeIOData::dataTypeName()
{
	return "osg::ref_ptr<osg::Drawable>";
}

GCF_BEGIN_QUERY_TABLE(OsgDrawableVisNodeIOData)
GCF_IMPLEMENTS(OsgDrawableVisNodeIOData)
GCF_END_QUERY_TABLE(OsgDrawableVisNodeIOData)

osg::Drawable * OsgDrawableVisNodeIOData::getOsgDrawable() const
{
	return m_osgDrawable;
}