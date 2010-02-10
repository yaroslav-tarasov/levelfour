#include "QOSGGraphics.h"
#include <QtGui/QPaintEngine>
#include <QtGui/QGraphicsSceneMouseEvent>

osg::QAdapterScene::QAdapterScene(QObject* parent)
:	QGraphicsScene( 0, 0, 100, 100, parent)
{
	_gw = new osgViewer::GraphicsWindowEmbedded(0,0,width(),height());
}

osg::QAdapterScene::QAdapterScene( const QRectF & sceneRect, QObject * parent )
:	QGraphicsScene(sceneRect, parent)
{
	_gw = new osgViewer::GraphicsWindowEmbedded(sceneRect.x(), sceneRect.y(), sceneRect.width(), sceneRect.height());
}

osg::QAdapterScene::QAdapterScene( qreal x, qreal y, qreal w, qreal h, QObject * parent )
:	QGraphicsScene(x,y,w,h,parent)
{
	_gw = new osgViewer::GraphicsWindowEmbedded(x,y,w,h);
}

void osg::QAdapterScene::resizeEvent( QResizeEvent* event )
{
	_gw->getEventQueue()->windowResize(0, 0, event->size().width(), event->size().height());
    _gw->resized(0, 0, event->size().width(), event->size().height());
}

void osg::QAdapterScene::keyPressEvent( QKeyEvent* event )
{
	QGraphicsScene::keyPressEvent(event);
	if( event->isAccepted() ) return;
	if( event->text().isEmpty() )
		_gw->getEventQueue()->keyPress( convertSymbol(event->key()) );
	else
		_gw->getEventQueue()->keyPress( (osgGA::GUIEventAdapter::KeySymbol) *(event->text().toAscii().data() ) );
}

void osg::QAdapterScene::keyReleaseEvent( QKeyEvent* event )
{
	QGraphicsScene::keyReleaseEvent(event);
	if( event->isAccepted() ) return;
	if( event->text().isEmpty() )
		_gw->getEventQueue()->keyRelease( convertSymbol(event->key()) );
	else
		_gw->getEventQueue()->keyRelease( (osgGA::GUIEventAdapter::KeySymbol) *(event->text().toAscii().data() ) );
}

void osg::QAdapterScene::mousePressEvent( QGraphicsSceneMouseEvent* event )
{
	QGraphicsScene::mousePressEvent(event);
	if( event->isAccepted() ) return;
    int button = 0;
    switch(event->button())
    {
        case(Qt::LeftButton): button = 1; break;
        case(Qt::MidButton): button = 2; break;
        case(Qt::RightButton): button = 3; break;
        case(Qt::NoButton): button = 0; break;
        default: button = 0; break;
    }
    _gw->getEventQueue()->mouseButtonPress(event->scenePos().x(), event->scenePos().y(), button);
}

void osg::QAdapterScene::mouseReleaseEvent( QGraphicsSceneMouseEvent* event )
{
	QGraphicsScene::mouseReleaseEvent(event);
	if( event->isAccepted() ) return;
    int button = 0;
    switch(event->button())
    {
        case(Qt::LeftButton): button = 1; break;
        case(Qt::MidButton): button = 2; break;
        case(Qt::RightButton): button = 3; break;
        case(Qt::NoButton): button = 0; break;
        default: button = 0; break;
    }
    _gw->getEventQueue()->mouseButtonRelease(event->scenePos().x(), event->scenePos().y(), button);
}

void osg::QAdapterScene::mouseMoveEvent( QGraphicsSceneMouseEvent* event )
{
	QGraphicsScene::mouseMoveEvent(event);
	if( event->isAccepted() ) return;
    _gw->getEventQueue()->mouseMotion(event->scenePos().x(), event->scenePos().y());
}

