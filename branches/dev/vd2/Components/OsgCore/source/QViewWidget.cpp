#include "QViewWidget.h"

QViewWidget::QViewWidget(ViewportsSplitter * splitterContainer)
	: splitter(splitterContainer)
{
	//// init widgets specific to the "view" type
	// Camera actions (these are provided by the combo box)
	sceneCameras.addItem("Perspective");
	sceneCameras.addItem("Left");
	sceneCameras.addItem("Right");
	sceneCameras.addItem("Front");
	sceneCameras.addItem("Back");
	sceneCameras.addItem("Top");
	sceneCameras.addItem("Bottom");
	sceneCameras.addItem("Isometric");
	sceneCameras.setCurrentIndex(TOP);

	// Display actions (these are provided by the combo box)
	sceneDisplays.addItem(QIcon(":/OsgCore/wireframeDisplay.png"), "Wireframe");
	sceneDisplays.addItem(QIcon(":/OsgCore/textureDisplay.png"), "Fill");
	sceneDisplays.addItem(QIcon(":/OsgCore/backfacecullingDisplay.png"), "Backface Culling");
	sceneDisplays.addItem(QIcon(":/OsgCore/shadingDisplay.png"), "Shading");
	sceneDisplays.setCurrentIndex(POLYMODE_FILL);

	// HUD actions
	showgridAction = new QAction(QIcon(":/OsgCore/grid.png"), tr("&Display Grid"), this);
	showgridAction->setStatusTip(tr("Displays a grid"));
	showgizmoAction = new QAction(QIcon(":/OsgCore/gizmo.png"), tr("&Display Gizmo"), this);
	showgizmoAction->setStatusTip(tr("Displays a gizmo control for navigation"));
	showaxisAction = new QAction(QIcon(":/OsgCore/axis.png"), tr("&Display Axis"), this);
	showaxisAction->setStatusTip(tr("Displays an axis for orientation"));

	// Save view options
	saveviewAction = new QAction(QIcon(":/LevelFour/camera.png"), tr("&Save view as..."), this);
	saveviewAction->setStatusTip(tr("Provides options for saving view in raster and vector formats"));
	
	sceneToolBar = new QToolBar;
	
	// Add widgets and actions to the toolbar
	sceneToolBar->addWidget(&sceneSelection);
	sceneToolBar->addSeparator();
	sceneToolBar->addWidget(&sceneCameras);
	sceneToolBar->addSeparator();
	sceneToolBar->addWidget(&sceneDisplays);
	sceneToolBar->addSeparator();

	sceneToolBar->addAction(showgridAction);
	sceneToolBar->addAction(showgizmoAction);
	sceneToolBar->addAction(showaxisAction);
	sceneToolBar->addAction(saveviewAction);
	
	// Scene view actions
	connect(&sceneSelection, SIGNAL(activated(int)), this, SLOT(setSelectedScene(int)));     
	connect(&sceneCameras, SIGNAL(activated(int)), this, SLOT(setSelectedCamera(int)));     
	connect(&sceneDisplays, SIGNAL(activated(int)), this, SLOT(setSelectedDisplay(int)));     
	connect(showgridAction, SIGNAL(triggered()), this, SLOT(toggleXYGrid()));
	connect(showaxisAction, SIGNAL(triggered()), this, SLOT(toggleAxes()));
//	connect(showgizmoAction, SIGNAL(triggered()), this, SLOT(toggleGizmo()));

	sceneStack = new QStackedWidget;
}

QViewWidget::~QViewWidget()
{
	delete showgridAction;
	delete showgizmoAction;
	delete showaxisAction;
	delete saveviewAction;
	delete saveviewAction;
	delete sceneToolBar;
	delete sceneStack;
}

QStackedWidget * QViewWidget::getCurrentWidget() const
{
	return sceneStack;
}

QToolBar * QViewWidget::getCurrentToolBar() const
{
	return sceneToolBar;
}

void QViewWidget::addScene(osg::Group * sceneRoot, QString name)
{
	bool showGrid = true;
	bool showAxes = true;
	if (name.toStdString() == "dummy")
		showGrid = showAxes = false;

	ViewWidget * viewWidget = splitter->
		addViewWidget(splitter->createCamera(0,0,100,100), sceneRoot, showGrid, showAxes);

	if (viewWidget)
	{
		if (name.toStdString() != "dummy")
			sceneSelection.addItem(name);
		
		sceneStack->addWidget(viewWidget->getWidget());
		if (viewMap.count() == 1)
			sceneStack->setCurrentWidget(viewWidget->getWidget());

		viewMap.insert(name, viewWidget);
	}
}

