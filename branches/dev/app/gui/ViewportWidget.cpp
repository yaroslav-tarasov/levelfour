/*
-----------------------------------------------------------------------------
This source file is part of FRAPPER
research.animationsinstitut.de
sourceforge.net/projects/frapper

Copyright (c) 2008-2009 Filmakademie Baden-Wuerttemberg, Institute of Animation 

This program is free software; you can redistribute it and/or modify it under
the terms of the GNU Lesser General Public License as published by the Free Software
Foundation; version 2.1 of the License.

This program is distributed in the hope that it will be useful, but WITHOUT
ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
FOR A PARTICULAR PURPOSE. See the GNU Lesser General Public License for more details.

You should have received a copy of the GNU Lesser General Public License along with
this program; if not, write to the Free Software Foundation, Inc., 59 Temple
Place - Suite 330, Boston, MA 02111-1307, USA, or go to
http://www.gnu.org/licenses/old-licenses/lgpl-2.1.html
-----------------------------------------------------------------------------
*/

//!
//! \file "ViewportWidget.cpp"
//! \brief Implementation file for ViewportWidget class.
//!
//! \author     Stefan Habel <stefan.habel@filmakademie.de>
//! \author     Nils Zweiling <nils.zweiling@filmakademie.de>
//! \version    1.0
//! \date       29.06.2009 (last updated)
//!

#include "ViewportWidget.h"
#include "OgreTools.h"
#include "OgreManager.h"
#include "Log.h"
#include <QtGui/QApplication>
#include <OgreOverlay.h>
#include <OgreOverlayManager.h>
#if !defined(Q_WS_WIN)
#include <QX11Info>
#endif

INIT_INSTANCE_COUNTER(ViewportWidget)


///
/// Private Static Data
///


//!
//! The index of the next viewport widget to create.
//!
unsigned int ViewportWidget::s_viewportIndex = 0;

//!
//! Material for self-illuminating red objects (like coordinate X axes).
//!
Ogre::MaterialPtr ViewportWidget::s_redMaterial;

//!
//! Material for self-illuminating green objects (like coordinate Y axes).
//!
Ogre::MaterialPtr ViewportWidget::s_greenMaterial;

//!
//! Material for self-illuminating blue objects (like coordinate Z axes).
//!
Ogre::MaterialPtr ViewportWidget::s_blueMaterial;

//!
//! Material for self-illuminating gray objects.
//!
Ogre::MaterialPtr ViewportWidget::s_grayMaterial;

//!
//! Material for self-illuminating black objects.
//!
Ogre::MaterialPtr ViewportWidget::s_blackMaterial;

//!
//! Default material to use in shaded mode.
//!
Ogre::MaterialPtr ViewportWidget::s_defaultMaterial;

//!
//! Material for Null objects.
//!
Ogre::MaterialPtr ViewportWidget::s_nullMaterial;

//!
//! Material for semi-transparent mask overlays.
//!
Ogre::MaterialPtr ViewportWidget::s_maskMaterial;

////!
////! Material for the viewport's image overlay that is used to display image
////! results from nodes.
////!
//Ogre::MaterialPtr ViewportWidget::s_imageMaterial;


///
/// Constructors and Destructors
///


//!
//! Constructor of the ViewportWidget class.
//!
//! \param parent The parent widget the created instance will be a child of.
//! \param stageIndex The index of the stage to display in the viewport.
//! \param fullscreen Flag that controls whether the widget is used in fullscreen mode.
//! \param width The width of the viewport when in fullscreen mode.
//! \param height The height of the viewport when in fullscreen mode.
//!
ViewportWidget::ViewportWidget ( QWidget *parent, unsigned int stageIndex /* = 1 */, bool fullscreen /* = false */, bool stereo /* = false */, int width /* = 1024 */, int height /* = 768 */ ) :
    QWidget(parent),
    m_tmpTexture(0),
    m_stageIndex(stageIndex),
    m_fullscreen(fullscreen),
    m_stereo(stereo),
    m_stereoNearClip(0.01),
    m_stereoFarClip(1000.0),
    m_stereoEyeDist(0.3),
    m_stereoFOffsetMultiplier(1.0),
    m_stereoFov(50.0),
    m_stereoFocalLength(150.0),
    m_stereoRttWidth(1920),
    m_stereoRttHeight(1080),
    m_width(width),
    m_height(height),
    // OGRE system objects
    m_renderWindow(0),
    m_sceneManager(OgreManager::getSceneManager()),
    m_cameraSceneNode(0),
    m_camera(0),
    m_camera0(0),
    m_camera1(0),
    m_camera2(0),
    m_camera3(0),
    m_camera4(0),
    m_cameraName(""),
    m_headLight(0),
    m_viewport(0),
	m_backgroundColor(Ogre::ColourValue(0.23f, 0.23f, 0.23f)),
    m_stereoViewport0(0),
    m_stereoViewport1(0),
    m_stereoViewport2(0),
    m_stereoViewport3(0),
    m_stereoViewport4(0),
    m_stereoRenderTarget(0),
    m_orientationIndicatorCameraSceneNode(0),
    m_orientationIndicatorCamera(0),
    m_orientationIndicatorViewport(0),
    // Overlays
    m_labelOverlay(0),
    m_imageOverlay(0),
    m_maskOverlay(0),
    m_leftTopHudPanel(0),
    m_leftTopHudTextArea(0),
    m_imagePanel(0),
    m_topMaskPanel(0),
    m_bottomMaskPanel(0),
    m_leftMaskPanel(0),
    m_rightMaskPanel(0),
    // OGRE scene nodes
    m_viewportObjectsSceneNode(0),
    m_originSceneNode(0),
    m_centerPointSceneNode(0),
    m_orientationIndicatorSceneNode(0),
    m_gridSceneNode(0),
    // viewing flags
    m_viewMode(false),
    m_lastPosition(QPoint(0, 0)),
    m_viewingParameters(new ViewingParameters()),
    // display flags
    m_wireframe(false),
    m_backfaceCulling(false),
    m_showOrigin(false),
    m_showCenterPoint(false),
    m_showOrientationIndicator(false),
    m_showGrid(false),
    m_overlayOnly(false),
    m_dbgColor(0.0)
{
    // set Qt widget properties
    setFocusPolicy(Qt::StrongFocus);
    setMouseTracking(true);

    // create the render window
    Log::debug("Creating render window...", "ViewportWidget::ViewportWidget");
    bool renderWindowCreated = createRenderWindow();
    setAttribute(Qt::WA_PaintOnScreen, m_sceneManager && renderWindowCreated);
    setAttribute(Qt::WA_NoSystemBackground, m_sceneManager && renderWindowCreated);
    setAttribute(Qt::WA_OpaquePaintEvent, m_sceneManager && renderWindowCreated);
    if (renderWindowCreated) {
        Log::debug("Initializing resources...", "ViewportWidget::ViewportWidget");
        initializeResources();
        Log::debug("Creating viewport objects...", "ViewportWidget::ViewportWidget");
        createViewportObjects();
        Log::debug("Creating overlays...", "ViewportWidget::ViewportWidget");
        createOverlays();

        homeView();
        setWireframe(m_wireframe);
        setBackfaceCulling(m_backfaceCulling);
    }

    //Ogre::Camera *camera = m_sceneManager->createCamera("CameraShot_blocking_cam");
    //camera->setProjectionType(Ogre::PT_ORTHOGRAPHIC);
    //camera->setOrthoWindowWidth(8.236441);
    //camera->setPosition(20.24489464, 9.845539671, 9.262762583);
    //camera->setOrientation(Ogre::Quaternion(-0.3529864455, 0.24425184, 0.01483895624, 0.9030622422));
    //camera->setFOVy(Ogre::Radian(0.006334871897));
    //camera->setAspectRatio(1.7438);
    //camera->setNearClipDistance(0.1);
    //camera->setFarClipDistance(1000);
    //Ogre::Matrix4 projectionMatrix = camera->getProjectionMatrix();
    //Ogre::Matrix4 viewMatrix = camera->getViewMatrix();

    //Log::debug("Projection Matrix:", "ViewportWidget::ViewportWidget");
    //for (int i = 0; i < 4; ++i)
    //    Log::debug(QString("%1 %2 %3 %4").arg(projectionMatrix[i][0]).arg(projectionMatrix[i][1]).arg(projectionMatrix[i][2]).arg(projectionMatrix[i][3]), "ViewportWidget::ViewportWidget");
    //Log::debug("View Matrix:", "ViewportWidget::ViewportWidget");
    //for (int i = 0; i < 4; ++i)
    //    Log::debug(QString("%1 %2 %3 %4").arg(viewMatrix[i][0]).arg(viewMatrix[i][1]).arg(viewMatrix[i][2]).arg(viewMatrix[i][3]), "ViewportWidget::ViewportWidget");
    ++s_viewportIndex;

    INC_INSTANCE_COUNTER
}