/** Translate Qt key symbols (Qt::Key) to osg one.
 *	Special key cannot been translated via ascii value.
 *	When a key symbol exists in both side, we translate it (done at qt4.3 and osg2.3.6-doc generated Fri Jun 15 16:28:09 2007).
 *	Prefere a convertion from ascii if possible.
 *	Since Qt does not distinguish Ctrl left and right, we return the osg left one.
 */
osgGA::GUIEventAdapter::KeySymbol osg::QAdapterScene::convertSymbol(int k)
{
	switch(k)
	{
	case Qt::Key_Space : return osgGA::GUIEventAdapter::KEY_Space;
	case Qt::Key_Backspace : return osgGA::GUIEventAdapter::KEY_BackSpace;
	case Qt::Key_Tab : return osgGA::GUIEventAdapter::KEY_Tab;
	case Qt::Key_Clear : return osgGA::GUIEventAdapter::KEY_Clear;
	case Qt::Key_Return : return osgGA::GUIEventAdapter::KEY_Return;
	case Qt::Key_Enter : return osgGA::GUIEventAdapter::KEY_KP_Enter;
	case Qt::Key_Pause : return osgGA::GUIEventAdapter::KEY_Pause;
	case Qt::Key_ScrollLock : return osgGA::GUIEventAdapter::KEY_Scroll_Lock;
	case Qt::Key_SysReq : return osgGA::GUIEventAdapter::KEY_Sys_Req;
	case Qt::Key_Escape : return osgGA::GUIEventAdapter::KEY_Escape;
	case Qt::Key_Delete : return osgGA::GUIEventAdapter::KEY_Delete;
	case Qt::Key_Home : return osgGA::GUIEventAdapter::KEY_Home;
	case Qt::Key_Left : return osgGA::GUIEventAdapter::KEY_Left;
	case Qt::Key_Up : return osgGA::GUIEventAdapter::KEY_Up;
	case Qt::Key_Right : return osgGA::GUIEventAdapter::KEY_Right;
	case Qt::Key_Down : return osgGA::GUIEventAdapter::KEY_Down;
	case Qt::Key_PageUp : return osgGA::GUIEventAdapter::KEY_Page_Up;
	case Qt::Key_PageDown : return osgGA::GUIEventAdapter::KEY_Page_Down;
	case Qt::Key_End : return osgGA::GUIEventAdapter::KEY_End;
	case Qt::Key_Select : return osgGA::GUIEventAdapter::KEY_Select;
	case Qt::Key_Print : return osgGA::GUIEventAdapter::KEY_Print;
	case Qt::Key_Execute : return osgGA::GUIEventAdapter::KEY_Execute;
	case Qt::Key_Insert : return osgGA::GUIEventAdapter::KEY_Insert;
	case Qt::Key_Menu : return osgGA::GUIEventAdapter::KEY_Menu;
	case Qt::Key_Cancel : return osgGA::GUIEventAdapter::KEY_Cancel;
	case Qt::Key_Help : return osgGA::GUIEventAdapter::KEY_Help;
	case Qt::Key_Mode_switch : return osgGA::GUIEventAdapter::KEY_Mode_switch;
	case Qt::Key_NumLock : return osgGA::GUIEventAdapter::KEY_Num_Lock;
	case Qt::Key_Equal : return osgGA::GUIEventAdapter::KEY_KP_Equal;
	case Qt::Key_Asterisk : return osgGA::GUIEventAdapter::KEY_KP_Multiply;
	case Qt::Key_Plus : return osgGA::GUIEventAdapter::KEY_KP_Add;
	case Qt::Key_Minus : return osgGA::GUIEventAdapter::KEY_KP_Subtract;
	case Qt::Key_Comma : return osgGA::GUIEventAdapter::KEY_KP_Decimal;
	case Qt::Key_Slash : return osgGA::GUIEventAdapter::KEY_KP_Divide;
	case Qt::Key_0 : return osgGA::GUIEventAdapter::KEY_KP_0;
	case Qt::Key_1 : return osgGA::GUIEventAdapter::KEY_KP_1;
	case Qt::Key_2 : return osgGA::GUIEventAdapter::KEY_KP_2;
	case Qt::Key_3 : return osgGA::GUIEventAdapter::KEY_KP_3;
	case Qt::Key_4 : return osgGA::GUIEventAdapter::KEY_KP_4;
	case Qt::Key_5 : return osgGA::GUIEventAdapter::KEY_KP_5;
	case Qt::Key_6 : return osgGA::GUIEventAdapter::KEY_KP_6;
	case Qt::Key_7 : return osgGA::GUIEventAdapter::KEY_KP_7;
	case Qt::Key_8 : return osgGA::GUIEventAdapter::KEY_KP_8;
	case Qt::Key_9 : return osgGA::GUIEventAdapter::KEY_KP_9;
	case Qt::Key_F1 : return osgGA::GUIEventAdapter::KEY_F1;
	case Qt::Key_F2 : return osgGA::GUIEventAdapter::KEY_F2;
	case Qt::Key_F3 : return osgGA::GUIEventAdapter::KEY_F3;
	case Qt::Key_F4 : return osgGA::GUIEventAdapter::KEY_F4;
	case Qt::Key_F5 : return osgGA::GUIEventAdapter::KEY_F5;
	case Qt::Key_F6 : return osgGA::GUIEventAdapter::KEY_F6;
	case Qt::Key_F7 : return osgGA::GUIEventAdapter::KEY_F7;
	case Qt::Key_F8 : return osgGA::GUIEventAdapter::KEY_F8;
	case Qt::Key_F9 : return osgGA::GUIEventAdapter::KEY_F9;
	case Qt::Key_F10 : return osgGA::GUIEventAdapter::KEY_F10;
	case Qt::Key_F11 : return osgGA::GUIEventAdapter::KEY_F11;
	case Qt::Key_F12 : return osgGA::GUIEventAdapter::KEY_F12;
	case Qt::Key_F13 : return osgGA::GUIEventAdapter::KEY_F13;
	case Qt::Key_F14 : return osgGA::GUIEventAdapter::KEY_F14;
	case Qt::Key_F15 : return osgGA::GUIEventAdapter::KEY_F15;
	case Qt::Key_F16 : return osgGA::GUIEventAdapter::KEY_F16;
	case Qt::Key_F17 : return osgGA::GUIEventAdapter::KEY_F17;
	case Qt::Key_F18 : return osgGA::GUIEventAdapter::KEY_F18;
	case Qt::Key_F19 : return osgGA::GUIEventAdapter::KEY_F19;
	case Qt::Key_F20 : return osgGA::GUIEventAdapter::KEY_F20;
	case Qt::Key_F21 : return osgGA::GUIEventAdapter::KEY_F21;
	case Qt::Key_F22 : return osgGA::GUIEventAdapter::KEY_F22;
	case Qt::Key_F23 : return osgGA::GUIEventAdapter::KEY_F23;
	case Qt::Key_F24 : return osgGA::GUIEventAdapter::KEY_F24;
	case Qt::Key_F25 : return osgGA::GUIEventAdapter::KEY_F25;
	case Qt::Key_F26 : return osgGA::GUIEventAdapter::KEY_F26;
	case Qt::Key_F27 : return osgGA::GUIEventAdapter::KEY_F27;
	case Qt::Key_F28 : return osgGA::GUIEventAdapter::KEY_F28;
	case Qt::Key_F29 : return osgGA::GUIEventAdapter::KEY_F29;
	case Qt::Key_F30 : return osgGA::GUIEventAdapter::KEY_F30;
	case Qt::Key_F31 : return osgGA::GUIEventAdapter::KEY_F31;
	case Qt::Key_F32 : return osgGA::GUIEventAdapter::KEY_F32;
	case Qt::Key_F33 : return osgGA::GUIEventAdapter::KEY_F33;
	case Qt::Key_F34 : return osgGA::GUIEventAdapter::KEY_F34;
	case Qt::Key_F35 : return osgGA::GUIEventAdapter::KEY_F35;
	case Qt::Key_Shift : return osgGA::GUIEventAdapter::KEY_Shift_L;
//	case Qt::Key_Shift_R : return osgGA::GUIEventAdapter::KEY_Shift_R;
	case Qt::Key_Control : return osgGA::GUIEventAdapter::KEY_Control_L;
//	case Qt::Key_Control_R : return osgGA::GUIEventAdapter::KEY_Control_R;
	case Qt::Key_CapsLock : return osgGA::GUIEventAdapter::KEY_Caps_Lock;
	case Qt::Key_Meta : return osgGA::GUIEventAdapter::KEY_Meta_L;
//	case Qt::Key_Meta_R: return osgGA::GUIEventAdapter::KEY_Meta_R;
	case Qt::Key_Alt : return osgGA::GUIEventAdapter::KEY_Alt_L;
//	case Qt::Key_Alt_R : return osgGA::GUIEventAdapter::KEY_Alt_R;
	case Qt::Key_Super_L : return osgGA::GUIEventAdapter::KEY_Super_L;
	case Qt::Key_Super_R : return osgGA::GUIEventAdapter::KEY_Super_R;
	case Qt::Key_Hyper_L : return osgGA::GUIEventAdapter::KEY_Hyper_L;
	case Qt::Key_Hyper_R : return osgGA::GUIEventAdapter::KEY_Hyper_R;
	default:
		return (osgGA::GUIEventAdapter::KeySymbol)k;
	}
}

