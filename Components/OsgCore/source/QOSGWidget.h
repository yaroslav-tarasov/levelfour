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
#ifndef QOSGWIDGET_H
#define QOSGWIDGET_H

#define USE_QT4 1

#include <osg/Config>

#if defined(_MSC_VER) && defined(OSG_DISABLE_MSVC_WARNINGS)
    // disable warning "'QtConcurrent::BlockSizeManager' : assignment operator could not be generated"
    #pragma warning( disable : 4512 )
#endif

////////////////////////////////////////////////////////////////////////////////
/**
 * The number of lines in each direction.
 */
const int GRID_LINE_COUNT = 49;

/**
 * The amount of space between each line.
 */
const float GRID_LINE_SPACING = 1.0f;

////////////////////////////////////////////////////////////////////////////////


#if USE_QT4

    #include <QtCore/QString>
    #include <QtCore/QTimer>
    #include <QtGui/QKeyEvent>
    #include <QtGui/QApplication>
    #include <QtGui/QtGui>
    #include <QtGui/QWidget>
	#include <QtGui/QPaintEngine>

	using Qt::WindowFlags;

#else

    class QWidget;
    #include <qtimer.h>
    #include <qgl.h>
    #include <qapplication.h>

    #define WindowFlags WFlags

#endif


#include <osgViewer/Viewer>
#include <osgViewer/CompositeViewer>
#include <osgViewer/ViewerEventHandlers>
#include <osgViewer/GraphicsWindow>
#include "CompassAxis.h"

#include <osgViewer/ViewerEventHandlers>

#if defined(WIN32) && !defined(__CYGWIN__)
#include <osgViewer/api/Win32/GraphicsWindowWin32>
typedef HWND WindowHandle;
typedef osgViewer::GraphicsWindowWin32::WindowData WindowData;

#elif defined(__APPLE__)  // Assume using Carbon on Mac.
#include <osgViewer/api/Carbon/GraphicsWindowCarbon>
typedef WindowRef WindowHandle;
typedef osgViewer::GraphicsWindowCarbon::WindowData WindowData;
#else // all other unix
#include <osgViewer/api/X11/GraphicsWindowX11>
typedef Window WindowHandle;
typedef osgViewer::GraphicsWindowX11::WindowData WindowData;
#endif


#include <osgGA/TrackballManipulator>
#include <osgGA/FlightManipulator>
#include <osgGA/DriveManipulator>
#include <osgGA/KeySwitchMatrixManipulator>
#include <osgGA/StateSetManipulator>
#include <osgGA/AnimationPathManipulator>
#include <osgGA/TerrainManipulator>
#include <osg/ShapeDrawable>
#include <osg/Geode>
#include <osg/MatrixTransform>


#include <osgDB/ReadFile>

#include <iostream>
#include <sstream>

class QOSGWidget : public QWidget
{
    public:

        QOSGWidget( QWidget * parent = 0, const char * name = 0, WindowFlags f = 0, bool overrideTraits = false);

        virtual ~QOSGWidget() {}

        osgViewer::GraphicsWindow* getGraphicsWindow() { return _gw.get(); }
        const osgViewer::GraphicsWindow* getGraphicsWindow() const { return _gw.get(); }

    protected:

        void init();
        void createContext();

        //  The GraphincsWindowWin32 implementation already takes care of message handling.
        //  We don't want to relay these on Windows, it will just cause duplicate messages
        //  with further problems downstream (i.e. not being able to throw the trackball
#ifndef WIN32 
        virtual void mouseDoubleClickEvent ( QMouseEvent * event );
        virtual void closeEvent( QCloseEvent * event );
        virtual void destroyEvent( bool destroyWindow = true, bool destroySubWindows = true);
        virtual void resizeEvent( QResizeEvent * event );
        virtual void keyPressEvent( QKeyEvent* event );
        virtual void keyReleaseEvent( QKeyEvent* event );
        virtual void mousePressEvent( QMouseEvent* event );
        virtual void mouseReleaseEvent( QMouseEvent* event );
        virtual void mouseMoveEvent( QMouseEvent* event );
#endif
        osg::ref_ptr<osgViewer::GraphicsWindow> _gw;
    bool _overrideTraits;
};




class ViewerQOSG : public osgViewer::Viewer, public QOSGWidget
{
public:

	ViewerQOSG(QWidget * parent = 0, const char * name = 0, WindowFlags f = 0, bool showGrids = true, bool showAxes = true):
        QOSGWidget( parent, name, f )
	{
		mpXYGridTransform = mpXZGridTransform = mpYZGridTransform = 0;
		compassAxes = 0;
		init();
		_showGrids = showGrids;
		_showAxes = showAxes;
		if (_showGrids) initGrids();
		if (_showAxes) initAxes();
	}

	void updateCamera()
    {
        getCamera()->setViewport(new osg::Viewport(0,0,width(),height()));
        getCamera()->setProjectionMatrixAsPerspective(30.0f, static_cast<double>(width())/static_cast<double>(height()), 1.0f, 10000.0f);
        getCamera()->setGraphicsContext(getGraphicsWindow());
    }

	bool showGrids() const {return _showGrids;}
	bool showAxes() const {return _showAxes;}

    virtual void paintEvent( QPaintEvent * event ) { frame(); }
	
	void toggleXYGrid(bool enabled);
	void toggleXZGrid(bool enabled);
	void toggleYZGrid(bool enabled);
	void toggleAxes(bool enabled);

	osg::MatrixTransform* getXYGrid() const {return mpXYGridTransform;}
	osg::MatrixTransform* getXZGrid() const {return mpXZGridTransform;}
	osg::MatrixTransform* getYZGrid() const {return mpYZGridTransform;}

	CompassAxis * getAxes() const {return compassAxes;}

	void setupManipulatorAndHandler();

private:
	void init()
	{
        setThreadingModel(osgViewer::Viewer::SingleThreaded);

        connect(&_timer, SIGNAL(timeout()), this, SLOT(update()));
        _timer.start(10);
	}

	void initGrids();
	bool _showGrids;
	void initAxes();
	bool _showAxes;

	osg::MatrixTransform* mpXYGridTransform;
	osg::MatrixTransform* mpXZGridTransform;
	osg::MatrixTransform* mpYZGridTransform;

	CompassAxis * compassAxes;

protected:

    QTimer _timer;

};

class QOSGContainer : public QWidget
{
public:
	QOSGContainer(QWidget * parent = 0, ViewerQOSG * scene = 0) : QWidget(parent) 
	{
		if (scene && scene != _scene)
		{
			_scene = scene;
			_scene->setParent(this);
		}
	}

	virtual ~QOSGContainer() {}

	ViewerQOSG * getScene() const { return _scene; }
	void setScene(ViewerQOSG * scene) { _scene = scene; }

private:
	ViewerQOSG * _scene;
};
#endif