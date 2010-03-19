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
//! \file "ViewportWidget.h"
//! \brief Header file for ViewportWidget class.
//!
//! \author     Stefan Habel <stefan.habel@filmakademie.de>
//! \author     Nils Zweiling <nils.zweiling@filmakademie.de>
//! \version    1.0
//! \date       29.06.2009 (last updated)
//!

#ifndef VIEWPORTWIDGET_H
#define VIEWPORTWIDGET_H

#include "FrapperPrerequisites.h"
#include "InstanceCounterMacros.h"
#include "SceneModel.h"
#include "ViewingParameters.h"
#include <QtGui/QWidget>
#include <QtGui/QMouseEvent>
#include <QtGui/QMessageBox>
#include <QtGui/QRgb>
#include <QtGui/QColor>
#include <QtCore/QString>
#include <Ogre.h>
#if (OGRE_PLATFORM  == OGRE_PLATFORM_WIN32)
#include <windows.h>
#endif
#include <OgrePanelOverlayElement.h>
#include <OgreTextAreaOverlayElement.h>


//!
//! Macro definition for a visibility flag for objects that should be visible
//! in the main viewport.
//!
#define VF_MAIN_VIEWPORT 0x1

//!
//! Macro definition for a visibility flag for objects that should be visible
//! in the orientation indicator viewport only.
//!
#define VF_ORIENTATION_VIEWPORT 0x2


//!
//! Class for a 3D viewport widget powered by OGRE.
//!
//! <div align="center"><b>Inheritance Diagram</b></div>
//!
//! \dot
//!   digraph ViewportWidget {
//!     node [fontname="FreeSans",fontsize="10",shape="box",height=0.2,width=0.4,fontname="FreeSans",color="black",style="filled",fillcolor="white"];
//!     edge [fontname="FreeSans",fontsize="10",labelfontname="FreeSans",labelfontsize="10",dir=back,style="solid",color="midnightblue"];
//!
//!     ViewportWidget [label="ViewportWidget",fillcolor="grey75"];
//!     QWidget -> ViewportWidget;
//!     QWidget [label="QWidget",fontcolor="white",fillcolor="#66b036",URL="http://doc.qtsoftware.com/4.5/qwidget.html"];
//!   }
//! \enddot
//! <div><center>[<a href="graph_legend.html">legend</a>]</center></div>
//!
class FRAPPER_GUI_EXPORT ViewportWidget : public QWidget
{

    Q_OBJECT
    ADD_INSTANCE_COUNTER

private: // static data
    
    //!
    //! The index of the next viewport widget to create.
    //!
    static unsigned int s_viewportIndex;

    //!
    //! Material for self-illuminating red objects (like coordinate X axes).
    //!
    static Ogre::MaterialPtr s_redMaterial;

    //!
    //! Material for self-illuminating green objects (like coordinate Y axes).
    //!
    static Ogre::MaterialPtr s_greenMaterial;

    //!
    //! Material for self-illuminating blue objects (like coordinate Z axes).
    //!
    static Ogre::MaterialPtr s_blueMaterial;

    //!
    //! Material for self-illuminating gray objects.
    //!
    static Ogre::MaterialPtr s_grayMaterial;

    //!
    //! Material for self-illuminating black objects.
    //!
    static Ogre::MaterialPtr s_blackMaterial;

    //!
    //! Default material to use in shaded mode.
    //!
    static Ogre::MaterialPtr s_defaultMaterial;

    //!
    //! Material for Null objects.
    //!
    static Ogre::MaterialPtr s_nullMaterial;

    //!
    //! Material for semi-transparent mask overlays.
    //!
    static Ogre::MaterialPtr s_maskMaterial;

    ////!
    ////! Material for the viewport's image overlay that is used to display image
    ////! results from nodes.
    ////!
    //static Ogre::MaterialPtr s_imageMaterial;

public: // constructors and destructors