void osg::QOSGScene::init(qreal x, qreal y, qreal w, qreal h)
{
	getCamera()->setViewport(new osg::Viewport(x,y,w,h));
	getCamera()->setProjectionMatrixAsPerspective(30.0f, static_cast<double>(w)/static_cast<double>(h), 1.0f, 10000.0f);
	getCamera()->setGraphicsContext(getGraphicsWindow());

	osg::Vec4 bgColor(0.45,0.45,0.45,1);
	getCamera()->setClearColor(bgColor);

	setThreadingModel(osgViewer::Viewer::SingleThreaded);

	connect(&_timer, SIGNAL(timeout()), this, SLOT(update()));
	_timer.start(20);
}

osg::QOSGScene::QOSGScene(QObject* parent)
:	QAdapterScene( 0, 0, 100, 100, parent)
{
	init(0, 0, 100, 100);
}

osg::QOSGScene::QOSGScene( const QRectF & sceneRect, QObject * parent )
:	QAdapterScene(sceneRect, parent)
{
	init(sceneRect.x(), sceneRect.y(), sceneRect.width(), sceneRect.height());
}

osg::QOSGScene::QOSGScene( qreal x, qreal y, qreal w, qreal h, QObject * parent )
:	QAdapterScene(x,y,w,h,parent)
{
	init(x,y,w,h);
}

