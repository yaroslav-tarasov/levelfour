#include <osg/ref_ptr>
#include "vtkUpdateCallback.h"
#include "vtkActorToOSG.h"

osg::ref_ptr<osg::Geode> vtkUpdateCallback::getGeode() const
{
	return m_geode;
}

void vtkUpdateCallback::Execute(vtkObject *caller, unsigned long eventId, void* arguments)
{
	m_actor = dynamic_cast<vtkActor*> (caller);
	if (m_actor)
		m_geode = vtkActorToOSG(m_actor);
}