    //!
    //! Constructor of the ViewportWidget class.
    //!
    //! \param parent The parent widget the created instance will be a child of.
    //! \param stageIndex The index of the stage to display in the viewport.
    //! \param fullscreen Flag that controls whether the widget is used in fullscreen mode.
    //! \param stereo Flag that controls whether the widget is used in stereo mode.
    //! \param width The width of the viewport when in fullscreen mode.
    //! \param height The height of the viewport when in fullscreen mode.
    //!
    ViewportWidget ( QWidget *parent = 0, unsigned int stageIndex = 1, bool fullscreen = false, bool stereo = false, int width = 1024, int height = 768 );

    //!
    //! Destructor of the ViewportWidget class.
    //!
    //! Defined virtual to guarantee that the destructor of a derived class
    //! will be called if the instance of the derived class is saved in a
    //! variable of its parent class type.
    //!
    virtual ~ViewportWidget();

public: // functions

    //!
    //! Returns a pointer to the paint engine used for drawing on the device.
    //!
    //! Returning 0 by this function fixes the widget flickering bug in Qt 4.5.
    //!
    //! \return 0.
    //!
    virtual QPaintEngine * paintEngine () const;

    //!
    //! Returns the index of the stage to display in the viewport.
    //!
    //! \return The index of the stage to display in the viewport.
    //!
    unsigned int getStageIndex ();

    //!
    //! Sets the index of the stage to display in the viewport.
    //!
    //! \param stageIndex The index of the new stage to display in the viewport.
    //!
    void setStageIndex ( unsigned int stageIndex );

    //!
    //! Activates or deactivates the view mode in the viewport widget.
    //!
    //! \param viewMode The new value for the view mode flag.
    //!
    void setViewMode ( bool viewMode );

    //!
    //! Centers the viewport's view on currently selected objects.
    //!
    void frameSelectedObjects ();

	//!
    //! Sets the fog parameters of the viewport.
    //!
    //! \param enabled Sets whether fog is enabled or not.
    //! \param density Sets the density of the fog (exponential).
    //!
    void setFog ( bool enabled, double density );

    //!
    //! Returns the fog color of the scene.
    //!
    //! \return Fog color of the scene.
    //!
    QColor getFogColor ();

    //!
    //! Sets the fog color of the scene.
    //!
    //! \param color Fog color of the scene.
    //!
    void setFogColor ( const QColor &color );

    //!
    //! Returns the background color of the scene.
    //!
    //! \return Fog background of the scene.
    //!
    QColor getBackgroundColor ();

    //!
    //! Sets the background color of the scene.
    //!
    //! \param color Background color of the scene.
    //!
    void setBackgroundColor ( const QColor &color );

	//!
    //! Sets fullscreen mode for the viewport widget.
    //!
    //! \param fullscreen Flag to control whether to enable fullscreen mode.
    //! \param width The width of the viewport in fullscreen mode.
    //! \param height The height of the viewport in fullscreen mode.
    //!
    void setFullscreen ( bool fullscreen, int width, int height );

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
    void getStereoParameters( double &rttWidth, double &rttHeight,
                              double &eyeDist, double &fOffsetMultiplier,
                              double &nearClip, double &farClip, double &fov, double &focalLength );

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
    void setStereoParameters( const unsigned int rttWidth, const unsigned int rttHeight,
                              const float eyeDist, const float fOffsetMultiplier,
                              const float nearClip, const float farClip, const float fov, const float focalLength );

    //!
    //! Toggles the stereo mode for the viewport widget.
    //!
    //! \param enable Flag to enable or disable the stereo mode for the viewport widget.
    //!
    void setStereo ( bool enable );

    //!
    //! Sets whether only overlays (no geometry) should be displayed in viewport.
    //!
    //! \param enable If true no geometry will be displayed.
    //!
    void setOverlayOnly ( bool enable );

    //!
    //! Returns the current number of frames per second rendered in the
    //! viewport.
    //!
    //! \return The current number of frames per second rendered in the viewport.
    //!
    float getFps ();

    //!
    //! Returns the number of triangles last rendered in the viewport.
    //!
    //! \return The number of triangles last rendered in the viewport.
    //!
    int getTriangleCount ();

    //!
    //! Returns the viewing parameter's of the currently active camera.
    //!
    //! \return The viewing parameter's of the currently active camera.
    //!
    ViewingParameters * getViewingParameters ();

