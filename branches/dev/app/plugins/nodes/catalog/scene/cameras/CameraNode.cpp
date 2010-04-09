/*
-----------------------------------------------------------------------------
This source file is part of FRAPPER
research.animationsinstitut.de
sourceforge.net/projects/frapper

Copyright (c) 2008-2009 Filmakademie Baden-Wuerttemberg, Institute of Animation 

This program is free software; you can redistribute it and/or modify it under
the terms of the GNU Lesser General Public License as published by the Free Software
Foundation; either version 2 of the License, or (at your option) any later
version.

This program is distributed in the hope that it will be useful, but WITHOUT
ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
FOR A PARTICULAR PURPOSE. See the GNU Lesser General Public License for more details.

You should have received a copy of the GNU Lesser General Public License along with
this program; if not, write to the Free Software Foundation, Inc., 59 Temple
Place - Suite 330, Boston, MA 02111-1307, USA, or go to
http://www.gnu.org/copyleft/lesser.txt.
-----------------------------------------------------------------------------
*/

//!
//! \file "CameraNode.cpp"
//! \brief Implementation file for CameraNode class.
//!
//! \author     Stefan Habel <stefan.habel@filmakademie.de>
//! \author     Nils Zweiling <nils.zweiling@filmakademie.de>
//! \author     Felix Bucella <felix.bucella@filmakademie.de>
//! \version    1.0
//! \date       29.06.2009 (last updated)
//!

#include "CameraNode.h"
#include "SceneNodeParameter.h"
#include "EnumerationParameter.h"
#include "NumberParameter.h"
#include "OgreTools.h"
#include "OgreManager.h"
#include "Log.h"

INIT_INSTANCE_COUNTER(CameraNode)


///
/// Static Constants
///

//!
//! Matrix to transform clip-space into image-space.
//!
static const Ogre::Matrix4 s_CLIP_SPACE_TO_IMAGE_SPACE(
0.5,    0,    0,  0.5,
0,   -0.5,    0,  0.5,
0,      0,    1,    0,
0,      0,    0,    1);

///
/// Constructors and Destructors
///