void QViewWidget::removeScene(QString name, bool last)
{
	// remove the name from the selection combobox
	int index = sceneSelection.findText(name);
	if (index >= 0)
		sceneSelection.removeItem(index);

	if (viewMap.contains(name))
	{
		// remove the related view from the composite viewer
		splitter->removeView(viewMap.value(name)->getView());

		// remove the widget from the stack
		sceneStack->removeWidget(viewMap.value(name)->getWidget());

		viewMap.remove(name);
	}
}

void QViewWidget::toggleXYGrid()
{
	if (viewMap.contains(sceneSelection.currentText()))
		viewMap.value(sceneSelection.currentText())->toggleXYGrid();
}

void QViewWidget::toggleAxes()
{
	if (viewMap.contains(sceneSelection.currentText()))
		viewMap.value(sceneSelection.currentText())->toggleAxes();
}

void QViewWidget::setSelectedScene(int index)
{
	ViewWidget * viewWidget = viewMap.value(sceneSelection.itemText(index));
	sceneStack->setCurrentWidget(viewWidget->getWidget());
	viewWidget->getView()->requestRedraw();
}

void QViewWidget::setSelectedCamera(int index)
{
	if (!viewMap.contains(sceneSelection.currentText()) || (index > ISOMETRIC && index < PERSPECTIVE))
		return;

	ViewWidget * viewWidget = viewMap.value(sceneSelection.currentText());
	osg::Vec3d cameraVector = viewWidget->getView()->getCameraManipulator()->getMatrix().getTrans();
	double distance = pow((pow(cameraVector.x(),2) + pow(cameraVector.y(),2) + pow(cameraVector.z(),2)), 0.5);
	osg::Vec3d *eye = 0, center(0,0,0), up(0,0,1);

	switch (index)
	{
	case PERSPECTIVE:
	case ISOMETRIC:
		eye = new osg::Vec3d(0,distance,distance);
		break;
	case LEFT:
		eye = new osg::Vec3d(-distance,0,0);
		break;
	case RIGHT:
		eye = new osg::Vec3d(distance,0,0);
		break;
	case FRONT:
		eye = new osg::Vec3d(0,distance,0);
		break;
	case BACK:
		eye = new osg::Vec3d(0,-distance,0);
		break;
	case TOP:
		eye = new osg::Vec3d(0,0,distance);
		break;
	case BOTTOM:
		eye = new osg::Vec3d(0,0,-distance);
		break;
	}

	// if the change concerned the eye position, apply it
	if (eye)
	{
		viewWidget->getView()->getCameraManipulator()->setHomePosition(*eye,center,up);
		viewWidget->getView()->home();
		delete eye;
	}
}

void QViewWidget::setSelectedDisplay(int index)
{
	if (!viewMap.contains(sceneSelection.currentText()) || (index > ISOMETRIC && index < PERSPECTIVE))
		return;

	ViewWidget * viewWidget = viewMap.value(sceneSelection.currentText());
	osg::StateSet * state = viewWidget->getRoot()->getOrCreateStateSet();
	osg::CullFace* cf;
	osg::PolygonMode *polyModeObj;
	osg::ShadeModel* sm;

	switch (index)
	{
	case WIREFRAME:
		polyModeObj = dynamic_cast<osg::PolygonMode*>(state->getAttribute(osg::StateAttribute::POLYGONMODE));
		if (!polyModeObj) 
		{
			polyModeObj = new osg::PolygonMode;
			state->setAttributeAndModes(polyModeObj,
									osg::StateAttribute::ON | osg::StateAttribute::OVERRIDE);
		}
		polyModeObj->setMode(osg::PolygonMode::FRONT_AND_BACK, osg::PolygonMode::LINE);
		break;
	case POLYMODE_FILL:
		polyModeObj = dynamic_cast<osg::PolygonMode*>(state->getAttribute(osg::StateAttribute::POLYGONMODE));
		if (polyModeObj) 
			polyModeObj->setMode(osg::PolygonMode::FRONT_AND_BACK, osg::PolygonMode::FILL);
		break;
	case BACK_CULL:
		cf = dynamic_cast<osg::CullFace*>(state->getAttribute(osg::StateAttribute::CULLFACE));
		if (!cf)
			cf = new osg::CullFace; 
		state->setAttributeAndModes(cf, osg::StateAttribute::ON | osg::StateAttribute::OVERRIDE);
		break;
	case SHADING:
		sm = new osg::ShadeModel;
		sm->setMode(osg::ShadeModel::FLAT);
		state->setAttributeAndModes(sm, osg::StateAttribute::ON | osg::StateAttribute::OVERRIDE);
		break;
	}
}


void QViewWidget::setSplitter(ViewportsSplitter * splitterContainer)
{
	this->splitter = splitterContainer;
}

//// VIEW WIDGET

