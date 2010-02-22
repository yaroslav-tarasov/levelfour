#ifndef COMPASSAXIS_H
#define COMPASSAXIS_H

// OSG Includes
#include <osg/Group>
#include <osg/Geode>
#include <osg/LineWidth>
#include <osg/Geometry>
#include <osg/Camera>
#include <osg/Quat>
#include <osg/MatrixTransform>
#include <osg/Referenced>
#include <osg/NodeCallback>
#include <osgGA/TrackballManipulator>
#include <osgViewer/Viewer>

class ViewerQOSG;

class CompassAxis : public osg::Group
{
public:
	// Public Constructor
	explicit CompassAxis(ViewerQOSG * viewer);
	// Public Destructor
	virtual ~CompassAxis();

	void updateCurrentRotation();

private:
	osg::Geometry *createAxes();

	// Prevent Copy & Assignment
	CompassAxis(const CompassAxis &);
	CompassAxis &operator=(const CompassAxis &);

	osg::ref_ptr<osg::MatrixTransform> m_rotation; 
	ViewerQOSG * _viewer;
};


class AxesNodeCallback : public osg::NodeCallback 
{
public:
	virtual void operator()(osg::Node* node, osg::NodeVisitor* nv)
	{
		osg::ref_ptr<CompassAxis> axes = 
		dynamic_cast<CompassAxis*> (node);
		if(axes)
		{
			axes->updateCurrentRotation();
		}
		traverse(node, nv); 
	}
}; 

#endif