//!
//! Destructor of the ViewportWidget class.
//!
//! Defined virtual to guarantee that the destructor of a derived class
//! will be called if the instance of the derived class is saved in a
//! variable of its parent class type.
//!
ViewportWidget::~ViewportWidget ()
{
    delete m_viewingParameters;

    DEC_INSTANCE_COUNTER

    if (s_numberOfInstances == 0) {
        s_redMaterial.setNull();
        s_greenMaterial.setNull();
        s_blueMaterial.setNull();
        s_grayMaterial.setNull();
        s_blackMaterial.setNull();
        s_defaultMaterial.setNull();
        s_nullMaterial.setNull();
        s_maskMaterial.setNull();
        //m_imageMaterial.setNull();
    }
}


///
/// Public Functions
///


//!
//! Returns a pointer to the paint engine used for drawing on the device.
//!
//! Returning 0 by this function fixes the widget flickering bug in Qt 4.5.
//!
//! \return 0.
//!
QPaintEngine * ViewportWidget::paintEngine () const
{
    return 0;
}


//!
//! Returns the index of the stage to display in the viewport.
//!
//! \return The index of the stage to display in the viewport.
//!
unsigned int ViewportWidget::getStageIndex ()
{
    return m_stageIndex;
}


//!
//! Sets the index of the stage to display in the viewport.
//!
//! \param stageIndex The index of the new stage to display in the viewport.
//!
void ViewportWidget::setStageIndex ( unsigned int stageIndex )
{
    m_stageIndex = stageIndex;
}


//!
//! Activates or deactivates the view mode in the viewport widget.
//!
//! \param viewMode The new value for the view mode flag.
//!
void ViewportWidget::setViewMode ( bool viewMode )
{
    m_viewMode = viewMode;

    if (m_viewMode)
        setCursor(Qt::OpenHandCursor);
    else
        setCursor(Qt::ArrowCursor);
}


//!
//! Centers the viewport's view on currently selected objects.
//!
void ViewportWidget::frameSelectedObjects ()
{
    emit frameSelectedObjectsRequested(m_viewingParameters);

    updateView();
    render();
}

//!
//! Sets the fog parameters of the viewport.
//!
//! \param enabled Sets whether fog is enabled or not.
//! \param density Sets the density of the fog (exponential).
//!
void ViewportWidget::setFog ( bool enabled, double density )
{
	if (m_sceneManager) {
		if (enabled)
			m_sceneManager->setFog(Ogre::FOG_EXP, m_fogColor, density);
		else
			m_sceneManager->setFog();
        updateView();
        render();
	}
}

//!
//! Returns the fog color of the scene.
//!
//! \return Fog color of the scene.
//!
QColor ViewportWidget::getFogColor ()
{
    QColor fogColor;
    fogColor.setRedF(m_fogColor.r);
    fogColor.setGreenF(m_fogColor.g);
    fogColor.setBlueF(m_fogColor.b);
    fogColor.setAlphaF(m_fogColor.a);
    return fogColor;
}

//!
//! Sets the fog color of the scene.
//!
//! \param color Fog color of the scene.
//!
void ViewportWidget::setFogColor ( const QColor &color )
{
    m_fogColor.r = color.redF();
    m_fogColor.g = color.greenF();
    m_fogColor.b = color.blueF();
    m_fogColor.a = color.alphaF();
}

//!
//! Returns the background color of the scene.
//!
//! \return Fog background of the scene.
//!
QColor ViewportWidget::getBackgroundColor ()
{
    QColor backgroundColor;
    backgroundColor.setRedF(m_backgroundColor.r);
    backgroundColor.setGreenF(m_backgroundColor.g);
    backgroundColor.setBlueF(m_backgroundColor.b);
    backgroundColor.setAlphaF(m_backgroundColor.a);
    return backgroundColor;
}

//!
//! Sets the background color of the scene.
//!
//! \param color Background color of the scene.
//!
void ViewportWidget::setBackgroundColor ( const QColor &color )
{
    m_backgroundColor.r = color.redF();
    m_backgroundColor.g = color.greenF();
    m_backgroundColor.b = color.blueF();
    m_backgroundColor.a = color.alphaF();
    if (m_viewport)
        m_viewport->setBackgroundColour(m_backgroundColor);
    if (m_stereoViewport0)
        m_stereoViewport0->setBackgroundColour(m_backgroundColor);
    if (m_stereoViewport1)
        m_stereoViewport1->setBackgroundColour(m_backgroundColor);
    if (m_stereoViewport2)
        m_stereoViewport2->setBackgroundColour(m_backgroundColor);
    if (m_stereoViewport3)
        m_stereoViewport3->setBackgroundColour(m_backgroundColor);
    if (m_stereoViewport4)
        m_stereoViewport4->setBackgroundColour(m_backgroundColor);
    render();
}

//!
//! Sets fullscreen mode for the viewport widget.
//!
//! \param fullscreen Flag to control whether to enable fullscreen mode.
//! \param width The width of the viewport in fullscreen mode.
//! \param height The height of the viewport in fullscreen mode.
//!
void ViewportWidget::setFullscreen ( bool fullscreen, int width, int height )
{
    if (m_renderWindow)
        m_renderWindow->setFullscreen(fullscreen, width, height);
}


//!
//! Fills the given variables with stereo parameter values.
//!
//! \param rttWidth The width of the render texture.
//! \param rttHeight The height of the render texture.
//! \param eyeDist The viewer's eye distance.
//! \param fOffsetMultiplier A multiplier to calculate the frustum offset from the camera position.
//! \param nearClip The near clipping plane of the camera.
//! \param farClip The far clippping plane of the camera.
//! \param fov The camera's field of view setting.
//! \param focalLength The camera's focal length setting.
//!
void ViewportWidget::getStereoParameters( double &rttWidth, double &rttHeight,
                                          double &eyeDist, double &fOffsetMultiplier,
                                          double &nearClip, double &farClip, double &fov, double &focalLength )
{
    rttWidth = (double) m_stereoRttWidth;
    rttHeight = (double) m_stereoRttHeight;
    eyeDist = m_stereoEyeDist;
    fOffsetMultiplier = m_stereoFOffsetMultiplier;
    nearClip = m_stereoNearClip;
    farClip = m_stereoFarClip;
    fov = m_stereoFov;
    focalLength = m_stereoFocalLength;
}


