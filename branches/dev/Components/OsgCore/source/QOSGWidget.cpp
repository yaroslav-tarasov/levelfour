/* OpenSceneGraph example, osganimate.
*
*  Permission is hereby granted, free of charge, to any person obtaining a copy
*  of this software and associated documentation files (the "Software"), to deal
*  in the Software without restriction, including without limitation the rights
*  to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
*  copies of the Software, and to permit persons to whom the Software is
*  furnished to do so, subject to the following conditions:
*
*  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
*  IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
*  FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
*  AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
*  LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
*  OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
*  THE SOFTWARE.
*/


#include <QOSGWidget.h>

QOSGWidget::QOSGWidget( QWidget * parent, const char * name, WindowFlags f, bool overrideTraits):
#if USE_QT4
    QWidget(parent, f), _overrideTraits (overrideTraits)
#else
    QWidget(parent, name, f), _overrideTraits (overrideTraits)
#endif
{
    createContext();
    

#if USE_QT4
    setAttribute(Qt::WA_PaintOnScreen);
    setAttribute(Qt::WA_NoSystemBackground);
    setFocusPolicy(Qt::ClickFocus);
#else
    setBackgroundMode(Qt::NoBackground);
#endif    
}

void QOSGWidget::createContext()
{
    osg::DisplaySettings* ds = osg::DisplaySettings::instance();

    osg::ref_ptr<osg::GraphicsContext::Traits> traits = new osg::GraphicsContext::Traits;

    traits->readDISPLAY();
    if (traits->displayNum<0) traits->displayNum = 0;

    traits->windowName = "osgViewerQt";
    traits->screenNum = 0;
    traits->x = x();
    traits->y = y();
    traits->width = width();
    traits->height = height();
    traits->alpha = ds->getMinimumNumAlphaBits();
    traits->stencil = ds->getMinimumNumStencilBits();
    traits->windowDecoration = false;
    traits->doubleBuffer = false; // if set to true -> crash
    traits->sharedContext = 0;
    traits->sampleBuffers = ds->getMultiSamples();
    traits->samples = ds->getNumMultiSamples();

#if defined(__APPLE__) 
    // Extract a WindowPtr from the HIViewRef that QWidget::winId() returns.
    // Without this change, the peer tries to call GetWindowPort on the HIViewRef
    // which returns 0 and we only render white.
    traits->inheritedWindowData = new WindowData(HIViewGetWindow((HIViewRef)winId()));

#else // all others
    traits->inheritedWindowData = new WindowData(winId());
#endif


    if (ds->getStereo())
    {
        switch(ds->getStereoMode())
        {
            case(osg::DisplaySettings::QUAD_BUFFER): traits->quadBufferStereo = true; break;
            case(osg::DisplaySettings::VERTICAL_INTERLACE):
            case(osg::DisplaySettings::CHECKERBOARD):
            case(osg::DisplaySettings::HORIZONTAL_INTERLACE): traits->stencil = 8; break;
            default: break;
        }
    }

    osg::ref_ptr<osg::GraphicsContext> gc = osg::GraphicsContext::createGraphicsContext(traits.get());
    _gw = dynamic_cast<osgViewer::GraphicsWindow*>(gc.get());

    // get around dearanged traits on X11 (MTCompositeViewer only)
    if (_overrideTraits)
    {
        traits->x = x();
        traits->y = y();
        traits->width = width();
        traits->height = height();
    }

}

QPaintEngine* QWidget::paintEngine () const
{
    return 0;
}

#ifndef WIN32 
void QOSGWidget::destroyEvent(bool destroyWindow, bool destroySubWindows)
{   
    _gw->getEventQueue()->closeWindow();
}


void QOSGWidget::closeEvent( QCloseEvent * event )
{
#ifndef USE_QT4
    event->accept();
#endif

    _gw->getEventQueue()->closeWindow();
}


void QOSGWidget::resizeEvent( QResizeEvent * event )
{
    const QSize & size = event->size();
    _gw->getEventQueue()->windowResize(0, 0, size.width(), size.height() );
    _gw->resized(0, 0, size.width(), size.height());
}

void QOSGWidget::keyPressEvent( QKeyEvent* event )
{
#if USE_QT4
    _gw->getEventQueue()->keyPress( (osgGA::GUIEventAdapter::KeySymbol) *(event->text().toAscii().data() ) );
#else
    _gw->getEventQueue()->keyPress( (osgGA::GUIEventAdapter::KeySymbol) event->ascii() );
#endif
}

void QOSGWidget::keyReleaseEvent( QKeyEvent* event )
{
#if USE_QT4
    int c = *event->text().toAscii().data();
#else
    int c = event->ascii();
#endif

    _gw->getEventQueue()->keyRelease( (osgGA::GUIEventAdapter::KeySymbol) (c) );
}