//!
//! Constructor of the CameraNode class.
//!
//! \param name The name for the new node.
//! \param parameterRoot A copy of the parameter tree specific for the type of the node.
//!
CameraNode::CameraNode ( const QString &name, ParameterGroup *parameterRoot ) :
    ViewNode(name, parameterRoot),
    m_sceneManager(OgreManager::getSceneManager()),
    m_sceneNode(0),
    m_ogreContainer(0),
    m_camera(0),
    m_entity(0),
    m_outputCameraName("Camera"),
    m_presetChanging(false)
{
    // calculate the render resolution's aspect ratio based on the width and height set in the camera.xml file
    unsigned int renderWidth = getUnsignedIntValue("Render Resolution > Render Width");
    unsigned int renderHeight = getUnsignedIntValue("Render Resolution > Render Height");
    if (renderHeight == 0) {
        Log::error("Render height is 0.", "CameraNode::CameraNode");
        return;
    }
    float aspectRatio = (float) renderWidth / renderHeight;
    setValue("Render Resolution > Aspect Ratio", QString("%1").arg(aspectRatio), true);

    // create an output camera parameter
    CameraParameter *outputCameraParameter = new CameraParameter(m_outputCameraName);
    outputCameraParameter->setPinType(Parameter::PT_Output);
    outputCameraParameter->setNode(this);
    outputCameraParameter->addAffectingParameter(getParameter("Render Resolution > Preset"));
    outputCameraParameter->addAffectingParameter(getParameter("Render Width"));
    outputCameraParameter->addAffectingParameter(getParameter("Render Height"));
    outputCameraParameter->addAffectingParameter(getParameter("Focal Length"));
    outputCameraParameter->addAffectingParameter(getParameter("Field of View"));
    outputCameraParameter->addAffectingParameter(getParameter("Horizontal Aperture"));
    outputCameraParameter->addAffectingParameter(getParameter("Near Clipping Plane"));
    outputCameraParameter->addAffectingParameter(getParameter("Far Clipping Plane"));
	outputCameraParameter->addAffectingParameter(getParameter("Image Space Projection Matix(4x4)"));
	outputCameraParameter->addAffectingParameter(getParameter("World Matix(4x4)"));
	outputCameraParameter->addAffectingParameter(getParameter("Far Corner"));
    parameterRoot->addParameter(outputCameraParameter);

    // create OGRE scene node and scene node encapsulation
    m_sceneNode = OgreManager::createSceneNode(m_name);
    if (m_sceneNode) {
        m_ogreContainer = new OgreContainer(m_sceneNode);
        m_sceneNode->setUserAny(Ogre::Any(m_ogreContainer));
        m_sceneNode->setScale(0.3, 0.3, -0.4);
    }
    setValue(m_outputCameraName, m_sceneNode, true);

    // create an OGRE camera object
    QString cameraName = QString("%1Camera").arg(m_name);
    m_camera = m_sceneManager->createCamera(cameraName.toStdString());

    // set additional camera info
    CameraInfo *cameraInfo = new CameraInfo();
    cameraInfo->width = renderWidth;
    cameraInfo->height = renderHeight;
    //m_camera->Ogre::MovableObject::setUserAny(Ogre::Any(cameraInfo));
    Ogre::MovableObject *movableObject = dynamic_cast<Ogre::MovableObject *>(m_camera);
    movableObject->setUserAny(Ogre::Any(cameraInfo));

    // create a new OGRE entity to represent the camera in the viewport
    QString entityName = QString("%1Entity").arg(m_name);
    m_entity = m_sceneManager->createEntity(entityName.toStdString(), "camera.mesh");

    // attach the camera and the entity to the scene node
    if (m_sceneNode) {
        if (m_camera)
            m_sceneNode->attachObject(m_camera);
        if (m_entity)
            m_sceneNode->attachObject(m_entity);
    }

    // set up parameter callback functions
    setChangeFunction("Position", SLOT(applyPosition()));
    setChangeFunction("Orientation", SLOT(applyOrientation()));
    setChangeFunction("Render Resolution > Preset", SLOT(renderResolutionPresetChanged()));
    setChangeFunction("Render Resolution > Render Width", SLOT(renderWidthChanged()));
    setChangeFunction("Render Resolution > Render Height", SLOT(renderHeightChanged()));
    setChangeFunction("Focal Length", SLOT(focalLengthChanged()));
    setChangeFunction("Field of View", SLOT(fieldOfViewChanged()));
    setChangeFunction("Horizontal Aperture", SLOT(horizontalApertureChanged()));
    setChangeFunction("Near Clipping Plane", SLOT(nearClippingPlaneChanged()));
    setChangeFunction("Far Clipping Plane", SLOT(farClippingPlaneChanged()));
    connect(this, SIGNAL(frameChanged(int)), SLOT(updateTransform()));

    // initialize the camera by calling the callback functions
    applyPosition();
    applyOrientation();
    renderWidthChanged();
    renderHeightChanged();
    focalLengthChanged();
    fieldOfViewChanged();
    horizontalApertureChanged();
    nearClippingPlaneChanged();
    farClippingPlaneChanged();

    INC_INSTANCE_COUNTER
}


//!
//! Destructor of the CameraNode class.
//!
//! Defined virtual to guarantee that the destructor of a derived class
//! will be called if the instance of the derived class is saved in a
//! variable of its parent class type.
//!
CameraNode::~CameraNode ()
{
    if (m_entity) {
        m_sceneManager->destroyEntity(m_entity);
        m_entity = 0;
    }
    if (m_camera) {
        m_sceneManager->destroyCamera(m_camera);
        m_camera = 0;
    }
    if (m_sceneNode) {
        m_sceneManager->destroySceneNode(m_sceneNode);
        m_sceneNode = 0;
    }

    setValue(m_outputCameraName, (Ogre::SceneNode *) 0, true);

    DEC_INSTANCE_COUNTER
}


///
/// Public Functions
///


//!
//! Returns the scene node that contains scene objects created or modified
//! by this node.
//!
//! \return The scene node containing objects created or modified by this node.
//!
Ogre::SceneNode * CameraNode::getSceneNode ()
{
    Ogre::SceneNode *result = getSceneNodeValue(m_outputCameraName, true);
    if (!result)
        Log::error("Could not obtain camera scene node.", "CameraNode::getSceneNode");
    return result;
}


///
/// Private Functions
///