    //!
    //! Calculates the transformation of the viewport's camera according to the
    //! current viewing angles, the viewing radius, and the center of view.
    //!
    void updateView ();

public slots: //

    //!
    //! Applies the given viewing parameters to the viewport's viewing
    //! parameters.
    //!
    //! \param viewingParameters The viewing parameters to apply to the viewport.
    //!
    void applyViewingParameters ( ViewingParameters *viewingParameters );

    //!
    //! Sets the image that should be displayed in the viewport.
    //!
    //! \param stageIndex The index of the stage to which the image belongs.
    //! \param image The image that should be displayed in the viewport.
    //!
    void setImage ( unsigned int stageIndex, Ogre::TexturePtr image );

    //!
    //! Resets the viewport's view settings.
    //!
    void homeView ();

    //!
    //! Activates or deactivates wireframe view in the viewport.
    //!
    //! \param wireframe Flag that controls wireframe view in the viewport.
    //!
    void setWireframe ( bool wireframe );

    //!
    //! Activates or deactivates the omission of polygons facing away from the
    //! camera.
    //!
    //! \param backfaceCulling Flag that controls the culling of polygons facing away from the camera.
    //!
    void setBackfaceCulling ( bool backfaceCulling );

    //!
    //! Toggles the display of the point of origin in the viewport.
    //!
    void toggleOrigin ();

    //!
    //! Toggles the display of the center point in the viewport.
    //!
    void toggleCenterPoint ();

    //!
    //! Toggles the display of the orientation indicator in the viewport.
    //!
    void toggleOrientationIndicator ();

    //!
    //! Toggles the display of the coordinate grid in the viewport.
    //!
    void toggleGrid ();

signals: //

    //!
    //! Signal that is emitted when an object in the viewport has been clicked.
    //!
    //! \param objectName The name of the object that was selected, or an empty string if no object has been selected.
    //! \param toggle Flag that controls whether the object's selected state should be toggled.
    //!
    void objectSelected ( const QString &objectName, bool toggle );

    //!
    //! Signal that is emitted when the viewing parameters have changed.
    //!
    //! \param viewingParameters The current viewing parameters.
    //!
    void viewChanged ( ViewingParameters *viewingParameters );

    //!
    //! Signal that is emitted when the selected objects in the viewport should
    //! be framed by the viewport's camera.
    //!
    //! Requests an adjustment of the viewing parameters the viewport's camera
    //! so that the currently selected objects are centered in the view.
    //!
    //! \param viewingParameters The viewing parameters to modify.
    //!
    void frameSelectedObjectsRequested ( ViewingParameters *viewingParameters );
    
    //!
    //! Rendering process has been initiated.
    //!
    void renderingTriggered();

protected: // events

    //!
    //! Handles resize events for the widget.
    //!
    //! \param event The description of the resize event.
    //!
    virtual void resizeEvent ( QResizeEvent *event );

    //!
    //! Handles paint events for the widget.
    //!
    //! \param event The description of the paint event.
    //!
    virtual void paintEvent ( QPaintEvent *event );

    //!
    //! Handles mouse press events for the widget.
    //!
    //! \param event The description of the mouse event.
    //!
    virtual void mousePressEvent ( QMouseEvent *event );

    //!
    //! Handles mouse move events for the widget.
    //!
    //! \param event The description of the mouse event.
    //!
    virtual void mouseMoveEvent ( QMouseEvent *event );

    //!
    //! Handles mouse release events for the widget.
    //!
    //! \param event The description of the mouse event.
    //!
    virtual void mouseReleaseEvent ( QMouseEvent *event );

    //!
    //! Handles mouse wheel events for the widget.
    //!
    //! \param event The description of the mouse wheel event.
    //!
    virtual void wheelEvent ( QWheelEvent *event );

private: // functions

    //!
    //! Creates a unique name from the given object name by adding the viewport
    //! widget's index.
    //!
    Ogre::String createUniqueName ( const QString &objectName );

    //!
    //! Creates the OGRE render window.
    //!
    //! \return True if creating the OGRE render window succeeded, otherwise False.
    //!
    bool createRenderWindow ();

    //!
    //! Initializes the resources that will be available to OGRE.
    //!
    void initializeResources ();

