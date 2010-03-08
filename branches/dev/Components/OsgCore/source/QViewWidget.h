#ifndef QVIEWWIDGET_H
#define QVIEWWIDGET_H

#include <QTabWidget>
#include <QWidget>
#include <QMap>
#include <QToolBar>
#include <QComboBox>
#include <QAction>
#include <QStackedWidget>

#include <osgViewer/View>
#include <osg/PolygonMode>
#include <osg/CullFace>
#include <osg/ShadeModel>

#include <math.h>

#include "CompassAxis.h"
#include "ViewportsSplitter.h"
#include "GraphicsWindowQt.h"

class ViewportsSplitter;
class CompassAxis;

#define PERSPECTIVE 0
#define LEFT 1
#define RIGHT 2
#define FRONT 3
#define BACK 4
#define TOP 5
#define BOTTOM 6
#define ISOMETRIC 7

#define WIREFRAME 0
#define POLYMODE_FILL 1
#define BACK_CULL 2
#define SHADING 3

/**
 * The number of lines in each direction.
 */
const int GRID_LINE_COUNT = 49;

/**
 * The amount of space between each line.
 */
const float GRID_LINE_SPACING = 1.0f;

struct ViewWidget {
public:
	ViewWidget(QWidget * widget, osgViewer::View * view, osg::Group * sceneRoot, bool showGrids = true, bool showAxes = true);
	~ViewWidget();

	QWidget * getWidget() const {return _widget;}
	osgViewer::View * getView() const {return _view;}
	osg::Group * getRoot() const {return _sceneRoot;}

	bool valid() const {return _sceneRoot != 0;}

	CompassAxis * getAxes() const {return compassAxes;}
	bool showGrids() const {return _showGrids;}
	bool showAxes() const {return _showAxes;}

	void toggleXYGrid();
	void toggleXZGrid();
	void toggleYZGrid();
	void toggleAxes();

	osg::MatrixTransform* getXYGrid() const {return mpXYGridTransform;}
	osg::MatrixTransform* getXZGrid() const {return mpXZGridTransform;}
	osg::MatrixTransform* getYZGrid() const {return mpYZGridTransform;}

private:
	QWidget * _widget;
	osgViewer::View * _view;
	osg::Group * _sceneRoot;

	bool xyGridToggled;
	bool xzGridToggled;
	bool yzGridToggled;
	bool axesToggled;

	void initGrids();
	bool _showGrids;
	void initAxes();
	bool _showAxes;

	osg::MatrixTransform* mpXYGridTransform;
	osg::MatrixTransform* mpXZGridTransform;
	osg::MatrixTransform* mpYZGridTransform;
	CompassAxis * compassAxes;
};

class QViewWidget : QObject
{
	Q_OBJECT

public:
	QViewWidget(ViewportsSplitter * compositeViewer);
	~QViewWidget();

	void addScene(osg::Group * sceneRoot, QString name);
	void removeScene(QString name, bool last = false);

	QStackedWidget * getCurrentWidget() const;
	QToolBar * getCurrentToolBar() const;

	void setSplitter(ViewportsSplitter * compositeViewer);

public slots:
	// scene view slots
	void setSelectedScene(int index);
	void setSelectedCamera(int index);
	void setSelectedDisplay(int index);
	void toggleXYGrid();
	void toggleAxes();

private:
	QMap<QString, ViewWidget*> viewMap;
	QComboBox sceneSelection;
	QStackedWidget * sceneStack;
	QToolBar * sceneToolBar;
	QComboBox sceneCameras;
	QComboBox sceneDisplays;
	QAction * showgridAction;
	QAction * showgizmoAction;
	QAction * showaxisAction;
	QAction * saveviewAction;

	ViewportsSplitter * splitter;
};

#endif