//!
//! Checks if the currently set render width and height corresponds to a
//! render resolution preset, and if so, selects that preset.
//!
void CameraNode::checkForPreset ()
{
    EnumerationParameter *renderResolutionPresetParameter = getEnumerationParameter("Render Resolution > Preset");
    NumberParameter *renderWidthParameter = getNumberParameter("Render Resolution > Render Width");
    NumberParameter *renderHeightParameter = getNumberParameter("Render Resolution > Render Height");
    if (!renderResolutionPresetParameter || !renderWidthParameter || !renderHeightParameter) {
        Log::error("Render resoulution preset, width and/or render height parameter could not be obtained.", "CameraNode::checkForPreset");
        return;
    }

    unsigned int width = renderWidthParameter->getValue().toUInt();
    unsigned int height = renderHeightParameter->getValue().toUInt();

    QStringList values = renderResolutionPresetParameter->getValues();
    int index = 0;
    foreach (QString value, values) {
        QStringList parts = value.split(" x ");
        if (parts.size() == 2) {
            unsigned int presetWidth = parts[0].toUInt();
            unsigned int presetHeight = parts[1].toUInt();

            if (presetWidth == width && presetHeight == height) {
                renderResolutionPresetParameter->setValue(index);
                return;
            }
        }
        ++index;
    }

    // select the custom render resolution item
    QStringList literals = renderResolutionPresetParameter->getLiterals();
    int customIndex = literals.indexOf("Custom");
    renderResolutionPresetParameter->setValue(customIndex);
}


///
/// Private Slots
///

//!
//! Applies the currently set transformation for the node to the OGRE scene.
//! on frame change.
//!
void CameraNode::updateTransform ()
{
    getVectorValue("Position", true);
    getVectorValue("Orientation", true);
    applyPosition ();
    applyOrientation ();
}

//!
//! Applies the currently set position for the node to the OGRE scene
//! objects contained in this node.
//!
void CameraNode::applyPosition ()
{
    Ogre::Vector3 position = getVectorValue("Position");

    // apply the position to the camera's scene node and all its copies
    m_sceneNode->setPosition(position);
	if (getBoolValue("lookAtEnabled")) {
		//Ogre::Vector3 lookAtTarget = getVectorValue("lookAtTarget");
		Ogre::Vector3 lookAtTarget(0.0, 0.0, 0.0);
		Ogre::Vector3 sceneNodePosition = m_sceneNode->getPosition();
		Ogre::Vector3 lookAtVector = lookAtTarget - sceneNodePosition;
		//m_sceneNode->lookAt(lookAtTarget, Ogre::Node::TS_WORLD);
		m_sceneNode->setDirection(lookAtVector);
	}
	m_ogreContainer->updateCopies();
	
    // TODO: NILZ: Need for a global update. Not here. Perhaps slider need a redraw connection.
    //triggerRedraw();
}


//!
//! Applies the currently set orientation for the node to the OGRE scene
//! objects contained in this node.
//!
void CameraNode::applyOrientation ()
{
	if (!getBoolValue("lookAtEnabled")) {
		Ogre::Vector3 orientation = getVectorValue("Orientation");

		Ogre::Radian xRadian;
		Ogre::Radian yRadian;
		Ogre::Radian zRadian;
		bool useRadians = getBoolValue("useRadians");
		if (useRadians) {
			// decode the parameter's values
			xRadian = Ogre::Radian(orientation.x);
			yRadian = Ogre::Radian(orientation.y);
			zRadian = Ogre::Radian(orientation.z);
		}
		else {
			xRadian = Ogre::Radian(Ogre::Degree(orientation.x));
			yRadian = Ogre::Radian(Ogre::Degree(orientation.y));
			zRadian = Ogre::Radian(Ogre::Degree(orientation.z));
		}

		// apply the orientation to the camera's scene node and all its copies
		m_sceneNode->resetOrientation();
		m_sceneNode->pitch(xRadian, Ogre::Node::TS_WORLD);
		m_sceneNode->yaw(yRadian, Ogre::Node::TS_WORLD);
		m_sceneNode->roll(zRadian, Ogre::Node::TS_WORLD);
		m_ogreContainer->updateCopies();

		triggerRedraw();
	}
}