ViewWidget::ViewWidget(QWidget * widget, osgViewer::View * view, osg::Group * sceneRoot, bool showGrids, bool showAxes)
		:	_widget(widget), 
			_view(view),
			compassAxes(0),
			mpXYGridTransform(0),
			mpXZGridTransform(0),
			mpYZGridTransform(0)
{
	if (!view)
		return;

	_sceneRoot = static_cast<osg::Group*>(_view->getSceneData());
	_showGrids = showGrids;
	_showAxes = showAxes;
	if (_showGrids) 
	{
		initGrids();
		if (mpXYGridTransform && sceneRoot)
			_sceneRoot->addChild(getXYGrid());
	}
	if (_showAxes)
	{
		initAxes();
		if (compassAxes && sceneRoot)
			_sceneRoot->addChild(getAxes());
	}

	xyGridToggled = showGrids;
	axesToggled = showAxes;
}

ViewWidget::~ViewWidget()
{
	if (showAxes() && _sceneRoot)
		_sceneRoot->removeChild(getAxes());
	if (showGrids() && _sceneRoot)
		_sceneRoot->removeChild(getXYGrid());
	delete _widget;
	delete compassAxes;
}


void ViewWidget::initAxes()
{
	compassAxes = new CompassAxis(this);
}

void ViewWidget::initGrids()
{
   const int numVertices = 2 * 2 * GRID_LINE_COUNT;
   osg::Vec3 vertices[numVertices];
   float length = (GRID_LINE_COUNT - 1) * GRID_LINE_SPACING;
   int ptr = 0;

   for(int i = 0; i < GRID_LINE_COUNT; ++i)
   {
      vertices[ptr++].set(-length / 2 + i * GRID_LINE_SPACING, length / 2, 0.0f);
      vertices[ptr++].set(-length / 2 + i * GRID_LINE_SPACING, -length / 2, 0.0f);
   }

   for (int i = 0; i < GRID_LINE_COUNT; ++i)
   {
      vertices[ptr++].set(length / 2, -length / 2 + i * GRID_LINE_SPACING, 0.0f);
      vertices[ptr++].set(-length / 2, -length / 2 + i * GRID_LINE_SPACING, 0.0f);
   }

   osg::ref_ptr<osg::Vec4Array> colors (new osg::Vec4Array());
   colors->push_back (osg::Vec4 (0.3, 0.3, 0.3, 1.0));

   osg::Geometry* geometry = new osg::Geometry;
   geometry->setVertexArray(new osg::Vec3Array(numVertices, vertices));
   geometry->addPrimitiveSet(new osg::DrawArrays(osg::PrimitiveSet::LINES, 0, numVertices));
   geometry->setColorArray (colors.get());
   geometry->setColorBinding (osg::Geometry::BIND_PER_PRIMITIVE_SET);

   osg::Geode* geode = new osg::Geode;
   geode->addDrawable(geometry);
   geode->getOrCreateStateSet()->setMode(GL_LIGHTING, 0);

   mpXYGridTransform = new osg::MatrixTransform;
   mpXYGridTransform->addChild(geode);

   mpXZGridTransform = new osg::MatrixTransform;
   mpXZGridTransform->setMatrix(osg::Matrix::rotate(osg::PI_2, 1, 0, 0));

   mpXZGridTransform->addChild(geode);
   mpXZGridTransform->setNodeMask(0x0);

   mpYZGridTransform = new osg::MatrixTransform;
   mpYZGridTransform->setMatrix(osg::Matrix::rotate(osg::PI_2, 0, 1, 0));

   mpYZGridTransform->addChild(geode);
   mpYZGridTransform->setNodeMask(0x0);
}

void ViewWidget::toggleXYGrid()
{
	xyGridToggled = !xyGridToggled;
	if(xyGridToggled)
	{
	  mpXYGridTransform->setNodeMask(0xFFFFFFFF);
	}
	else
	{
	  mpXYGridTransform->setNodeMask(0x0);
	}
}

void ViewWidget::toggleXZGrid()
{
	xzGridToggled = !xzGridToggled;
   if(xzGridToggled)
   {
      mpXZGridTransform->setNodeMask(0xFFFFFFFF);
   }
   else
   {
      mpXZGridTransform->setNodeMask(0x0);
   }
}

void ViewWidget::toggleYZGrid()
{
	yzGridToggled = !yzGridToggled;
   if(yzGridToggled)
   {
      mpYZGridTransform->setNodeMask(0xFFFFFFFF);
   }
   else
   {
      mpYZGridTransform->setNodeMask(0x0);
   }
}

void ViewWidget::toggleAxes()
{
	axesToggled = !axesToggled;
	if(axesToggled)
   {
      compassAxes->setNodeMask(0xFFFFFFFF);
   }
   else
   {
      compassAxes->setNodeMask(0x0);
   }
}

