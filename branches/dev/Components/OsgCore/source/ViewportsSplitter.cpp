#include "ViewportsSplitter.h"

ViewportsSplitter::ViewportsSplitter(QWidget * parent) 
	: QSplitter(Qt::Vertical, parent)	
{
    connect( &_timer, SIGNAL(timeout()), this, SLOT(update()) );
    _timer.start( 10 );
    
	first.setSplitter(this);
	second.setSplitter(this);
	third.setSplitter(this);
	fourth.setSplitter(this);
	dummy.setSplitter(this);
	splitQuad();

	// add a dummy scene to the scene stack in order to 
	// avoid empty viewports if the scene-view panel is selected
	// before any scene is added to the node pipeline
	addScene(new osg::Group, "dummy");
    this->installEventFilter(this);
}

ViewportsSplitter::~ViewportsSplitter() 
{
}

ViewWidget * ViewportsSplitter::addViewWidget( osg::Camera* camera, osg::Group* scene, bool showGrid, bool showAxes)
{
    osgViewer::View* view = new osgViewer::View;
    view->setCamera( camera );
    addView( view );
	osg::Group * mainRoot = new osg::Group;
	mainRoot->addChild(scene);
    view->setSceneData(mainRoot);
    view->addEventHandler( new osgViewer::StatsHandler );
    view->setCameraManipulator( new osgGA::TrackballManipulator );
    
    GraphicsWindowQt* gw = dynamic_cast<GraphicsWindowQt*>( camera->getGraphicsContext() );
	if (gw)
	{
		gw->getGraphWidget()->setGeometry(0,0,width(),height());
		gw->getGraphWidget()->setMinimumSize(50, 50);
		return new ViewWidget(gw->getGraphWidget(), view, scene, showGrid, showAxes);
	}

    return NULL;
}

osg::Camera* ViewportsSplitter::createCamera( int x, int y, int w, int h, const std::string& name, bool windowDecoration)
{
    osg::DisplaySettings* ds = osg::DisplaySettings::instance();
    osg::ref_ptr<osg::GraphicsContext::Traits> traits = new osg::GraphicsContext::Traits;
    traits->windowName = name;
    traits->windowDecoration = windowDecoration;
    traits->x = x;
    traits->y = y;
    traits->width = w;
    traits->height = h;
    traits->doubleBuffer = true;
    traits->alpha = ds->getMinimumNumAlphaBits();
    traits->stencil = ds->getMinimumNumStencilBits();
    traits->sampleBuffers = ds->getMultiSamples();
    traits->samples = ds->getNumMultiSamples();
    
    osg::ref_ptr<osg::Camera> camera = new osg::Camera;
    camera->setGraphicsContext( new GraphicsWindowQt(traits.get()) );
    
    camera->setClearColor( osg::Vec4(0.2, 0.2, 0.6, 1.0) );
    camera->setViewport( new osg::Viewport(0, 0, traits->width, traits->height) );
    camera->setProjectionMatrixAsPerspective(
        30.0f, static_cast<double>(traits->width)/static_cast<double>(traits->height), 1.0f, 10000.0f );
    return camera.release();
}

bool ViewportsSplitter::eventFilter(QObject *object, QEvent *event)
{
    if (event->type() == QEvent::Show)
    {
		if (object == this)
        {
			setViewportsLayout(&first,0);
		    this->removeEventFilter(this);
        }
    }
    return false;
}

void ViewportsSplitter::addScene(osg::Group* sceneRoot, QString sceneName)
{
	if (!sceneRoot || sceneName.size() <= 0)
		return;
	
	first.addScene(sceneRoot,sceneName);
	second.addScene(sceneRoot,sceneName);
	third.addScene(sceneRoot,sceneName);
	fourth.addScene(sceneRoot,sceneName);

	// need to add the scene to a last dummy viewport to avoid flickering on the 
	// last viewport
	dummy.addScene(sceneRoot,sceneName);
}

void ViewportsSplitter::removeScene(osg::Group* sceneRoot, QString sceneName)
{
	first.removeScene(sceneName);
	second.removeScene(sceneName);
	third.removeScene(sceneName);
	fourth.removeScene(sceneName);
	dummy.addScene(sceneRoot,sceneName);
}

void ViewportsSplitter::setViewportsLayout(ViewportPanel * vp, int index)
{
	switch (index)
	{
	case 0:
		splitSingle(vp);
		break;
	case 1:
		splitVertical(vp);
		break;
	case 2: 
		splitHorizontal(vp);
		break;
	case 3:
		split2Up1Down(vp);
		break;
	case 4:
		split1Up2Down(vp);
		break;
	case 5:
		splitQuad();
		break;
	}
	if (index<=5 && index>=0)
	{
		currentLayoutIndex = index;
	}
}

void ViewportsSplitter::splitSingle(ViewportPanel * vp)
{
	if (vp == &first)
		first.setParent(this);
	else
		first.setParent(0);

	if (vp == &second)
		second.setParent(this);
	else
		second.setParent(0);

	if (vp == &third)
		third.setParent(this);
	else
		third.setParent(0);

	if (vp == &fourth)
		fourth.setParent(this);
	else
		fourth.setParent(0);

	top.setParent(0);
	bottom.setParent(0);
}

void ViewportsSplitter::splitVertical(ViewportPanel * vp)
{
	setOrientation(Qt::Vertical);
	if (vp == &second)
	{
		first.setParent(0);
		second.setParent(this);
	} else {
		first.setParent(this);
		second.setParent(0);
	}

	if (vp == &fourth)
	{
		third.setParent(0);
		fourth.setParent(this);
	} else {
		third.setParent(this);
		fourth.setParent(0);
	}

	top.setParent(0);
	bottom.setParent(0);
}

void ViewportsSplitter::splitHorizontal(ViewportPanel * vp)
{
	setOrientation(Qt::Horizontal);
	if (vp == &second)
	{
		first.setParent(0);
		second.setParent(this);
	} else {
		first.setParent(this);
		second.setParent(0);
	}

	if (vp == &fourth)
	{
		third.setParent(0);
		fourth.setParent(this);
	} else {
		third.setParent(this);
		fourth.setParent(0);
	}

	top.setParent(0);
	bottom.setParent(0);
}

void ViewportsSplitter::split2Up1Down(ViewportPanel * vp) 
{
	setOrientation(Qt::Vertical);

	top.setParent(this);
	first.setParent(&top);
	second.setParent(&top);

	bottom.setParent(this);
	if (vp == &fourth)
	{
		third.setParent(0);
		fourth.setParent(&bottom);
	} else {
		third.setParent(&bottom);
		fourth.setParent(0);
	}
}

void ViewportsSplitter::split1Up2Down(ViewportPanel * vp)
{
	setOrientation(Qt::Vertical);

	top.setParent(this);
	if (vp == &second)
	{
		first.setParent(0);
		second.setParent(&top);
	} else {
		first.setParent(&top);
		second.setParent(0);
	}

	bottom.setParent(this);
	third.setParent(&bottom);
	fourth.setParent(&bottom);
}

void ViewportsSplitter::splitQuad()
{
	setOrientation(Qt::Vertical);

	top.setParent(this);
	first.setParent(&top);
	second.setParent(&top);

	bottom.setParent(this);
	third.setParent(&bottom);
	fourth.setParent(&bottom);
}

void ViewportsSplitter::deactivateAll()
{
}