//!
//! Sets the stereo parameters to the given values.
//!
//! \param rttWidth The width of the render texture.
//! \param rttHeight The height of the render texture.
//! \param eyeDist The viewer's eye distance.
//! \param fOffsetMultiplier A multiplier to calculate the frustum offset from the camera position.
//! \param nearClip The near clipping plane of the camera.
//! \param farClip The far clippping plane of the camera.
//! \param fov The camera's field of view setting.
//! \param focalLength The camera's focal length setting.
//!
void ViewportWidget::setStereoParameters( const unsigned int rttWidth, const unsigned int rttHeight,
                                          const float eyeDist, const float fOffsetMultiplier,
                                          const float nearClip, const float farClip, const float fov, const float focalLength )
{
    m_stereoRttWidth = rttWidth;
    m_stereoRttHeight = rttHeight;
    m_stereoEyeDist = eyeDist;
    m_stereoFOffsetMultiplier = fOffsetMultiplier;
    m_stereoNearClip = nearClip;
    m_stereoFarClip = farClip;
    m_stereoFov = fov;
    m_stereoFocalLength = focalLength;

    if (m_camera0 && m_camera1 && m_camera2 && m_camera3 && m_camera4) {
        m_camera0->setPosition(-2.0 * m_stereoEyeDist, 0.0, 0.0);
        m_camera0->setFrustumOffset(2.0 * m_stereoFOffsetMultiplier * m_stereoEyeDist, 0.0);
        m_camera0->setFocalLength(m_stereoFocalLength);
        m_camera0->setNearClipDistance(m_stereoNearClip);
        m_camera0->setFarClipDistance(m_stereoFarClip);
        m_camera0->setFOVy(Ogre::Radian(Ogre::Degree(m_stereoFov)));

        m_camera1->setPosition(-1.0 * m_stereoEyeDist, 0.0, 0.0);
        m_camera1->setFrustumOffset(1.0 * m_stereoFOffsetMultiplier * m_stereoEyeDist, 0.0);
        m_camera1->setFocalLength(m_stereoFocalLength);
        m_camera1->setNearClipDistance(m_stereoNearClip);
        m_camera1->setFarClipDistance(m_stereoFarClip);
        m_camera1->setFOVy(Ogre::Radian(Ogre::Degree(m_stereoFov)));

        m_camera2->setPosition(0.0, 0.0, 0.0);
        m_camera2->setFrustumOffset(0.0, 0.0);
        m_camera2->setFocalLength(m_stereoFocalLength);
        m_camera2->setNearClipDistance(m_stereoNearClip);
        m_camera2->setFarClipDistance(m_stereoFarClip);
        m_camera2->setFOVy(Ogre::Radian(Ogre::Degree(m_stereoFov)));

        m_camera3->setPosition(1.0 * m_stereoEyeDist, 0.0, 0.0);
        m_camera3->setFrustumOffset(-1.0 * m_stereoFOffsetMultiplier * m_stereoEyeDist, 0.0);
        m_camera3->setFocalLength(m_stereoFocalLength);
        m_camera3->setNearClipDistance(m_stereoNearClip);
        m_camera3->setFarClipDistance(m_stereoFarClip);
        m_camera3->setFOVy(Ogre::Radian(Ogre::Degree(m_stereoFov)));

        m_camera4->setPosition(2.0 * m_stereoEyeDist, 0.0, 0.0);
        m_camera4->setFrustumOffset(-2.0 * m_stereoFOffsetMultiplier * m_stereoEyeDist, 0.0);
        m_camera4->setFocalLength(m_stereoFocalLength);
        m_camera4->setNearClipDistance(m_stereoNearClip);
        m_camera4->setFarClipDistance(m_stereoFarClip);
        m_camera4->setFOVy(Ogre::Radian(Ogre::Degree(m_stereoFov)));
    }
    render();
}


//!
//! Toggles the stereo mode for the viewport widget.
//!
//! \param enable Flag to enable or disable the stereo mode for the viewport widget.
//!
void ViewportWidget::setStereo ( bool enable )
{
    if (enable) {
        // create the main cameras (stereo)
        m_camera0 = m_sceneManager->createCamera(createUniqueName("camera0"));
        m_camera0->setPosition(-2.0 * m_stereoEyeDist, 0.0, 0.0);
        m_camera0->setFrustumOffset(2.0 * m_stereoFOffsetMultiplier * m_stereoEyeDist, 0.0);
        m_camera0->setNearClipDistance(m_stereoNearClip);
        m_camera0->setFarClipDistance(m_stereoFarClip);
        m_camera0->setFocalLength(m_stereoFocalLength);
        m_camera0->setFOVy(Ogre::Radian(Ogre::Degree(m_stereoFov)));

        m_camera1 = m_sceneManager->createCamera(createUniqueName("camera1"));
        m_camera1->setPosition(-1.0 * m_stereoEyeDist, 0.0, 0.0);
        m_camera1->setFrustumOffset(1.0 * m_stereoFOffsetMultiplier * m_stereoEyeDist, 0.0);
        m_camera1->setNearClipDistance(m_stereoNearClip);
        m_camera1->setFarClipDistance(m_stereoFarClip);
        m_camera1->setFocalLength(m_stereoFocalLength);
        m_camera1->setFOVy(Ogre::Radian(Ogre::Degree(m_stereoFov)));

        m_camera2 = m_sceneManager->createCamera(createUniqueName("camera2"));
        m_camera2->setPosition(0.0, 0.0, 0.0);
        m_camera2->setFrustumOffset(0.0, 0.0);
        m_camera2->setNearClipDistance(m_stereoNearClip);
        m_camera2->setFarClipDistance(m_stereoFarClip);
        m_camera2->setFocalLength(m_stereoFocalLength);
        m_camera2->setFOVy(Ogre::Radian(Ogre::Degree(m_stereoFov)));

        m_camera3 = m_sceneManager->createCamera(createUniqueName("camera3"));
        m_camera3->setPosition(1.0 * m_stereoEyeDist, 0.0, 0.0);
        m_camera3->setFrustumOffset(-1.0 * m_stereoFOffsetMultiplier * m_stereoEyeDist, 0.0);
        m_camera3->setNearClipDistance(m_stereoNearClip);
        m_camera3->setFarClipDistance(m_stereoFarClip);
        m_camera3->setFocalLength(m_stereoFocalLength);
        m_camera3->setFOVy(Ogre::Radian(Ogre::Degree(m_stereoFov)));

        m_camera4 = m_sceneManager->createCamera(createUniqueName("camera4"));
        m_camera4->setPosition(2.0 * m_stereoEyeDist, 0.0, 0.0);
        m_camera4->setFrustumOffset(-2.0 * m_stereoFOffsetMultiplier * m_stereoEyeDist, 0.0);
        m_camera4->setNearClipDistance(m_stereoNearClip);
        m_camera4->setFarClipDistance(m_stereoFarClip);
        m_camera4->setFocalLength(m_stereoFocalLength);
        m_camera4->setFOVy(Ogre::Radian(Ogre::Degree(m_stereoFov)));

        m_cameraSceneNode->attachObject(m_camera0);
        m_cameraSceneNode->attachObject(m_camera1);
        m_cameraSceneNode->attachObject(m_camera2);
        m_cameraSceneNode->attachObject(m_camera3);
        m_cameraSceneNode->attachObject(m_camera4);

        float xStep = 1.0 / 2.0;
        float yStep = 1.0 / 3.0;

        // generate the 5 render textures
        Ogre::TexturePtr viewRtt = Ogre::TextureManager::getSingleton().createManual("StereoViewport/ViewRtt",
            Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME, Ogre::TEX_TYPE_2D,
            m_stereoRttWidth, m_stereoRttHeight, 0, Ogre::PF_R8G8B8, Ogre::TU_RENDERTARGET);
        m_stereoRenderTarget = viewRtt->getBuffer()->getRenderTarget();

        m_stereoCompositor = Ogre::CompositorManager::getSingleton().addCompositor(m_viewport, "StereoViewport", 0);
        m_stereoCompositor->setEnabled(true);

        // create the main viewport0 covering the entire widget
        m_stereoViewport0 = m_stereoRenderTarget->addViewport(m_camera0, 0, 0.0, 2 * yStep, xStep, yStep);
        //m_stereoViewport0->setBackgroundColour(Ogre::ColourValue(0.7f, 0.7f, 0.7f));
        m_stereoViewport0->setBackgroundColour(m_backgroundColor);
        m_stereoViewport0->setVisibilityMask(VF_MAIN_VIEWPORT);

        // create the main viewport1 covering the entire widget
        m_stereoViewport1 = m_stereoRenderTarget->addViewport(m_camera1, 1, xStep, yStep, xStep, yStep);
        //m_stereoViewport1->setBackgroundColour(Ogre::ColourValue(0.7f, 0.7f, 0.7f));
        m_stereoViewport0->setBackgroundColour(m_backgroundColor);
        m_stereoViewport1->setVisibilityMask(VF_MAIN_VIEWPORT);

        // create the main viewport2 covering the entire widget
        m_stereoViewport2 = m_stereoRenderTarget->addViewport(m_camera2, 2, 0.0, yStep, xStep, yStep);
        //m_stereoViewport2->setBackgroundColour(Ogre::ColourValue(0.7f, 0.7f, 0.7f));
        m_stereoViewport2->setBackgroundColour(m_backgroundColor);
        m_stereoViewport2->setVisibilityMask(VF_MAIN_VIEWPORT);

        // create the main viewport3 covering the entire widget
        m_stereoViewport3 = m_stereoRenderTarget->addViewport(m_camera3, 3, xStep, 0.0, xStep, yStep);
        //m_stereoViewport3->setBackgroundColour(Ogre::ColourValue(0.7f, 0.7f, 0.7f));
        m_stereoViewport3->setBackgroundColour(m_backgroundColor);
        m_stereoViewport3->setVisibilityMask(VF_MAIN_VIEWPORT);

        // create the main viewport4 covering the entire widget
        m_stereoViewport4 = m_stereoRenderTarget->addViewport(m_camera4, 4, 0.0, 0.0, xStep, yStep);
        //m_stereoViewport4->setBackgroundColour(Ogre::ColourValue(0.7f, 0.7f, 0.7f));
        m_stereoViewport4->setBackgroundColour(m_backgroundColor);
        m_stereoViewport4->setVisibilityMask(VF_MAIN_VIEWPORT);

        Ogre::MovableObject::setDefaultVisibilityFlags(VF_MAIN_VIEWPORT);

        //OgreManager::ReloadMaterial("StereoViewport/StereoViewport", "General", "StereoViewport.material", true);
    } else {
        if (m_camera0 && m_camera1 && m_camera2 && m_camera3 && m_camera4) {
            m_cameraSceneNode->detachObject(m_camera0);
            m_cameraSceneNode->detachObject(m_camera1);
            m_cameraSceneNode->detachObject(m_camera2);
            m_cameraSceneNode->detachObject(m_camera3);
            m_cameraSceneNode->detachObject(m_camera4);
            Ogre::SceneManager *sceneManager = OgreManager::getSceneManager();
            m_stereoRenderTarget->removeAllViewports();
            sceneManager->destroyCamera(m_camera0);
            sceneManager->destroyCamera(m_camera1);
            sceneManager->destroyCamera(m_camera2);
            sceneManager->destroyCamera(m_camera3);
            sceneManager->destroyCamera(m_camera4);
            m_camera0 = 0;
            m_camera1 = 0;
            m_camera2 = 0;
            m_camera3 = 0;
            m_camera4 = 0;
            m_stereoRenderTarget = 0;
        }
        m_stereoCompositor->setEnabled(false);
        OgreManager::unloadResource(Ogre::TextureManager::getSingletonPtr(), "StereoViewport/ViewRtt");
        OgreManager::unloadMaterials("StereoViewport.material");
        Ogre::CompositorManager::getSingleton().removeCompositor(m_viewport, "StereoViewport");
        //OgreManager::UnloadMaterials("StereoViewport/StereoViewport", "General", "StereoViewport.material", true);
        //m_stereoCompositor->getCompositor()->unload();
    }
}

