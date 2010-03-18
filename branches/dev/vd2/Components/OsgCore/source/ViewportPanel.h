#ifndef VIEWPORT_PANEL_H
#define VIEWPORT_PANEL_H

#include <QSplitter>
#include <QVBoxLayout>
#include <QToolBar>
#include <QComboBox>
#include <QStackedWidget>
#include "QViewWidget.h"

class ViewportsSplitter;
class QViewWidget;

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
	// select type (view, tree, table, data)
	void setSelectedType(int index);

	// select viewport (single, quad, top-bottom...)
	void setViewport(int index);
	void updateIndexLayout(const int& index);

private:
	QVBoxLayout sceneLayout;

	QToolBar mainToolBar;
	// stacked widgets used to switch from a typeOfView/scene and
	// related widgets to another typeOfView/scene 
	QStackedWidget toolStack;
	QStackedWidget centralStack;

	// global widgets
	QComboBox typeOfView;
	QComboBox sceneViewports;

	// scene view widgeta/actions
	QViewWidget * sceneWidgets;

	// same for tree, table, list...
	ViewportsSplitter * splitter;
};

#endif