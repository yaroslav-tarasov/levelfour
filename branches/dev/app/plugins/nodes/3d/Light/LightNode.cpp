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
//! \file "LightNode.cpp"
//! \brief Implementation file for LightNode class.
//!
//! \author     Nils Zweiling <nils.zweiling@filmakademie.de>
//! \author     Stefan Habel <stefan.habel@filmakademie.de>
//! \version    1.0
//! \date       29.06.2009 (last updated)
//!

#include "LightNode.h"
#include "SceneNodeParameter.h"
#include "EnumerationParameter.h"
#include "NumberParameter.h"
#include "OgreTools.h"
#include "OgreManager.h"
#include "Log.h"

INIT_INSTANCE_COUNTER(LightNode)


///
/// Constructors and Destructors
///


//!
//! Constructor of the LightNode class.
//!
//! \param name The name for the new node.
//! \param parameterRoot A copy of the parameter tree specific for the type of the node.
//!
LightNode::LightNode ( const QString &name, ParameterGroup *parameterRoot ) :
    ViewNode(name, parameterRoot),
    m_sceneManager(OgreManager::getSceneManager()),
    m_sceneNode(0),
    m_ogreContainer(0),
    m_ogreLightContainer(0),
    m_light(0),
    m_entity(0),
    m_outputLightName("Light")
{
    // create an output light parameter
    LightParameter *outputLightParameter = new LightParameter(m_outputLightName);
    outputLightParameter->setPinType(Parameter::PT_Output);
    outputLightParameter->setNode(this);
    //outputLightParameter->addAffectingParameter(getParameter("Light Type"));
    //outputLightParameter->addAffectingParameter(getParameter("Diffuse Color"));
    //outputLightParameter->addAffectingParameter(getParameter("Specular Color"));
    //outputLightParameter->addAffectingParameter(getParameter("Casts Shadows"));
    //outputLightParameter->addAffectingParameter(getParameter("Inner Fall-Off Radius"));
    //outputLightParameter->addAffectingParameter(getParameter("Outer Fall-Off Radius"));
    parameterRoot->addParameter(outputLightParameter);

    // create OGRE scene node and scene node encapsulation
    m_sceneNode = OgreManager::createSceneNode(m_name);
    if (m_sceneNode) {
        m_ogreContainer = new OgreContainer(m_sceneNode);
        m_sceneNode->setUserAny(Ogre::Any(m_ogreContainer));
        //m_sceneNode->setScale(0.3, 0.3, -0.4);
    }

    setValue(m_outputLightName, m_sceneNode, true);

    // create an OGRE light object
    QString lightName = QString("%1Light").arg(m_name);
    m_light = m_sceneManager->createLight(lightName.toStdString());

    if (m_light) {
        m_ogreLightContainer = new OgreContainer(m_light);
        m_light->setUserAny(Ogre::Any(m_ogreLightContainer));
        //m_sceneNode->setScale(0.3, 0.3, -0.4);
    }

    // create a new OGRE entity to represent the light in the viewport
    QString entityName = QString("%1Entity").arg(m_name);
    m_entity = m_sceneManager->createEntity(entityName.toStdString(), "light.mesh");

    // attach the light and the entity to the scene node
    if (m_sceneNode) {
        if (m_light)
            m_sceneNode->attachObject(m_light);
        if (m_entity)
            m_sceneNode->attachObject(m_entity);
    }

    // set up parameter callback functions
    setChangeFunction("Position", SLOT(applyPosition()));
    setChangeFunction("Orientation", SLOT(applyOrientation()));
    setChangeFunction("Light Type", SLOT(applyParameters()));
    setChangeFunction("Diffuse Color", SLOT(applyParameters()));
    setChangeFunction("Specular Color", SLOT(applyParameters()));
    setChangeFunction("Casts Shadows", SLOT(applyParameters()));
    setChangeFunction("Inner Angle", SLOT(applyParameters()));
    setChangeFunction("Outer Angle", SLOT(applyParameters()));

    // initialize the light by calling the callback functions
    applyPosition();
    applyOrientation();


    INC_INSTANCE_COUNTER
}