void QOSGWidget::mousePressEvent( QMouseEvent* event )
{
    int button = 0;
    switch(event->button())
    {
        case(Qt::LeftButton): button = 1; break;
        case(Qt::MidButton): button = 2; break;
        case(Qt::RightButton): button = 3; break;
        case(Qt::NoButton): button = 0; break;
        default: button = 0; break;
    }
    _gw->getEventQueue()->mouseButtonPress(event->x(), event->y(), button);
}
void QOSGWidget::mouseDoubleClickEvent ( QMouseEvent * event )
{
    int button = 0;
    switch(event->button())
    {
        case(Qt::LeftButton): button = 1; break;
        case(Qt::MidButton): button = 2; break;
        case(Qt::RightButton): button = 3; break;
        case(Qt::NoButton): button = 0; break;
        default: button = 0; break;
    }
    _gw->getEventQueue()->mouseDoubleButtonPress(event->x(), event->y(), button);
}
void QOSGWidget::mouseReleaseEvent( QMouseEvent* event )
{
    int button = 0;
    switch(event->button())
    {
        case(Qt::LeftButton): button = 1; break;
        case(Qt::MidButton): button = 2; break;
        case(Qt::RightButton): button = 3; break;
        case(Qt::NoButton): button = 0; break;
        default: button = 0; break;
    }
    _gw->getEventQueue()->mouseButtonRelease(event->x(), event->y(), button);
}

void QOSGWidget::mouseMoveEvent( QMouseEvent* event )
{
    _gw->getEventQueue()->mouseMotion(event->x(), event->y());
}
#endif

// Set up viewport interfaces
////////////////////////////////////////////////////////////////////////////////
void ViewerQOSG::toggleXYGrid(bool enabled)
{
   if(enabled)
   {
      mpXYGridTransform->setNodeMask(0xFFFFFFFF);
   }
   else
   {
      mpXYGridTransform->setNodeMask(0x0);
   }
}

////////////////////////////////////////////////////////////////////////////////
void ViewerQOSG::toggleXZGrid(bool enabled)
{
   if(enabled)
   {
      mpXZGridTransform->setNodeMask(0xFFFFFFFF);
   }
   else
   {
      mpXZGridTransform->setNodeMask(0x0);
   }
}

////////////////////////////////////////////////////////////////////////////////
void ViewerQOSG::toggleYZGrid(bool enabled)
{
   if(enabled)
   {
      mpYZGridTransform->setNodeMask(0xFFFFFFFF);
   }
   else
   {
      mpYZGridTransform->setNodeMask(0x0);
   }
}

////////////////////////////////////////////////////////////////////////////////

void ViewerQOSG::toggleAxes(bool enabled)
{
   if(enabled)
   {
      compassAxes->setNodeMask(0xFFFFFFFF);
   }
   else
   {
      compassAxes->setNodeMask(0x0);
   }
}

////////////////////////////////////////////////////////////////////////////////

void ViewerQOSG::initAxes()
{
	compassAxes = new CompassAxis(this);
}

////////////////////////////////////////////////////////////////////////////////

void ViewerQOSG::initGrids()
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

   osg::Geometry* geometry = new osg::Geometry;
   geometry->setVertexArray(new osg::Vec3Array(numVertices, vertices));
   geometry->addPrimitiveSet(new osg::DrawArrays(osg::PrimitiveSet::LINES, 0, numVertices));

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

void ViewerQOSG::setupManipulatorAndHandler()
{
    // set up the camera manipulators.
    {
        osg::ref_ptr<osgGA::KeySwitchMatrixManipulator> keyswitchManipulator = new osgGA::KeySwitchMatrixManipulator;

        keyswitchManipulator->addMatrixManipulator( '1', "Trackball", new osgGA::TrackballManipulator() );
        keyswitchManipulator->addMatrixManipulator( '2', "Flight", new osgGA::FlightManipulator() );
        keyswitchManipulator->addMatrixManipulator( '3', "Drive", new osgGA::DriveManipulator() );
        keyswitchManipulator->addMatrixManipulator( '4', "Terrain", new osgGA::TerrainManipulator() );

        this->setCameraManipulator( keyswitchManipulator.get() );
    }

    // add the state manipulator
    this->addEventHandler( new osgGA::StateSetManipulator(this->getCamera()->getOrCreateStateSet()) );

    // add the thread model handler
    this->addEventHandler(new osgViewer::ThreadingHandler);

    // add the window size toggle handler
    this->addEventHandler(new osgViewer::WindowSizeHandler);

    // add the stats handler
    this->addEventHandler(new osgViewer::StatsHandler);

}