    //!
    //! Creates basic viewport objects like the viewing camera, the head light,
    //! coordinate axes and the grid.
    //!
    void createViewportObjects ();

    //!
    //! Creates overlay layers for visual components that will be rendered on
    //! top of the main scene layer.
    //!
    void createOverlays ();

    //!
    //! Redraws the OGRE scene into the viewport widget.
    //!
    void render ();

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
    Ogre::MaterialPtr createColorMaterial ( const Ogre::String &group, const Ogre::String &name, const Ogre::ColourValue &color, bool wireframe = false );

    //!
    //! Creates three manual objects for X, Y and Z axes with the given name and
    //! attaches them to the given parent scene node.
    //! The axes will each have a length of 1.
    //!
    //! \param name The prefix for the axes objects to create.
    //! \param parentNode The scene node to which to attach the axes objects.
    //! \param visibilityFlags The visibility flags to apply to the axes objects.
    //!
    void createAxes ( const QString &name, Ogre::SceneNode *parentNode, Ogre::uint32 visibilityFlags = VF_MAIN_VIEWPORT );

    //!
    //! Creates a coordinate grid and attaches it to the given parent scene node.
    //!
    //! \param parentNode The scene node to which to attach the grid object.
    //!
    void createGrid ( Ogre::SceneNode *parentNode );

    //!
    //! Returns the scene node that is located under the given mouse position in
    //! the viewport.
    //!
    //! \param mousePosition The position of the mouse pointer over the viewport widget.
    //! \return The clicked scene node or 0 if no scene node was clicked.
    //!
    Ogre::SceneNode * getClickedSceneNode ( const QPointF &mousePosition );

protected: // data

    //!
    //! The index of the stage to display in the viewport.
    //!
    unsigned int m_stageIndex;

    //!
    //! Show overlays.
    //!
    bool m_showOverlays;

    //!
    //! Flag that states whether the widget is used in fullscreen mode.
    //!
    bool m_fullscreen;

    //!
    //! Flag that states whether stereo viewing is activated.
    //!
    bool m_stereo;

    //!
    //! Stereo view parameters.
    //!
    float m_stereoEyeDist;
    float m_stereoFOffsetMultiplier;
    float m_stereoNearClip;
    float m_stereoFarClip;
    float m_stereoFov;
    float m_stereoFocalLength;
    unsigned int m_stereoRttWidth;
    unsigned int m_stereoRttHeight;

    //!
    //! The width of the viewport when in fullscreen mode.
    //!
    int m_width;

    //!
    //! The height of the viewport when in fullscreen mode.
    //!
    int m_height;

    // OGRE System Objects

    //!
    //! The OGRE render window to use in the viewport widget.
    //!
    Ogre::RenderWindow *m_renderWindow;

    //!
    //! The main OGRE scene manager.
    //!
    Ogre::SceneManager *m_sceneManager;

    //!
    //! The main viewport's viewing camera scene node.
    //!
    Ogre::SceneNode *m_cameraSceneNode;
    
    //!
    //! Material for the viewport's image overlay that is used to display image
    //! results from nodes.
    //!
    Ogre::MaterialPtr m_imageMaterial;

    //!
    //! The viewport's viewing cameras.
    //!
    Ogre::Camera *m_camera;
    Ogre::Camera *m_camera0;
    Ogre::Camera *m_camera1;
    Ogre::Camera *m_camera2;
    Ogre::Camera *m_camera3;
    Ogre::Camera *m_camera4;

    //!
    //! The current camera name.
    //!
    QString m_cameraName;

    //!
    //! The viewing camera's light.
    //!
    Ogre::Light *m_headLight;

    //!
    //! The stereo compositor.
    //!
    Ogre::CompositorInstance *m_stereoCompositor;

    //!
    //! The stereo OGRE texture render target.
    //!
    Ogre::RenderTarget *m_stereoRenderTarget;

    //!
    //! The OGRE viewports.
    //!
    Ogre::Viewport *m_viewport;
    Ogre::Viewport *m_stereoViewport0;
    Ogre::Viewport *m_stereoViewport1;
    Ogre::Viewport *m_stereoViewport2;
    Ogre::Viewport *m_stereoViewport3;
    Ogre::Viewport *m_stereoViewport4;