//!
//! Updates the render resolution parameters according to the selected
//! preset.
//!
//! Is called when the value of the render resolution preset parameter has
//! changed.
//!
void CameraNode::renderResolutionPresetChanged ()
{
    // obtain the render resolution preset parameter
    EnumerationParameter *renderResolutionPresetParameter = getEnumerationParameter("Render Resolution > Preset");
    if (!renderResolutionPresetParameter) {
        Log::error("Render resolution preset parameter could not be obtained.", "CameraNode::renderResolutionPresetChanged");
        return;
    }

    // get the value of the currently selected item in the enumeration
    QString value = renderResolutionPresetParameter->getCurrentValue();
    if (value.isNull())
        //> no value is available for the selected render resolution preset
        return;

    // split the enumeration parameter's value into parts
    QStringList values = value.split(" x ");
    if (values.size() == 2) {
        // decode the enumeration parameter's value into width and height and calculate the aspect ratio
        unsigned int width = values[0].toUInt();
        unsigned int height = values[1].toUInt();
        float aspectRatio = float(width) / height;

        // apply width, height and aspect ratio to the respective parameters
        m_presetChanging = true;
        setValue("Render Resolution > Render Width", width);
        setValue("Render Resolution > Render Height", height);
        setValue("Render Resolution > Aspect Ratio", QString("%1").arg(aspectRatio));
        m_presetChanging = false;
    } else
        Log::debug("The render resolution preset's value does not contain a width and height.", "CameraNode::renderResolutionPresetChanged");
}


//!
//! Applies the changed render width to the output camera.
//!
//! Is called when the value of the render width parameter has changed.
//!
void CameraNode::renderWidthChanged ()
{
    unsigned int width = getUnsignedIntValue("Render Resolution > Render Width");

    // apply the changed render width to the output camera
    CameraInfo *cameraInfo = Ogre::any_cast<CameraInfo *>(((Ogre::MovableObject *) m_camera)->getUserAny());
    if (cameraInfo) {
        cameraInfo->width = width;
        m_camera->setAspectRatio(float(cameraInfo->width) / cameraInfo->height);
    }

    // skip the rest of the function if the render width has changed due to a change of render resolution preset
    if (m_presetChanging)
        return;

    NumberParameter *renderHeightParameter = getNumberParameter("Render Resolution > Render Height");
    if (!renderHeightParameter)
        return;
    unsigned int height = renderHeightParameter->getValue().toDouble();

    // check if the aspect ratio is to be kept
    bool keepAspectRatio = getBoolValue("Keep Aspect Ratio");
    if (keepAspectRatio) {
        // calculate the height based on the current aspect ratio
        double aspectRatio = getValue("Aspect Ratio").toDouble();   // Aspect Ratio is a String parameter, so don't use getDoubleValue() here
        if (aspectRatio == 0) {
            Log::error("Aspect ratio is 0.", "CameraNode::renderWidthChanged");
            return;
        }
        height = (float) width / aspectRatio + 0.5;
        renderHeightParameter->setValue(height);
    } else {
        // calculate the aspect ratio based on the render
        double aspectRatio = (float) width / height;
        setValue("Render Resolution > Aspect Ratio", QString("%1").arg(aspectRatio));
    }

    checkForPreset();
}


//!
//! Applies the changed render height to the output camera.
//!
//! Is called when the value of the render height parameter has changed.
//!
void CameraNode::renderHeightChanged ()
{
    unsigned int height = getUnsignedIntValue("Render Resolution > Render Height");

    // apply the changed render height to the output camera
    CameraInfo *cameraInfo = Ogre::any_cast<CameraInfo *>(((Ogre::MovableObject *) m_camera)->getUserAny());
    if (cameraInfo) {
        cameraInfo->height = height;
        m_camera->setAspectRatio(float(cameraInfo->width) / cameraInfo->height);
    }

    // skip the rest of the function if the render width has changed due to a change of render resolution preset
    if (m_presetChanging)
        return;

    NumberParameter *renderWidthParameter = getNumberParameter("Render Resolution > Render Width");
    if (!renderWidthParameter)
        return;
    unsigned int width = renderWidthParameter->getValue().toUInt();

    // check if the aspect ratio is to be kept
    bool keepAspectRatio = getBoolValue("Keep Aspect Ratio");
    if (keepAspectRatio) {
        // calculate the width based on the current aspect ratio
        double aspectRatio = getValue("Aspect Ratio").toDouble();   // Aspect Ratio is a String parameter, so don't use getDoubleValue() here
        if (aspectRatio == 0) {
            Log::error("Aspect ratio is 0.", "CameraNode::renderHeightChanged");
            return;
        }
        width = (float) height * aspectRatio + 0.5;
        renderWidthParameter->setValue(width);
    } else {
        // calculate the aspect ratio based on the render
        double aspectRatio = (float) width / height;
        setValue("Render Resolution > Aspect Ratio", QString("%1").arg(aspectRatio));
    }

    checkForPreset();
}