/**	Draw OpenGL as background, all widgets or else will be drawn on top (see widget opacity).
 *	It is possible to reimplement drawForeground() to do OpenGL rendering on top. */
void  osg::QOSGScene::drawBackground ( QPainter * painter, const QRectF & rect )
{
	// we ensure QOSGScene is actually set in a correct way.
	if( painter->paintEngine()->type() == QPaintEngine::OpenGL )
	{
		glMatrixMode(GL_PROJECTION);
		glPushMatrix();
		glMatrixMode(GL_MODELVIEW);
		glPushMatrix();
		glMatrixMode(GL_TEXTURE);
		glPushMatrix();
		frame();
		glMatrixMode(GL_TEXTURE);
		glPopMatrix();
		glMatrixMode(GL_MODELVIEW);
		glPopMatrix();
		glMatrixMode(GL_PROJECTION);
		glPopMatrix();
	}
	else
		qWarning("QOSGScene: drawBackground needs a QGLWidget to be set as viewport on the graphics view.");
}

void osg::QOSGCompositeScene::init()
{
	setThreadingModel(osgViewer::CompositeViewer::SingleThreaded);

	connect(&_timer, SIGNAL(timeout()), this, SLOT(update()));
	_timer.start(20);
}

osg::QOSGCompositeScene::QOSGCompositeScene(QObject* parent)
:	QAdapterScene(0, 0, 100, 100, parent)
{
	init();
}

