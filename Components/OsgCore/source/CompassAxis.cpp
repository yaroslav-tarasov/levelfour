#include "CompassAxis.h"
#include "QOSGWidget.h"

CompassAxis::CompassAxis(ViewerQOSG * viewer)
: _viewer(viewer)
{
	osg::Camera *camera = new osg::Camera;
	camera->setProjectionMatrix(osg::Matrix::ortho(0.f, 1280, 0.f, 1024, -200.0, -200.0));
	camera->setReferenceFrame(osg::Transform::ABSOLUTE_RF);
	camera->setViewMatrix(osg::Matrix::identity());
	camera->setClearMask(GL_DEPTH_BUFFER_BIT);
	camera->setRenderOrder(osg::Camera::POST_RENDER);
	camera->setAllowEventFocus(false);


	osg::Geode* geode = new osg::Geode;


	// turn lighting off for the text and disable depth test to ensure it's always ontop.
	osg::StateSet* stateset = geode->getOrCreateStateSet();
	stateset->setMode(GL_LIGHTING,osg::StateAttribute::OFF);

	geode->addDrawable(createAxes());

	m_rotation = new osg::MatrixTransform();
	m_rotation->setMatrix(osg::Matrix::identity());
	m_rotation->addChild(geode);

	osg::MatrixTransform *scale = new osg::MatrixTransform();
	scale->setMatrix(osg::Matrix::scale(osg::Vec3(10.0f, 10.0f, 10.0f)));
	scale->addChild(m_rotation.get());

	osg::MatrixTransform *translate = new osg::MatrixTransform();
	
	translate->setMatrix(osg::Matrix::translate(osg::Vec3(_viewer->width()+150, _viewer->height()+150, 0.0f)));
	translate->addChild(scale);

	camera->addChild(translate);
	addChild(camera);


	setUpdateCallback(new AxesNodeCallback);
}

CompassAxis::~CompassAxis()
{
}

osg::Geometry * CompassAxis::createAxes()
{
	osg::Geometry* geom = new osg::Geometry;

	osg::Vec3Array* vertices = new osg::Vec3Array;
	vertices->push_back(osg::Vec3(0.0f, 0.0f, 0.0f));
	vertices->push_back(osg::Vec3(10.5f, 0.0f, 0.0f));
	vertices->push_back(osg::Vec3(0.0f, 0.0f, 0.0f));
	vertices->push_back(osg::Vec3(0.0f, 10.5f, 0.0f));
	vertices->push_back(osg::Vec3(0.0f, 0.0f, 0.0f));
	vertices->push_back(osg::Vec3(0.0f, 0.0f, 15.5f));
	geom->setVertexArray(vertices);

	osg::Vec3Array* normals = new osg::Vec3Array;
	normals->push_back(osg::Vec3(0.0f,0.0f,1.0f));
	geom->setNormalArray(normals);
	geom->setNormalBinding(osg::Geometry::BIND_OVERALL);

	osg::Vec4Array* colors = new osg::Vec4Array;
	colors->push_back(osg::Vec4(1.0f,0.0,0.0f,1.0f));
	colors->push_back(osg::Vec4(0.0f,1.0,0.0f,1.0f));
	colors->push_back(osg::Vec4(0.0f,0.0,1.0f,1.0f));
	geom->setColorArray(colors);
	geom->setColorBinding(osg::Geometry::BIND_PER_PRIMITIVE);

	geom->addPrimitiveSet(new osg::DrawArrays(GL_LINES,0,6));

	osg::StateSet* stateset = geom->getOrCreateStateSet();
	osg::LineWidth *lw = new osg::LineWidth(2.0f);
	stateset->setAttribute(lw);

	return geom;
}


void CompassAxis::updateCurrentRotation()
{
	osg::Matrix mat(_viewer->getCameraManipulator()->getInverseMatrix());
	m_rotation->setMatrix(mat);
} 