//!
//! Applies the changed focal length to the output camera.
//!
//! Is called when the value of the focal length parameter has changed.
//!
void CameraNode::focalLengthChanged ()
{
    NumberParameter *focalLengthParameter = getNumberParameter("Focal Length");
	if (m_camera && focalLengthParameter) {
        m_camera->setFocalLength(focalLengthParameter->getValue().toDouble());
		updateDependentOutputParameters ();
	}
}


//!
//! Applies the changed field of view to the output camera.
//!
//! Is called when the value of the field of view parameter has changed.
//!
void CameraNode::fieldOfViewChanged ()
{
    NumberParameter *fieldOfViewParameter = getNumberParameter("Field of View");
	if (m_camera && fieldOfViewParameter) {
		m_camera->setFOVy(Ogre::Degree(fieldOfViewParameter->getValue().toDouble()));
		updateDependentOutputParameters ();
	}
}


//!
//! Applies the changed horizontal aperture to the output camera.
//!
//! Is called when the value of the horizontal aperture parameter has
//! changed.
//!
void CameraNode::horizontalApertureChanged ()
{
    NumberParameter *horizontalApertureParameter = getNumberParameter("Horizontal Aperture");
	if (m_camera && horizontalApertureParameter) {
        m_camera->setFOVy(Ogre::Radian(horizontalApertureParameter->getValue().toDouble()));
		updateDependentOutputParameters();
	}
}


//!
//! Applies the changed near clipping plane to the output camera.
//!
//! Is called when the value of the near clipping plane parameter has
//! changed.
//!
void CameraNode::nearClippingPlaneChanged ()
{
    NumberParameter *nearClippingPlaneParameter = getNumberParameter("Near Clipping Plane");
	if (m_camera && nearClippingPlaneParameter) {
		float nearClipParam = nearClippingPlaneParameter->getValue().toDouble();
		if (nearClipParam == 0)
			nearClipParam = 0.001;
        m_camera->setNearClipDistance(nearClipParam);
		updateDependentOutputParameters();
	}
}


//!
//! Applies the changed far clipping plane to the output camera.
//!
//! Is called when the value of the far clipping plane parameter has
//! changed.
//!
void CameraNode::farClippingPlaneChanged ()
{
    NumberParameter *farClippingPlaneParameter = getNumberParameter("Far Clipping Plane");
	if (m_camera && farClippingPlaneParameter) {
        m_camera->setFarClipDistance(farClippingPlaneParameter->getValue().toDouble());
		updateDependentOutputParameters();
	}
}


//!
//! Updates output parameters based on the frustum
//!
//! It should be called when a value of the frustum has changed
//!
void CameraNode::updateDependentOutputParameters ()
{
	if (m_camera) {
		Ogre::Matrix4 wMat;
		m_camera->getWorldTransforms(&wMat);
		// get projection matrix 
		Ogre::Matrix4 ptMat = s_CLIP_SPACE_TO_IMAGE_SPACE * m_camera->getProjectionMatrixWithRSDepth();

		// set matrix as otput
		m_ptMatList.clear();
		m_wMatList.clear();

		for(int i = 0; i < 4; ++i) {
			m_ptMatList.append(QVariant::fromValue(ptMat[i][0]));
			m_ptMatList.append(QVariant::fromValue(ptMat[i][1]));
			m_ptMatList.append(QVariant::fromValue(ptMat[i][2]));
			m_ptMatList.append(QVariant::fromValue(ptMat[i][3]));

			m_wMatList.append(QVariant::fromValue(wMat[i][0]));
			m_wMatList.append(QVariant::fromValue(wMat[i][1]));
			m_wMatList.append(QVariant::fromValue(wMat[i][2]));
			m_wMatList.append(QVariant::fromValue(wMat[i][3]));

			//// Test: Display matrix on console for testing
			//std::cout << "    " << ptMat[i][0];
			//std::cout << " \t " << ptMat[i][1];
			//std::cout << " \t " << ptMat[i][2];
			//std::cout << " \t " << ptMat[i][3] << std::endl;
		}
		setValue("Screen Space Projection Matix(4x4)", m_ptMatList, true);
		setValue("World Matix(4x4)", m_wMatList, true);

		// prepare output parameters		
		Ogre::Vector3 farCorner = m_camera->getViewMatrix(true) * m_camera->getWorldSpaceCorners()[4];
		setValue("Far Corner", farCorner, true);
	}
}