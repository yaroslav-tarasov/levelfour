#include "ViewportPanel.h"
#include "ViewportsSplitter.h"

ViewportPanel::ViewportPanel(ViewportsSplitter * splitterContainer) 
	: QWidget(), _scene(0), splitter(splitterContainer)
{
	// Camera actions (these are provided by the combo box)
	sceneCameras.addItem("Perspective");
	sceneCameras.addItem("Left");
	sceneCameras.addItem("Right");
	sceneCameras.addItem("Front");
	sceneCameras.addItem("Back");
	sceneCameras.addItem("Top");
	sceneCameras.addItem("Bottom");
	sceneCameras.addItem("Isometric");

	// Display actions (these are provided by the combo box)
	sceneDisplays.addItem(QIcon(":/OsgCore/wireframeDisplay.png"), tr("Wireframe"));
	sceneDisplays.addItem(QIcon(":/OsgCore/textureDisplay.png"), tr("Textures"));
	sceneDisplays.addItem(QIcon(":/OsgCore/backfacecullingDisplay.png"), tr("Backface Culling"));
	sceneDisplays.addItem(QIcon(":/OsgCore/shadingDisplay.png"), tr("Shading"));

	// Viewport actions (these are provided by the combo box)
	sceneViewports.addItem(QIcon(":/MainWindow/single.png"), tr("Single"));
	sceneViewports.addItem(QIcon(":/MainWindow/overunder.png"), tr("Over Under"));
	sceneViewports.addItem(QIcon(":/MainWindow/sideside.png"), tr("Side by Side"));
	sceneViewports.addItem(QIcon(":/MainWindow/2u1d.png"), tr("2 Up 1 Down"));
	sceneViewports.addItem(QIcon(":/MainWindow/1u2d.png"), tr("1 Up 2 Down"));
	sceneViewports.addItem(QIcon(":/MainWindow/quad.png"), tr("Quad"));

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
	
	// Add widgets and actions to the toolbar
	sceneToolBar.addWidget(&sceneSelection);
	sceneToolBar.addWidget(&sceneCameras);
	sceneToolBar.addWidget(&sceneDisplays);
	sceneToolBar.addWidget(&sceneViewports);

	sceneToolBar.addAction(showgridAction);
	sceneToolBar.addAction(showgizmoAction);
	sceneToolBar.addAction(showaxisAction);
	sceneToolBar.addAction(saveviewAction);

	// Assemble the panel
	sceneLayout.addWidget(&sceneToolBar);
	sceneLayout.addWidget(&sceneStack);
	// simple viewer will add widgets to scene stack as needed;
	setLayout(&sceneLayout);


	// Toolbar actions
	// Scene selection provides toggle between different scene and data view nodes
	// sceneSelection.addItem("Select Scene"); This should be a string passed as scene name given in the node
	// The combo box selects scenes by index
	connect(&sceneSelection, SIGNAL(activated(int)), this, SLOT(setSelectedScene(int)));     
	connect(&sceneViewports, SIGNAL(activated(int)), this, SLOT(setViewport(int)));     

	connect(showgridAction, SIGNAL(triggered()), this, SLOT(toggleXYGrid()));
	connect(showaxisAction, SIGNAL(triggered()), this, SLOT(toggleAxes()));
//	connect(showgizmoAction, SIGNAL(triggered()), this, SLOT(toggleGizmo()));
}

ViewportPanel::~ViewportPanel()
{
}

void ViewportPanel::addSceneStack(QWidget * widget)
{
	sceneStack.addWidget(widget);
}

void ViewportPanel::addSceneItem(QString name)
{
	sceneSelection.addItem(name);
}

void ViewportPanel::removeSceneStack(QWidget * widget)
{
	sceneStack.removeWidget(widget);
}

void ViewportPanel::removeSceneItem(QString name)
{
	int index = sceneSelection.findText(name);
	if (index >= 0)
		sceneSelection.removeItem(index);
}

void ViewportPanel::toggleXYGrid()
{
	if (!_scene)
	{
		QOSGContainer * sceneContainer = static_cast<QOSGContainer*>(sceneStack.currentWidget());
		if (sceneContainer)
			_scene = sceneContainer->getScene();
	}
	if (_scene)
	{
		xyGridToggled = !xyGridToggled;
		_scene->toggleXYGrid(xyGridToggled);
	}
}

void ViewportPanel::toggleAxes()
{
	if (!_scene)
	{
		QOSGContainer * sceneContainer = static_cast<QOSGContainer*>(sceneStack.currentWidget());
		if (sceneContainer)
			_scene = sceneContainer->getScene();
	}

	if (_scene)
	{
		axesToggled = !axesToggled;
		_scene->toggleAxes(axesToggled);
	}

}

void ViewportPanel::setSelectedScene(int index)
{
	sceneStack.setCurrentIndex(index);
	QOSGContainer * sceneContainer = static_cast<QOSGContainer*>(sceneStack.currentWidget());
	if (sceneContainer)
		_scene = sceneContainer->getScene();
}

void ViewportPanel::setViewport(int index)
{
	if (splitter)
		splitter->setViewportsLayout(this, index);
}

void ViewportPanel::setSplitter(ViewportsSplitter * splitterContainer)
{
	this->splitter = splitterContainer;
}
