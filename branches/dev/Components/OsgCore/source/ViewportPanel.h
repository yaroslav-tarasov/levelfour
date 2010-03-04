#ifndef VIEWPORT_PANEL_H
#define VIEWPORT_PANEL_H

#include <QTabWidget>
#include <QWidget>
#include <QMap>
#include <QSplitter>
#include <QVBoxLayout>
#include <QToolBar>
#include <QComboBox>
#include <QAction>
#include <QStackedWidget>
#include <osgViewer/View>
#include "CompassAxis.h"

class ViewportsSplitter;
class CompassAxis;

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

class ViewportPanel : public QWidget
{
	Q_OBJECT
public:
	ViewportPanel(ViewportsSplitter * splitter = 0);
	~ViewportPanel();

	void addScene(osg::Group * sceneRoot, QString name);
	void removeScene(QString name, bool last = false);
	
	void setSplitter(ViewportsSplitter * splitterContainer);
	void updateIndexLayout();

public slots:
	void toggleXYGrid();
	void toggleAxes();
	void setSelectedScene(int idex);
	void setViewport(int index);

private:
	QMap<QString, ViewWidget*> viewMap;

	QVBoxLayout sceneLayout;
	QStackedWidget sceneStack;
	QToolBar sceneToolBar;
	QComboBox sceneSelection;
	QComboBox sceneCameras;
	QComboBox sceneDisplays;
	QComboBox sceneViewports;

	ViewportsSplitter * splitter;

	// Actions for toolbar
	QAction * showgridAction;
	QAction * showgizmoAction;
	QAction * showaxisAction;
	QAction * saveviewAction;
};

#endif