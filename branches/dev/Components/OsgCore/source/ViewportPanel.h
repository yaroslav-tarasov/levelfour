#ifndef VIEWPORT_PANEL_H
#define VIEWPORT_PANEL_H

#include <QTabWidget>
#include "QOSGWidget.h"

#include <QWidget>
#include <QVBoxLayout>
#include <QToolBar>
#include <QComboBox>
#include <QAction>
#include <QStackedWidget>

class ViewportsSplitter;

class ViewportPanel : public QWidget
{
	Q_OBJECT
public:
	ViewportPanel(ViewportsSplitter * splitter = 0);
	~ViewportPanel();

	void addSceneStack(QWidget * widget);
	void addSceneItem(QString name);
	void removeSceneStack(QWidget * widget);
	void removeSceneItem(QString name);
	
	void setSplitter(ViewportsSplitter * splitterContainer);

public slots:
	void toggleXYGrid();
	void toggleAxes();
	void setSelectedScene(int idex);
	void setViewport(int index);

private:
	QVBoxLayout sceneLayout;
	QStackedWidget sceneStack;
	QToolBar sceneToolBar;
	QComboBox sceneSelection;
	QComboBox sceneCameras;
	QComboBox sceneDisplays;
	QComboBox sceneViewports;

	ViewportsSplitter * splitter;
	ViewerQOSG * _scene;

	bool xyGridToggled;
	bool axesToggled;

	// Actions for toolbar
	QAction * showgridAction;
	QAction * showgizmoAction;
	QAction * showaxisAction;
	QAction * saveviewAction;
};

#endif