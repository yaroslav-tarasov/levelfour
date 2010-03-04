#ifndef VIEWPORTS_SPLITTER_H
#define VIEWPORTS_SPLITTER_H

#include <QSplitter>
#include "ViewportPanel.h"
#include "GraphicsWindowQt.h"

class ViewportPanel;



class ViewportsSplitter : public QSplitter, public osgViewer::CompositeViewer
{
	Q_OBJECT
public:
	ViewportsSplitter(QWidget * parent = 0);
	~ViewportsSplitter();

	void setViewportsLayout(ViewportPanel * vp, int index);

	void addScene(osg::Group * sceneRoot, QString sceneName);
	void removeScene(osg::Group * sceneRoot, QString sceneName);
	ViewWidget * addViewWidget( osg::Camera* camera, osg::Group* scene, bool showGrid = true, bool showAxes = true);
	osg::Camera* createCamera( int x, int y, int w, int h, const std::string& name="", bool windowDecoration=false );

	int getLayoutIndex() const {return currentLayoutIndex;}
	ViewWidget * getDummy() const {return dummyViewWidget;}

private:
	bool eventFilter(QObject *object, QEvent *event);
	void splitSingle(ViewportPanel * vp);
	void splitVertical(ViewportPanel * vp);
	void splitHorizontal(ViewportPanel * vp);
	void split2Up1Down(ViewportPanel * vp);
	void split1Up2Down(ViewportPanel * vp);
	void splitQuad();
	void deactivateAll();

	ViewportPanel first, second, third, fourth, dummy;
	QSplitter top;
	QSplitter bottom;

	int currentLayoutIndex;
	ViewWidget * dummyViewWidget;
    
    virtual void paintEvent( QPaintEvent* event )
    { frame(); }

protected:
    
    QTimer _timer;
};

#endif