osg::QOSGCompositeScene::QOSGCompositeScene( const QRectF & sceneRect, QObject * parent )
:	QAdapterScene(sceneRect, parent)
{
	init();
}

osg::QOSGCompositeScene::QOSGCompositeScene( qreal x, qreal y, qreal w, qreal h, QObject * parent )
:	QAdapterScene(x,y,w,h,parent)
{
	init();
}

/**	Draw OpenGL as background, all widgets or else will be drawn on top (see widget opacity).
 *	It is possible to reimplement drawForeground() to do OpenGL renderingon top. */
void  osg::QOSGCompositeScene::drawBackground ( QPainter * painter, const QRectF & rect )
{
	// we ensure QOSGScene is actually set in a correct way.
	if( painter->paintEngine()->type() == QPaintEngine::OpenGL )
	{
		glMatrixMode(GL_PROJECTION);
		glPushMatrix();
		glMatrixMode(GL_MODELVIEW);
		glPushMatrix();
		glMatrixMode(GL_TEXTURE);
		glPushMatrix();
		frame();
		glMatrixMode(GL_TEXTURE);
		glPopMatrix();
		glMatrixMode(GL_MODELVIEW);
		glPopMatrix();
		glMatrixMode(GL_PROJECTION);
		glPopMatrix();
	}
	else
		qWarning("QOSGScene: drawBackground needs a QGLWidget to be set as viewport on the graphics view.");
}

/** Setup a graphic view to embedded OSG.
 *	Set a QGLWidget as viewport to enabled OpenGL rendering. 
 *	We use SamplerBuffers format specifier to enable multisample anti-aliasing in our rendering code.
 *	Set the update mode to FullViewportUpdate since a QGLWidget cannot performp partial upadtes. */
osg::QGLGraphicsView::QGLGraphicsView(QWidget * parent)
:	QGraphicsView(parent)
{
	setWindowTitle(tr("3D Model Viewer"));
	setViewport( new QGLWidget( QGLFormat(QGL::SampleBuffers | QGL::AccumBuffer | QGL::AlphaChannel) ) );
	setViewportUpdateMode( QGraphicsView::FullViewportUpdate );
}

/** Setup a graphic view to embedded OSG.
 *	Set a QGLWidget as viewport to enabled OpenGL rendering. 
 *	We use SamplerBuffers format specifier to enable multisample anti-aliasing in our rendering code.
 *	Set the update mode to FullViewportUpdate since a QGLWidget cannot performp partial upadtes. */
osg::QGLGraphicsView::QGLGraphicsView(QOSGScene * scene, QWidget * parent)
:	QGraphicsView(scene, parent)
{
	setWindowTitle(tr("3D Model Viewer"));
	setViewport( new QGLWidget( QGLFormat(QGL::SampleBuffers | QGL::AccumBuffer | QGL::AlphaChannel) ) );
	setViewportUpdateMode( QGraphicsView::FullViewportUpdate );
}

/** Change the size of the scene (especially 3D) to fit the size of the resizeEvent.
 *	It is very strange we must cast the scene to QAdapterScene to call the right setSceneRect().
 *	The compilator should do this itself ??? */
void osg::QGLGraphicsView::resizeEvent( QResizeEvent* event )
{
	if( scene() )
	{
		QAdapterScene * pAdapterScene = dynamic_cast<QAdapterScene*>(scene());
		if( pAdapterScene )		pAdapterScene->resizeEvent( event );

		scene()->setSceneRect( QRect( QPoint(0,0), event->size() ) );
	}

	QGraphicsView::resizeEvent( event );
}

/*EOF*/