//!
//! Sets whether only overlays (no geometry) should be displayed in viewport.
//!
//! \param enable If true no geometry will be displayed.
//!
void ViewportWidget::setOverlayOnly ( bool enable )
{
    m_overlayOnly = enable;
}

//!
//! Returns the current number of frames per second rendered in the
//! viewport.
//!
//! \return The current number of frames per second rendered in the viewport.
//!
float ViewportWidget::getFps ()
{
    if (m_renderWindow)
        return m_renderWindow->getLastFPS();
    else
        return 0;
}


//!
//! Returns the number of triangles last rendered in the viewport.
//!
//! \return The number of triangles last rendered in the viewport.
//!
int ViewportWidget::getTriangleCount ()
{
    if (m_renderWindow)
        return (int) m_renderWindow->getTriangleCount();
    else
        return 0;
}


//!
//! Returns the viewing parameter's of the currently active camera.
//!
//! \return The viewing parameter's of the currently active camera.
//!
ViewingParameters * ViewportWidget::getViewingParameters ()
{
    return m_viewingParameters;
}


///
/// Public Slots
///


//!
//! Applies the given viewing parameters to the viewport's viewing
//! parameters.
//!
//! \param viewingParameters The viewing parameters to apply to the viewport.
//!
void ViewportWidget::applyViewingParameters ( ViewingParameters *viewingParameters )
{
    viewingParameters->applyTo(m_viewingParameters);
}


//!
//! Sets the image that should be displayed in the viewport.
//!
//! Only affects viewports that are set to display the content of the stage
//! with the given index.
//!
//! \param stageIndex The index of the stage to which the image belongs.
//! \param image The image that should be displayed in the viewport.
//!
void ViewportWidget::setImage ( unsigned int stageIndex, Ogre::TexturePtr image )
{
    // discard the event if the viewport is set to a different stage
    if (stageIndex != m_stageIndex)
        return;

    if (!image.isNull()) {
        // use the image as a texture in the image overlay material
        Ogre::Pass *technique0pass0 = m_imageMaterial->getTechnique(0)->getPass(0);
        Ogre::TextureUnitState *textureUnitState0 = technique0pass0->getTextureUnitState(0);
        textureUnitState0->setTextureName(image->getName());

        // get image info
        unsigned int imageWidth = (unsigned int) image->getWidth();
        unsigned int imageHeight = (unsigned int) image->getHeight();
        QString pixelFormatName = QString("%1").arg(Ogre::PixelUtil::getFormatName(image->getFormat()).c_str());

        // update the image panel
        if (m_imagePanel) 
            m_imagePanel->setDimensions(imageWidth, imageHeight);
        
        // show the image and mask overlays
        m_showOverlays = true;
        if (m_maskOverlay)
            m_maskOverlay->show();
        if (m_imageOverlay)
            m_imageOverlay->show();

        // update the text information displayed in the HUD
        //m_leftTopHudTextArea->setCaption(QString("Viewport\nImage: %1x%2 pixel\nPixel format: %3").arg(imageWidth).arg(imageHeight).arg(pixelFormatName).toStdString());
    } else {
        // hide the image and mask overlays
        m_showOverlays = false;
        if (m_imageOverlay)
            m_imageOverlay->hide();
        if (m_maskOverlay)
            m_maskOverlay->hide();

        // update the text information displayed in the HUD
        //m_leftTopHudTextArea->setCaption("Viewport");
    }

    // force a resize event to update the panel overlays' positions and dimensions
    resizeEvent(new QResizeEvent(size(), size()));
}


//!
//! Resets the viewport's view settings.
//!
void ViewportWidget::homeView ()
{
    m_viewingParameters->reset();
    updateView();
    render();
}


//!
//! Activates or deactivates wireframe view in the viewport.
//!
//! \param wireframe Flag that controls wireframe view in the viewport.
//!
void ViewportWidget::setWireframe ( bool wireframe )
{
    m_wireframe = wireframe;

    // set the Ogre polygon mode according to the flag's value
    Ogre::PolygonMode polygonMode;
    if (m_wireframe)
        polygonMode = Ogre::PM_WIREFRAME;
    else
        polygonMode = Ogre::PM_SOLID;

    // update the viewport's camera
    if (m_camera)
        m_camera->setPolygonMode(polygonMode);

    // update the viewport's cameras (stere0)
    if (m_camera0 && m_camera1 && m_camera2 && m_camera3 && m_camera4) {
        m_camera0->setPolygonMode(polygonMode);
        m_camera1->setPolygonMode(polygonMode);
        m_camera2->setPolygonMode(polygonMode);
        m_camera3->setPolygonMode(polygonMode);
        m_camera4->setPolygonMode(polygonMode);
    }

    render();
}


//!
//! Activates or deactivates the omission of polygons facing away from the
//! camera.
//!
//! \param backfaceCulling Flag that controls the culling of polygons facing away from the camera.
//!
void ViewportWidget::setBackfaceCulling ( bool backfaceCulling )
{
    m_backfaceCulling = backfaceCulling;

    // set the Ogre culling mode according to the flag's value
    Ogre::CullingMode cullingMode;
    if (m_backfaceCulling)
        cullingMode = Ogre::CULL_CLOCKWISE;
    else
        cullingMode = Ogre::CULL_NONE;

    // iterate over all materials managed by the material manager
    Ogre::MaterialManager *materialManager = Ogre::MaterialManager::getSingletonPtr();
    if (materialManager) {
        Ogre::ResourceManager::ResourceMapIterator it = materialManager->getResourceIterator();
        while (it.hasMoreElements()) {
            Ogre::MaterialPtr currentMaterial = it.getNext();
            // set the culling mode for the material
            if (!currentMaterial.isNull() && currentMaterial->getGroup() != "ViewportMaterials")
                currentMaterial->getTechnique(0)->getPass(0)->setCullingMode(cullingMode);
        }
    }

    render();
}


//!
//! Toggles the display of the point of origin in the viewport.
//!
void ViewportWidget::toggleOrigin ()
{
    m_showOrigin = !m_showOrigin;
    render();
}


//!
//! Toggles the display of the center point in the viewport.
//!
void ViewportWidget::toggleCenterPoint ()
{
    m_showCenterPoint = !m_showCenterPoint;
    render();
}


//!
//! Toggles the display of the orientation indicator in the viewport.
//!
void ViewportWidget::toggleOrientationIndicator ()
{
    m_showOrientationIndicator = !m_showOrientationIndicator;
    render();
}


//!
//! Toggles the display of the coordinate grid in the viewport.
//!
void ViewportWidget::toggleGrid ()
{
    m_showGrid = !m_showGrid;
    render();
}


