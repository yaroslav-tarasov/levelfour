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
	splitQuad();
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
			setViewportsLayout(0,0);
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
}

void ViewportsSplitter::removeScene(osg::Group* sceneRoot, QString sceneName)
{
	// TODO iterate the list and look for the object scene that contains
	// the corresponding scene to remove it form the list
	// update the scene stack accordingly
	first.removeScene(sceneName);
	second.removeScene(sceneName);
	third.removeScene(sceneName);
	fourth.removeScene(sceneName);
}

void ViewportsSplitter::setViewportsLayout(ViewportPanel * vp, int index)
{
	if (index<=5 && index>=0)
		currentLayoutIndex = index;

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
}

void ViewportsSplitter::splitSingle(ViewportPanel * vp)
{
	first.setParent(this);
	second.setParent(0);
	third.setParent(0);
	fourth.setParent(0);

	top.setParent(0);
	bottom.setParent(0);
}

void ViewportsSplitter::splitVertical(ViewportPanel * vp)
{
	setOrientation(Qt::Vertical);
	first.setParent(this);
	second.setParent(0);
	third.setParent(this);
	fourth.setParent(0);

	top.setParent(0);
	bottom.setParent(0);
}

void ViewportsSplitter::splitHorizontal(ViewportPanel * vp)
{
	setOrientation(Qt::Horizontal);
	first.setParent(this);
	second.setParent(0);
	third.setParent(this);
	fourth.setParent(0);

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
	third.setParent(&bottom);
	fourth.setParent(0);
}

void ViewportsSplitter::split1Up2Down(ViewportPanel * vp)
{
	setOrientation(Qt::Vertical);

	top.setParent(this);
	first.setParent(&top);
	second.setParent(0);

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