    //!
    //! The background color of the viewport.
    //!
	Ogre::ColourValue m_backgroundColor;

    //!
    //! The fog color of the scene.
    //!
	Ogre::ColourValue m_fogColor;

    //!
    //! The orientation indicator viewport's camera scene node.
    //!
    Ogre::SceneNode *m_orientationIndicatorCameraSceneNode;

    //!
    //! The orientation indicator viewport's camera.
    //!
    Ogre::Camera *m_orientationIndicatorCamera;

    //!
    //! The orientation indicator viewport to display axes that show the main
    //! camera's orientation.
    //!
    Ogre::Viewport *m_orientationIndicatorViewport;

    // Overlays

    //!
    //! The overlay for all text information panel overlay elements.
    //!
    Ogre::Overlay *m_labelOverlay;

    //!
    //! The overlay for the image panel overlay element.
    //!
    Ogre::Overlay *m_imageOverlay;

    //!
    //! The overlay for all mask panel overlay elements.
    //!
    Ogre::Overlay *m_maskOverlay;

    //!
    //! The panel overlay element used for displaying text information in the
    //! top left corner of the viewport.
    //!
    Ogre::PanelOverlayElement *m_leftTopHudPanel;

    //!
    //! The text area overlay element used for displaying text information in
    //! the top left corner of the viewport.
    //!
    Ogre::TextAreaOverlayElement *m_leftTopHudTextArea;

    //!
    //! The panel overlay element used for displaying an image.
    //!
    Ogre::PanelOverlayElement *m_imagePanel;

    //!
    //! The panel overlay element used for masking the top of the viewport.
    //!
    Ogre::PanelOverlayElement *m_topMaskPanel;

    //!
    //! The panel overlay element used for masking the bottom of the viewport.
    //!
    Ogre::PanelOverlayElement *m_bottomMaskPanel;

    //!
    //! The panel overlay element used for masking the left of the viewport.
    //!
    Ogre::PanelOverlayElement *m_leftMaskPanel;

    //!
    //! The panel overlay element used for masking the right of the viewport.
    //!
    Ogre::PanelOverlayElement *m_rightMaskPanel;

    // Scene Nodes

    //!
    //! The node that all exclusive viewport object scene nodes will be
    //! attached to.
    //!
    Ogre::SceneNode *m_viewportObjectsSceneNode;

    //!
    //! The node that the coordinate system's origin axes will be attached to.
    //!
    Ogre::SceneNode *m_originSceneNode;

    //!
    //! The node that the center axes will be attached to.
    //!
    Ogre::SceneNode *m_centerPointSceneNode;

    //!
    //! The node that the orientation indicator will be attached to.
    //!
    Ogre::SceneNode *m_orientationIndicatorSceneNode;

    //!
    //! The node that the coordinate grid will be attached to.
    //!
    Ogre::SceneNode *m_gridSceneNode;

    // Viewing

    //!
    //! Flag that states whether the viewing mode is activated.
    //!
    bool m_viewMode;

    //!
    //! The last position of the mouse pointer over the viewport.
    //!
    QPoint m_lastPosition;

    //!
    //! The viewing parameters for the viewport's active camera.
    //!
    ViewingParameters *m_viewingParameters;

    // Display Flags

    //!
    //! Flag that states whether wireframe view is activated.
    //!
    bool m_wireframe;

    //!
    //! Flag that states whether backface culling is activated.
    //!
    bool m_backfaceCulling;

    //!
    //! Flag that states whether the origin should be visible in the viewport.
    //!
    bool m_showOrigin;

    //!
    //! Flag that states whether the center should be visible in the viewport.
    //!
    bool m_showCenterPoint;

    //!
    //! Flag that states whether the orientation indicator is shown in the
    //! widget.
    //!
    bool m_showOrientationIndicator;

    //!
    //! Flag that states whether the grid should be visible in the viewport.
    //!
    bool m_showGrid;

    //!
    //! Flag that states whether the geometry should be visible.
    //!
    bool m_overlayOnly;

    /// DEBUG: NILZ:
    Ogre::TexturePtr m_tmpTexture;
    float m_dbgColor;

};

#endif