///
/// Protected Events
///


//!
//! Handles resize events for the widget.
//!
//! \param event The description of the resize event.
//!
void ViewportWidget::resizeEvent ( QResizeEvent *event )
{
    if (!m_renderWindow)
        return;

    int width = event->size().width();
    int height = event->size().height();

    // store the size of the widget in the viewing parameters
    m_viewingParameters->setCanvasSize(width, height);

#if defined(Q_WS_X11)
    m_renderWindow->resize(width, height);
#endif
    m_renderWindow->windowMovedOrResized();

    // set the camera aspect ratio to match the viewport
    if (m_camera)
        m_camera->setAspectRatio((Ogre::Real) width / height);

    // set the cameras aspects ratios to match the viewport (stereo)
    if (m_camera0 && m_camera1 && m_camera2 && m_camera3 && m_camera4) {
        m_camera0->setAspectRatio((Ogre::Real) width / height);
        m_camera1->setAspectRatio((Ogre::Real) width / height);
        m_camera2->setAspectRatio((Ogre::Real) width / height);
        m_camera3->setAspectRatio((Ogre::Real) width / height);
        m_camera4->setAspectRatio((Ogre::Real) width / height);
    }

    // update the position and size of the orientation indicator viewport
    if (m_orientationIndicatorViewport)
        m_orientationIndicatorViewport->setDimensions(0, 1.0 - 40.0 / height, 40.0 / width, 40.0 / height);

    if (m_imageOverlay && m_imageOverlay->isVisible()) {
        // update image panel
        Ogre::Real imageWidth = m_imagePanel->getWidth();
        Ogre::Real imageHeight = m_imagePanel->getHeight();
        m_imagePanel->setPosition(-imageWidth / 2, -imageHeight / 2);

        // set the masks' width and height
        Ogre::Real verticalMaskWidth = (width - imageWidth) / 2;
        Ogre::Real horizontalMaskHeight = (height - imageHeight) / 2;

        if (m_topMaskPanel && m_bottomMaskPanel) {
        // check if horizontal mask panels are needed
        if (horizontalMaskHeight > 0) {
            // update top mask panel
            m_topMaskPanel->setWidth(width);
            m_topMaskPanel->setHeight(horizontalMaskHeight);
            // update bottom mask panel
            m_bottomMaskPanel->setWidth(width);
            m_bottomMaskPanel->setHeight(horizontalMaskHeight);
            m_bottomMaskPanel->setTop(height - horizontalMaskHeight);
            // show the mask panels if they're invisible
            if (!m_topMaskPanel->isVisible())
                m_topMaskPanel->show();
            if (!m_bottomMaskPanel->isVisible())
                m_bottomMaskPanel->show();
        } else {
            // hide the mask panels if they're visible
            if (m_topMaskPanel->isVisible())
                m_topMaskPanel->hide();
            if (m_bottomMaskPanel->isVisible())
                m_bottomMaskPanel->hide();
        }
        }
        
        if (m_leftMaskPanel && m_rightMaskPanel) {
            // check if vertical mask panels are needed
            if (verticalMaskWidth > 0) {
                // update the left mask panel
                m_leftMaskPanel->setWidth(verticalMaskWidth);
                m_leftMaskPanel->setHeight(imageHeight);
                m_leftMaskPanel->setTop(horizontalMaskHeight);
                // update the right mask panel
                m_rightMaskPanel->setWidth(verticalMaskWidth);
                m_rightMaskPanel->setHeight(imageHeight);
                m_rightMaskPanel->setLeft(width - verticalMaskWidth);
                m_rightMaskPanel->setTop(horizontalMaskHeight);
                // show the mask panels if they're invisible
                if (!m_leftMaskPanel->isVisible())
                    m_leftMaskPanel->show();
                if (!m_rightMaskPanel->isVisible())
                    m_rightMaskPanel->show();
            } else {
                // hide the mask panels if they're visible
                if (m_leftMaskPanel->isVisible())
                    m_leftMaskPanel->hide();
                if (m_rightMaskPanel->isVisible())
                    m_rightMaskPanel->hide();
            }
        }
    }

    render();
}


//!
//! Handles paint events for the widget.
//!
//! \param event The description of the paint event.
//!
void ViewportWidget::paintEvent ( QPaintEvent *event )
{
    Q_UNUSED(event)

    render();
}


//!
//! Handles mouse press events for the widget.
//!
//! \param event The description of the mouse event.
//!
void ViewportWidget::mousePressEvent ( QMouseEvent *event )
{
    m_lastPosition = event->pos();

    if (event->button() == Qt::MidButton)
        setCursor(Qt::SizeAllCursor);
    else
        if (m_viewMode) {
            if (event->button() == Qt::LeftButton)
                setCursor(Qt::ClosedHandCursor);
            else if (event->button() == Qt::RightButton)
                setCursor(Qt::SizeFDiagCursor);
        } else {
            if (event->button() == Qt::LeftButton) {
                Ogre::SceneNode *clickedSceneNode = getClickedSceneNode(event->posF());
                QString clickedSceneNodeName;
                if (clickedSceneNode)
                    clickedSceneNodeName = QString(clickedSceneNode->getName().c_str());
                // notify connected objects that an object in the viewport has been clicked
                emit objectSelected(clickedSceneNodeName, event->modifiers() & Qt::ControlModifier);
                // update the viewport to highlight selected objects
                render();
            }
        }

    QWidget::mousePressEvent(event);
}


//!
//! Handles mouse move events for the widget.
//!
//! \param event The description of the mouse event.
//!
void ViewportWidget::mouseMoveEvent ( QMouseEvent *event )
{
    if (qApp->focusWidget() != this) {
        setViewMode(false);
        setFocus(Qt::MouseFocusReason);
    }

    double dx = double(event->x() - m_lastPosition.x()) / height();
    double dy = double(event->y() - m_lastPosition.y()) / height();
    m_lastPosition = event->pos();

    if (m_viewMode && event->buttons() & Qt::LeftButton) {
        // rotate view
        m_viewingParameters->rotate(180.0 * dx, 180.0 * dy);
        updateView();
        render();
    } else if (event->buttons() & Qt::MidButton) {
        // translate view
        m_viewingParameters->translate(dx, dy);
        updateView();
        render();
    } else if (m_viewMode && event->buttons() & Qt::RightButton) {
        // dolly view
        double dmax;
        if (qAbs(dx) > qAbs(dy))
            dmax = -dx;
        else
            dmax = -dy;
        m_viewingParameters->dolly(dmax);
        updateView();
        render();
    }

    QWidget::mouseMoveEvent(event);
}


//!
//! Handles mouse release events for the widget.
//!
//! \param event The description of the mouse event.
//!
void ViewportWidget::mouseReleaseEvent ( QMouseEvent *event )
{
    if (m_viewMode)
        setCursor(Qt::OpenHandCursor);
    else
        setCursor(Qt::ArrowCursor);

    QWidget::mouseReleaseEvent(event);
}


//!
//! Handles mouse wheel events for the widget.
//!
//! \param event The description of the mouse wheel event.
//!
void ViewportWidget::wheelEvent ( QWheelEvent *event )
{
    m_viewingParameters->dolly(event->delta(), true);
    updateView();
    render();
}


///
/// Private Functions
///


//!
//! Creates a unique name from the given object name by adding the viewport
//! widget's index.
//!
Ogre::String ViewportWidget::createUniqueName ( const QString &objectName )
{
    return QString("viewport%1_%2").arg(s_viewportIndex).arg(objectName).toStdString();
}


