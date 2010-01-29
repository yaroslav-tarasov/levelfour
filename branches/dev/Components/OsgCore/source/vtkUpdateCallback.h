#include "vtkCommand.h"
#include "vtkObject.h"
#include "vtkActor.h"
#include <osg/ref_ptr>
#include <osg/Geode>

class vtkUpdateCallback : public vtkCommand
{
public:
	static vtkUpdateCallback *New()
	{
		return new vtkUpdateCallback;
	}

	void Delete()
	{
		delete this;
	}

	virtual void Execute(vtkObject *caller, unsigned long eventId, void* arguments);

	osg::ref_ptr<osg::Geode> getGeode() const;

private:
	osg::ref_ptr<osg::Geode> m_geode;
	vtkActor * m_actor;
};