//!
//! Destructor of the LightNode class.
//!
//! Defined virtual to guarantee that the destructor of a derived class
//! will be called if the instance of the derived class is saved in a
//! variable of its parent class type.
//!
LightNode::~LightNode ()
{
    if (m_entity) {
        m_sceneManager->destroyEntity(m_entity);
        m_entity = 0;
    }
    if (m_light) {
        m_sceneManager->destroyLight(m_light);
        m_light = 0;
    }
    if (m_sceneNode) {
        m_sceneManager->destroySceneNode(m_sceneNode);
        m_sceneNode = 0;
    }

    setValue(m_outputLightName, (Ogre::SceneNode *) 0, true);

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
Ogre::SceneNode * LightNode::getSceneNode ()
{
    Ogre::SceneNode *result = getSceneNodeValue(m_outputLightName, true);
    if (!result)
        Log::error("Could not obtain light scene node.", "LightNode::getSceneNode");
    return result;
}


///
/// Private Functions
///


///
/// Private Slots
///


//!
//! Applies the currently set position for the node to the OGRE scene
//! objects contained in this node.
//!
void LightNode::applyPosition ()
{
    Ogre::Vector3 position = getVectorValue("Position");

    // apply the position to the light's scene node and all its copies
    m_sceneNode->setPosition(position);
    m_ogreContainer->updateCopies();

    triggerRedraw();
}


//!
//! Applies the currently set orientation for the node to the OGRE scene
//! objects contained in this node.
//!
void LightNode::applyOrientation ()
{
    Ogre::Vector3 orientation = getVectorValue("Orientation");

    // decode the parameter's values
    Ogre::Radian xRadian = Ogre::Radian(Ogre::Degree(orientation.x));
    Ogre::Radian yRadian = Ogre::Radian(Ogre::Degree(orientation.y));
    Ogre::Radian zRadian = Ogre::Radian(Ogre::Degree(orientation.z));

    // apply the orientation to the light's scene node and all its copies
    m_sceneNode->resetOrientation();
    m_sceneNode->pitch(xRadian, Ogre::Node::TS_WORLD);
    m_sceneNode->yaw(yRadian, Ogre::Node::TS_WORLD);
    m_sceneNode->roll(zRadian, Ogre::Node::TS_WORLD);
    m_ogreContainer->updateCopies();

    triggerRedraw();
}

//!
//! Applies all additional parameters of the light node.
//!
//! Is called when any of the additional parameters of the light node has been
//! changed
//!
void LightNode::applyParameters ()
{
    if (!m_light)
        return;

    EnumerationParameter *lightTypeParameter = getEnumerationParameter("Light Type");
    if (lightTypeParameter) {
        Ogre::Light::LightTypes lightType = Ogre::Light::LT_POINT;
        QString lightTypeString = lightTypeParameter->getCurrentValue();
        if (lightTypeString == "Directional Light")
            lightType = Ogre::Light::LT_POINT;
        else if (lightTypeString == "Spot Light")
            lightType = Ogre::Light::LT_SPOTLIGHT;
        m_light->setType(lightType);
    }
    Parameter *diffuseColorParameter = getParameter("Diffuse Color");
    if (diffuseColorParameter) {
        QColor color = getColorValue("Diffuse Color");
        m_light->setDiffuseColour(color.red()/255, color.green()/255, color.blue()/255);
    }
    Parameter *specularColorParameter = getParameter("Specular Color");
    if (specularColorParameter) {
        QColor color = getColorValue("Specular Color");
        m_light->setSpecularColour(color.red()/255, color.green()/255, color.blue()/255);
    }
    Parameter *castsShadowsParameter = getParameter("Casts Shadows");
    if (castsShadowsParameter) {
        bool castsShadows = getBoolValue("Casts Shadows");
        m_light->setCastShadows(castsShadows);
        m_light->setVisible(castsShadows);
    }
    Parameter *innerAngleParameter = getParameter("Inner Angle");
    if (innerAngleParameter) {
        double innerAngle = getDoubleValue("Inner Angle");
        m_light->setSpotlightInnerAngle(Ogre::Radian(Ogre::Degree(innerAngle)));
    }
    Parameter *outerAngleParameter = getParameter("Outer Angle");
    if (outerAngleParameter) {
        double outerAngle = getDoubleValue("Outer Angle");
        m_light->setSpotlightOuterAngle(Ogre::Radian(Ogre::Degree(outerAngle)));
    }
    m_ogreLightContainer->updateCopies();
}