//!
//! Creates the OGRE render window.
//!
//! \return True if creating the OGRE render window succeeded, otherwise False.
//!
bool ViewportWidget::createRenderWindow ()
{
    Ogre::Root *ogreRoot = Ogre::Root::getSingletonPtr();
    if (!ogreRoot)
        return false;

    // create a name/value list with miscellaneous parameters for the render window according to current OS
    Ogre::NameValuePairList miscParameters;
    miscParameters["FSAA"] = "4";
    if (!m_fullscreen) {
#if defined(Q_WS_X11)
        QWidget *q_parent = dynamic_cast<QWidget *>(parent());
        QX11Info xInfo = x11Info();
        miscParameters["parentWindowHandle"] = Ogre::StringConverter::toString((unsigned long) xInfo.display())
                                       + ":" + Ogre::StringConverter::toString((unsigned int) xInfo.screen())
                                       + ":" + Ogre::StringConverter::toString((unsigned long) q_parent->winId());
#else
        miscParameters["externalWindowHandle"] = Ogre::StringConverter::toString((size_t) (HWND) winId());
        miscParameters["FSAA"] = "4";
#endif
    }

    // create the render window
    int windowWidth = width();
    int windowHeight = height();
    if (m_fullscreen) {
        windowWidth = m_width;
        windowHeight = m_height;
    }
    try {
        m_renderWindow = ogreRoot->createRenderWindow(createUniqueName("renderWindow"), windowWidth, windowHeight, m_fullscreen, &miscParameters);
    }
    catch (Ogre::RenderingAPIException e) {
        m_renderWindow = 0;
        Log::error(QString("The render window could not be created: %1").arg(e.getDescription().c_str()), "ViewportWidget::createRenderWindow");
    }

//#if defined(Q_WS_X11)
//    if (m_renderWindow) {
//        WId windowId;
//        m_renderWindow->getCustomAttribute("GLXWINDOW", &windowId);
//
//        // take over the OGRE-created window
//        QWidget::create(windowId);
//
//        m_renderWindow->reposition(x(),y());
//    }
//#endif

    Ogre::TextureManager::getSingleton().setDefaultNumMipmaps(5);
    Ogre::MaterialManager::getSingleton().setDefaultTextureFiltering(Ogre::TFO_ANISOTROPIC);
    Ogre::MaterialManager::getSingleton().setDefaultAnisotropy(2);

    return m_renderWindow != 0;
}


//!
//! Initializes the resources that will be available to OGRE.
//!
void ViewportWidget::initializeResources ()
{
    // define pointers used temporarily for creating the following materials
    Ogre::Pass *technique0pass0;
    Ogre::TextureUnitState *textureUnitState;    
    
    // create material for image overlay
    m_imageMaterial = Ogre::MaterialManager::getSingletonPtr()->create(createUniqueName("imageMaterial"), "ViewportMaterials");
    technique0pass0 = m_imageMaterial->getTechnique(0)->getPass(0);
    technique0pass0->setLightingEnabled(false);
    technique0pass0->setSceneBlending(Ogre::SBT_TRANSPARENT_ALPHA);
    textureUnitState = technique0pass0->createTextureUnitState();
    textureUnitState->setNumMipmaps(0);
    
    if (s_redMaterial.isNull()) {
        // create color materials
        s_redMaterial = createColorMaterial("ViewportMaterials", "redMaterial", Ogre::ColourValue(1.0f, 0.0f, 0.0f), true);
        s_greenMaterial = createColorMaterial("ViewportMaterials", "greenMaterial", Ogre::ColourValue::Green, true);
        s_blueMaterial = createColorMaterial("ViewportMaterials", "blueMaterial", Ogre::ColourValue::Blue, true);
        s_grayMaterial = createColorMaterial("ViewportMaterials", "grayMaterial", Ogre::ColourValue(0.5f, 0.5f, 0.5f, 0.5f), true);
        s_blackMaterial = createColorMaterial("ViewportMaterials", "blackMaterial", Ogre::ColourValue::Black, true);
        s_defaultMaterial = createColorMaterial("ViewportMaterials", "defaultMaterial", Ogre::ColourValue(0.5f, 0.5f, 0.5f));
        s_nullMaterial = createColorMaterial("ViewportMaterials", "nullMaterial", Ogre::ColourValue(0.0f, 0.74902f, 0.0f), true);

        //// NILZ: TODO: Insert stage code here
        //// create material for image overlay
        //m_imageMaterial = Ogre::MaterialManager::getSingletonPtr()->create("imageMaterial", "ViewportMaterials");
        //technique0pass0 = m_imageMaterial->getTechnique(0)->getPass(0);
        //technique0pass0->setLightingEnabled(false);
        //technique0pass0->setSceneBlending(Ogre::SBT_TRANSPARENT_ALPHA);
        //textureUnitState = technique0pass0->createTextureUnitState();
        //textureUnitState->setNumMipmaps(0);

        // create material for mask overlays
        s_maskMaterial = Ogre::MaterialManager::getSingletonPtr()->create("maskMaterial", "ViewportMaterials");
        technique0pass0 = s_maskMaterial->getTechnique(0)->getPass(0);
        technique0pass0->setLightingEnabled(false);
        technique0pass0->setSceneBlending(Ogre::SBT_TRANSPARENT_ALPHA);
        textureUnitState = technique0pass0->createTextureUnitState();
        textureUnitState->setColourOperationEx(Ogre::LBX_SOURCE1, Ogre::LBS_MANUAL, Ogre::LBS_CURRENT, Ogre::ColourValue::Black);
        textureUnitState->setAlphaOperation(Ogre::LBX_SOURCE1, Ogre::LBS_MANUAL, Ogre::LBS_CURRENT, 0.4);
    }

    Ogre::ResourceGroupManager::getSingleton().initialiseAllResourceGroups();
}


//!
//! Creates basic viewport objects like the viewing camera, the head light,
//! coordinate axes and the grid.
//!
void ViewportWidget::createViewportObjects ()
{
    if (!m_sceneManager) {
        Log::error("The OGRE scene manager could not be obtained.", "ViewportWidget::createViewportObjects");
        return;
    }

    // create a faint ambient light
    m_sceneManager->setAmbientLight(Ogre::ColourValue(0.1f, 0.1f, 0.1f));

    // create the parent scene node for the scene node tree to which all viewport-specific objects will be attached
    m_viewportObjectsSceneNode = m_sceneManager->getRootSceneNode()->createChildSceneNode(createUniqueName("viewportObjects"));

    // create the main viewport camera
    m_camera = m_sceneManager->createCamera(createUniqueName("camera"));
    m_cameraSceneNode = m_viewportObjectsSceneNode->createChildSceneNode(createUniqueName("cameraSceneNode"));
    m_cameraSceneNode->attachObject(m_camera);

    // set additional camera info
    CameraInfo *cameraInfo = new CameraInfo();
    cameraInfo->width = width();
    cameraInfo->height = height();
    //m_camera->Ogre::MovableObject::setUserAny(Ogre::Any(cameraInfo));
    Ogre::MovableObject *movableObject = dynamic_cast<Ogre::MovableObject *>(m_camera);
    movableObject->setUserAny(Ogre::Any(cameraInfo));

    // create the orientation indicator viewport camera
    m_orientationIndicatorCamera = m_sceneManager->createCamera(createUniqueName("orientationIndicatorCamera"));
    m_orientationIndicatorCamera->setAspectRatio(1.0);
    m_orientationIndicatorCamera->setNearClipDistance(0.01);
    m_orientationIndicatorCamera->setFarClipDistance(100.0);
    m_orientationIndicatorCamera->setFOVy(Ogre::Radian(Ogre::Degree(35)));
    m_orientationIndicatorCameraSceneNode = m_viewportObjectsSceneNode->createChildSceneNode(createUniqueName("orientationIndicatorCameraSceneNode"));
    m_orientationIndicatorCameraSceneNode->attachObject(m_orientationIndicatorCamera);

    // create the viewing camera's light
    m_headLight = m_sceneManager->createLight(createUniqueName("headLight"));
    m_headLight->setType(Ogre::Light::LT_POINT);
    m_headLight->setDiffuseColour(Ogre::ColourValue::White);
    m_headLight->setSpecularColour(Ogre::ColourValue::White);
    m_cameraSceneNode->attachObject(m_headLight);

    // set the default visibility flags for movable objects so that they show up in the main viewport
    Ogre::MovableObject::setDefaultVisibilityFlags(VF_MAIN_VIEWPORT);

    // create the main viewport covering the entire widget
    m_viewport = m_renderWindow->addViewport(m_camera);
	//Ogre::ColourValue backgroundColor(0.7f, 0.7f, 0.7f);
	m_viewport->setBackgroundColour(m_backgroundColor);
	//m_sceneManager->setFog(Ogre::FOG_LINEAR, backgroundColor, 0.0, 1, 10);
	//m_sceneManager->setFog(Ogre::FOG_EXP, backgroundColor, 0.01);
    m_viewport->setVisibilityMask(VF_MAIN_VIEWPORT);

    // create the orientation indicator viewport covering only a corner of the widget
    m_orientationIndicatorViewport = m_renderWindow->addViewport(m_orientationIndicatorCamera, 1, 0.1, 0.1, 0.1, 0.1);
    m_orientationIndicatorViewport->setOverlaysEnabled(false);
    m_orientationIndicatorViewport->setVisibilityMask(VF_ORIENTATION_VIEWPORT);
    // disable clearing the color buffer so that the orientation indicator viewport acts as a transparent overlay on top of the main viewport
    m_orientationIndicatorViewport->setClearEveryFrame(false, Ogre::FBT_COLOUR);
    // enable clearing the depth buffer so that elements from the main viewport do not overlap elements in the orientation indicator viewport
    m_orientationIndicatorViewport->setClearEveryFrame(true, Ogre::FBT_DEPTH);

    // create the origin node
    m_originSceneNode = m_viewportObjectsSceneNode->createChildSceneNode(createUniqueName("originNode"));
    createAxes("origin", m_originSceneNode);

    // create the center point node
    m_centerPointSceneNode = m_viewportObjectsSceneNode->createChildSceneNode(createUniqueName("centerPointNode"));
    createAxes("centerPoint", m_centerPointSceneNode);

    // create the orientation indicator with a special visibility flag so that it only shows up in the orientation indicator viewport
    m_orientationIndicatorSceneNode = m_viewportObjectsSceneNode->createChildSceneNode(createUniqueName("orientationIndicatorNode"));
    createAxes("orientationIndicator", m_orientationIndicatorSceneNode, VF_ORIENTATION_VIEWPORT);

    // create the coordinate grid
    m_gridSceneNode = m_viewportObjectsSceneNode->createChildSceneNode(createUniqueName("gridNode"));
    createGrid(m_gridSceneNode);

    // stereo compositor.
    //m_stereoCompositor = Ogre::CompositorManager::getSingleton().addCompositor(m_viewport, "StereoViewport", 0);
}


//!
//! Creates overlay layers for visual components that will be rendered on
//! top of the main scene layer.
//!
void ViewportWidget::createOverlays ()
{
    Ogre::OverlayManager *overlayManager = Ogre::OverlayManager::getSingletonPtr();
    int viewportWidth = overlayManager->getViewportWidth();

    // create the label text area overlay element
    m_leftTopHudTextArea = dynamic_cast<Ogre::TextAreaOverlayElement *>(overlayManager->createOverlayElement("TextArea", createUniqueName("leftTopHudTextArea")));
    //m_leftTopHudTextArea->setCaption("Viewport");
    m_leftTopHudTextArea->setMetricsMode(Ogre::GMM_RELATIVE);
    m_leftTopHudTextArea->setDimensions(1.0, 1.0);
    m_leftTopHudTextArea->setMetricsMode(Ogre::GMM_PIXELS);
    m_leftTopHudTextArea->setPosition(10, 10);
    m_leftTopHudTextArea->setFontName("BlueHighway");
    m_leftTopHudTextArea->setParameter("char_height", "16");
    m_leftTopHudTextArea->setColour(Ogre::ColourValue::White);

    // create the label overlay element
    m_leftTopHudPanel = dynamic_cast<Ogre::PanelOverlayElement *>(overlayManager->createOverlayElement("Panel", createUniqueName("leftTopHudPanel")));
    m_leftTopHudPanel->setMetricsMode(Ogre::GMM_PIXELS);
    m_leftTopHudPanel->setDimensions(200, 100);
    m_leftTopHudPanel->setPosition(0, 0);
    m_leftTopHudPanel->addChild(m_leftTopHudTextArea);

    // create the image overlay element
    m_imagePanel = dynamic_cast<Ogre::PanelOverlayElement *>(overlayManager->createOverlayElement("Panel", createUniqueName("imagePanel")));
    m_imagePanel->setMetricsMode(Ogre::GMM_PIXELS);
    m_imagePanel->setHorizontalAlignment(Ogre::GHA_CENTER);
    m_imagePanel->setVerticalAlignment(Ogre::GVA_CENTER);
    m_imagePanel->setMaterialName(m_imageMaterial->getName());

    // create the left mask panel
    m_leftMaskPanel = dynamic_cast<Ogre::PanelOverlayElement *>(overlayManager->createOverlayElement("Panel", createUniqueName("leftMaskPanel")));
    m_leftMaskPanel->setMetricsMode(Ogre::GMM_PIXELS);
    m_leftMaskPanel->setMaterialName(s_maskMaterial->getName());

    // create the right mask panel
    m_rightMaskPanel = dynamic_cast<Ogre::PanelOverlayElement *>(overlayManager->createOverlayElement("Panel", createUniqueName("rightMaskPanel")));
    m_rightMaskPanel->setMetricsMode(Ogre::GMM_PIXELS);
    m_rightMaskPanel->setMaterialName(s_maskMaterial->getName());

    // create the top mask panel
    m_topMaskPanel = dynamic_cast<Ogre::PanelOverlayElement *>(overlayManager->createOverlayElement("Panel", createUniqueName("topMaskPanel")));
    m_topMaskPanel->setMetricsMode(Ogre::GMM_PIXELS);
    m_topMaskPanel->setMaterialName(s_maskMaterial->getName());

    // create the bottom mask panel
    m_bottomMaskPanel = dynamic_cast<Ogre::PanelOverlayElement *>(overlayManager->createOverlayElement("Panel", createUniqueName("bottomMaskPanel")));
    m_bottomMaskPanel->setMetricsMode(Ogre::GMM_PIXELS);
    m_bottomMaskPanel->setMaterialName(s_maskMaterial->getName());

    // create the label overlay
    m_labelOverlay = overlayManager->create(createUniqueName("labelOverlay"));
    m_labelOverlay->setZOrder(3);
    m_labelOverlay->add2D(m_leftTopHudPanel);
    m_labelOverlay->show();

    // create the image overlay
    m_imageOverlay = overlayManager->create(createUniqueName("imageOverlay"));
    m_imageOverlay->setZOrder(2);
    m_imageOverlay->add2D(m_imagePanel);

    // create the mask overlay
    m_maskOverlay = overlayManager->create(createUniqueName("maskOverlay"));
    m_maskOverlay->setZOrder(1);
    m_maskOverlay->add2D(m_leftMaskPanel);
    m_maskOverlay->add2D(m_rightMaskPanel);
    m_maskOverlay->add2D(m_topMaskPanel);
    m_maskOverlay->add2D(m_bottomMaskPanel);
}


//!
//! Redraws the OGRE scene into the viewport widget.
//!
void ViewportWidget::render ()
{
    Ogre::Root *ogreRoot = OgreManager::getRoot();
    if (!ogreRoot || !m_renderWindow || !m_sceneManager)
        return;

    ogreRoot->_fireFrameStarted();

    // make the objects that are exclusive to this viewport visible    
    m_viewportObjectsSceneNode->setVisible(true); // also makes all child objects visible
    m_originSceneNode->setVisible(m_showOrigin);
    m_centerPointSceneNode->setVisible(m_showCenterPoint);
    m_orientationIndicatorSceneNode->setVisible(m_showOrientationIndicator);
    m_gridSceneNode->setVisible(m_showGrid);

    // hide all geometry only display overlay
    if (m_sceneManager->hasSceneNode("sceneRoot")) {    
        Ogre::SceneNode *sceneRoot = m_sceneManager->getSceneNode("sceneRoot");
        if (m_overlayOnly) {
            // make the objects that are exclusive to this viewport visible    
            sceneRoot->setVisible(false);
            // make the objects that are exclusive to this viewport visible    
            m_viewportObjectsSceneNode->setVisible(true); // also makes all child objects visible
            m_originSceneNode->setVisible(false);
            m_centerPointSceneNode->setVisible(false);
            m_orientationIndicatorSceneNode->setVisible(false);
            m_gridSceneNode->setVisible(false);
        }
        else {
            sceneRoot->setVisible(true);
        }
    }
    emit renderingTriggered();
    
    if (m_showOverlays) {
        m_imageOverlay->show();
        m_maskOverlay->show();
        m_leftTopHudTextArea->show();
    }

    // TODO: (stages) make the objects contained in the stage that this viewport is set to visible
    // update viewport
    m_renderWindow->update(false);
	//ogreRoot->_updateAllRenderTargets();
    // give client app opportunity to use queued GPU time
    ogreRoot->_fireFrameRenderingQueued();
    // block for final swap
    m_renderWindow->swapBuffers(true);
    

    // update stereo render target
    if (m_stereoRenderTarget)
        m_stereoRenderTarget->update();

    m_imageOverlay->hide();
    m_maskOverlay->hide();
    m_leftTopHudTextArea->hide();

    // TODO: (stages) make the objects contained in the stage that this viewport is set to invisible

    // make the objects that are exclusive to this viewport invisible
    m_viewportObjectsSceneNode->setVisible(false); // also makes all child objects invisible

    ogreRoot->_fireFrameEnded();

    // NILZ: DEBUG: write to file for ViewportWidget
    //if (m_stereoRenderTarget)
    //    m_stereoRenderTarget->writeContentsToFile("5ViewTestRender.png");
    //if (!m_tmpTexture.isNull()) {
    //    Ogre::RenderTexture *renderTexture = m_tmpTexture->getBuffer()->getRenderTarget();
    //    renderTexture->writeContentsToFile("RenderTexture.png");
    //}

    //m_dbgColor += 0.01;
    //if (m_dbgColor > 1.0)
    //    m_dbgColor = 0.0;
    //Ogre::MaterialPtr material = Ogre::MaterialPtr(Ogre::MaterialManager::getSingleton().getByName("StereoViewport/StereoViewport"));
    //if (!material.isNull()) {
    //    Ogre::GpuProgramParametersSharedPtr fparams = material->getTechnique(0)->getPass(0)->getFragmentProgramParameters();
    //    fparams->setNamedConstant("test", Ogre::Real(m_dbgColor));
    //}
}


//!
//! Calculates the transformation of the viewport's camera according to the
//! current viewing angles, the viewing radius, and the center of view.
//!
void ViewportWidget::updateView ()
{
    m_viewingParameters->update();

    // apply the viewing parameters to the viewport's camera
    m_viewingParameters->applyTo(m_cameraSceneNode);
    m_viewingParameters->applyTo(m_camera);

    if (m_stereo) {
        //m_viewingParameters->applyTo(m_camera0);
        //m_viewingParameters->applyTo(m_camera1);
        //m_viewingParameters->applyTo(m_camera2);
        //m_viewingParameters->applyTo(m_camera3);
        //m_viewingParameters->applyTo(m_camera4);
    }

    // update the indicator viewport's camera
    m_viewingParameters->applyTo(m_orientationIndicatorCameraSceneNode, true);

    // update the center point node's transformations
    m_viewingParameters->applyTo(m_centerPointSceneNode, false, true);

    // notify connected objects that the viewing parameters have changed
    emit viewChanged(m_viewingParameters);
}


//!
//! Creates a self-illuminating material with the given name and color under
//! the given group name.
//!
//! \param group The name of the resource group to attach the material to.
//! \param name The unique name for the material.
//! \param color The color for the material.
//! \param wireframe Flag to control whether the material will display objects in wireframe mode.
//! \return The new color material.
//!
Ogre::MaterialPtr ViewportWidget::createColorMaterial ( const Ogre::String &group, const Ogre::String &name, const Ogre::ColourValue &color, bool wireframe /* = false */ )
{
    Ogre::MaterialPtr result = Ogre::MaterialManager::getSingleton().create(name, group);
    result->setReceiveShadows(false);
    Ogre::Technique *technique0 = result->getTechnique(0);
    technique0->setShadingMode(Ogre::SO_PHONG);
    technique0->setLightingEnabled(true);
    Ogre::Pass *pass0 = technique0->getPass(0);
    if (wireframe) {
        pass0->setPolygonMode(Ogre::PM_WIREFRAME);
        pass0->setDiffuse(Ogre::ColourValue::Black);
        pass0->setSelfIllumination(color);
    } else {
        pass0->setPolygonMode(Ogre::PM_SOLID);
        pass0->setDiffuse(color);
        pass0->setSelfIllumination(Ogre::ColourValue::Black);
    }
    pass0->setDepthCheckEnabled(true);
    pass0->setCullingMode(Ogre::CULL_NONE);
    pass0->setAmbient(Ogre::ColourValue::Black);
    pass0->setSpecular(Ogre::ColourValue::Black);
    return result;
}


//!
//! Creates three manual objects for X, Y and Z axes with the given name and
//! attaches them to the given parent scene node.
//! The axes will each have a length of 1.
//!
//! \param name The prefix for the axes objects to create.
//! \param parentNode The scene node to which to attach the axes objects.
//! \param visibilityFlags The visibility flags to apply to the axes objects.
//!
void ViewportWidget::createAxes ( const QString &name, Ogre::SceneNode *parentNode, Ogre::uint32 visibilityFlags /* = VF_MAIN_VIEWPORT */ )
{
    if (!m_sceneManager)
        return;

    Ogre::ManualObject *xAxis = m_sceneManager->createManualObject(createUniqueName(name + "_xAxis"));
    xAxis->begin("redMaterial", Ogre::RenderOperation::OT_LINE_LIST);
    xAxis->position(0, 0, 0);
    xAxis->position(1, 0, 0);
    xAxis->end();
    xAxis->setVisibilityFlags(visibilityFlags);

    Ogre::ManualObject *yAxis = m_sceneManager->createManualObject(createUniqueName(name + "_yAxis"));
    yAxis->begin("greenMaterial", Ogre::RenderOperation::OT_LINE_LIST);
    yAxis->position(0, 0, 0);
    yAxis->position(0, 1, 0);
    yAxis->end();
    yAxis->setVisibilityFlags(visibilityFlags);

    Ogre::ManualObject *zAxis = m_sceneManager->createManualObject(createUniqueName(name + "_zAxis"));
    zAxis->begin("blueMaterial", Ogre::RenderOperation::OT_LINE_LIST);
    zAxis->position(0, 0, 0);
    zAxis->position(0, 0, 1);
    zAxis->end();
    zAxis->setVisibilityFlags(visibilityFlags);

    parentNode->attachObject(xAxis);
    parentNode->attachObject(yAxis);
    parentNode->attachObject(zAxis);
}


//!
//! Creates a coordinate grid and attaches it to the given parent scene node.
//!
//! \param parentNode The scene node to which to attach the grid object.
//!
void ViewportWidget::createGrid ( Ogre::SceneNode *parentNode )
{
    if (!m_sceneManager)
        return;

    int numRows = 32;
    double size = 16.0;
    double halfSize = size / 2.0f;
    double sizeStep = size / numRows;
    double offset;

    Ogre::ManualObject *grid = m_sceneManager->createManualObject(createUniqueName("grid"));
    grid->begin("grayMaterial", Ogre::RenderOperation::OT_LINE_LIST);
    for (int i = 0; i <= numRows; ++i)
        if (i != numRows / 2) {
            offset = i * sizeStep;
            // add a line parallel to the X axis
            grid->position(-halfSize, 0, -halfSize + offset);
            grid->position(+halfSize, 0, -halfSize + offset);
            // add a line parallel to the Z axis
            grid->position(-halfSize + offset, 0, -halfSize);
            grid->position(-halfSize + offset, 0, +halfSize);
        }
    grid->end();

    Ogre::ManualObject *gridCenter = m_sceneManager->createManualObject(createUniqueName("gridCenter"));
    gridCenter->begin("blackMaterial", Ogre::RenderOperation::OT_LINE_LIST);
    // add a line through the X axis
    gridCenter->position(-halfSize, 0, 0);
    gridCenter->position(+halfSize, 0, 0);
    // add a line through the Z axis
    gridCenter->position(0, 0, -halfSize);
    gridCenter->position(0, 0, +halfSize);
    gridCenter->end();

    parentNode->attachObject(grid);
    parentNode->attachObject(gridCenter);
}


//!
//! Returns the scene node that is located under the given mouse position in
//! the viewport.
//!
//! \param mousePosition The position of the mouse pointer over the viewport widget.
//! \return The clicked scene node or 0 if no scene node was clicked.
//!
Ogre::SceneNode * ViewportWidget::getClickedSceneNode ( const QPointF &mousePosition )
{
    if (!m_sceneManager || !m_renderWindow)
        return 0;

    Ogre::SceneNode *result = 0;
    Ogre::Ray mouseRay = m_camera->getCameraToViewportRay(mousePosition.x() / width(), mousePosition.y() / height());
    Ogre::RaySceneQuery *rayQuery = m_sceneManager->createRayQuery(mouseRay);
    rayQuery->setSortByDistance(true);
    Ogre::RaySceneQueryResult &rayQueryResult = rayQuery->execute();
    unsigned int i = 0;
    while (i < rayQueryResult.size() && !result) {
        Ogre::MovableObject *movableObject = rayQueryResult.at(i).movable;
        if (movableObject && movableObject->getVisible() && movableObject->getParentSceneNode() != m_gridSceneNode)
            result = movableObject->getParentSceneNode();
        else
            ++i;
    }
    return result;
}
