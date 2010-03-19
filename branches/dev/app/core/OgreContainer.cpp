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
//! \file "OgreContainer.cpp"
//! \brief Implementation file for OgreContainer class.
//!
//! \author     Stefan Habel <stefan.habel@filmakademie.de>
//! \author     Nils Zweiling <nils.zweiling@filmakademie.de>
//! \version    1.0
//! \date       29.04.2009 (last updated)
//!

#include "OgreContainer.h"

INIT_INSTANCE_COUNTER(OgreContainer)


///
/// Constructors and Destructor
///


//!
//! Constructor of the OgreContainer class.
//!
//! \param sceneNode The scene node to be contained in the container.
//!
OgreContainer::OgreContainer ( Ogre::SceneNode *sceneNode ) :
    m_sceneNode(sceneNode),
    m_entity(0),
    m_light(0),
    m_camera(0)
{
    INC_INSTANCE_COUNTER
}


//!
//! Constructor of the OgreContainer class.
//!
//! \param entity The entity to be contained in the container.
//!
OgreContainer::OgreContainer ( Ogre::Entity *entity ) :
    m_sceneNode(0),
    m_entity(entity),
    m_light(0),
    m_camera(0)
{
    INC_INSTANCE_COUNTER
}


//!
//! Constructor of the OgreContainer class.
//!
//! \param light The light to be contained in the container.
//!
OgreContainer::OgreContainer ( Ogre::Light *light ) :
    m_sceneNode(0),
    m_entity(0),
    m_light(light),
    m_camera(0)
{
    INC_INSTANCE_COUNTER
}


//!
//! Constructor of the OgreContainer class.
//!
//! \param camera The camera to be contained in the container.
//!
OgreContainer::OgreContainer ( Ogre::Camera *camera ) :
    m_sceneNode(0),
    m_entity(0),
    m_light(0),
    m_camera(camera)
{
    INC_INSTANCE_COUNTER
}


//!
//! Destructor of the OgreContainer class.
//!
OgreContainer::~OgreContainer ()
{
    DEC_INSTANCE_COUNTER
}


///
/// Public Functions
///


//!
//! Returns the scene node contained in this container.
//!
//! \return The scene node contained in this container.
//!
Ogre::SceneNode * OgreContainer::getSceneNode ()
{
    return m_sceneNode;
}


//!
//! Sets the scene node to be contained in this container.
//!
//! \param sceneNode The scene node to be contained in this container.
//!
void OgreContainer::setSceneNode ( Ogre::SceneNode *sceneNode )
{
    m_sceneNode = sceneNode;
}


//!
//! Returns the entity contained in this container.
//!
//! \return The entity contained in this container.
//!
Ogre::Entity * OgreContainer::getEntity ()
{
    return m_entity;
}


//!
//! Sets the entity to be contained in this container.
//!
//! \param entity The entity to be contained in this container.
//!
void OgreContainer::setEntity ( Ogre::Entity *entity )
{
    m_entity = entity;
}


//!
//! Returns the light contained in this container.
//!
//! \return The light contained in this container.
//!
Ogre::Light * OgreContainer::getLight ()
{
    return m_light;
}


//!
//! Sets the light to be contained in this container.
//!
//! \param light The light to be contained in this container.
//!
void OgreContainer::setLight ( Ogre::Light *light )
{
    m_light = light;
}


//!
//! Returns the camera contained in this container.
//!
//! \return The camera contained in this container.
//!
Ogre::Camera * OgreContainer::getCamera ()
{
    return m_camera;
}


//!
//! Sets the camera to be contained in this container.
//!
//! \param camera The camera to be contained in this container.
//!
void OgreContainer::setCamera ( Ogre::Camera *camera )
{
    m_camera = camera;
}


//!
//! Emits the signal to notify connected objects that the scene node
//! contained in this container has been updated.
//!
void OgreContainer::updateCopies ()
{
    emit sceneNodeUpdated();
}


//!
//! Emits the signal to notify connected objects that the animation state
//! of the entity contained in this container has been updated.
//!
//! \param name The name of the animation state that has been updated.
//! \param progress The progress value for the animation state that has been updated.
//!
void OgreContainer::updateCopies ( const QString &name, double progress )
{
    emit animationStateUpdated(name, progress);
}


//!
//! Emits the signal to notify connected objects that the bone transformation
//! of the entity contained in this container has been updated.
//!
//! \param name The name of the bone that has been updated.
//! \param tx Translation x axis.
//! \param ty Translation y axis.
//! \param tz Translation z axis.
//! \param rx Rotation x axis.
//! \param ry Rotation y axis.
//! \param rz Rotation z axis.
//!
void OgreContainer::updateCopies ( const QString &name, double tx, double ty, double tz, double rx, double ry, double rz )
{
    emit boneTransformUpdated(name, tx, ty, tz, rx, ry, rz);
}


///
/// Public Slots
///


//!
//! Synchronizes the scene node contained in this container to the scene
//! node contained in the OgreContainer calling this slot.
//!
void OgreContainer::updateSceneNode ()
{
    if (!m_sceneNode)
        return;

    OgreContainer *ogreContainer = dynamic_cast<OgreContainer *>(sender());
    if (ogreContainer) {
        Ogre::SceneNode *sceneNode = ogreContainer->getSceneNode();
        if (sceneNode) {
            m_sceneNode->setPosition(sceneNode->getPosition());
            m_sceneNode->setOrientation(sceneNode->getOrientation());
            m_sceneNode->setScale(sceneNode->getScale());

            // notify connected objects that the scene node contained in this container has been updated
            emit sceneNodeUpdated();
        }
    }
}


//!
//! Synchronizes the camera contained in this container to the camera
//! contained in the OgreContainer calling this slot.
//!
void OgreContainer::updateCamera ()
{
    if (!m_camera)
        return;

    OgreContainer *ogreContainer = dynamic_cast<OgreContainer *>(sender());
    if (ogreContainer) {
        Ogre::Camera *camera = ogreContainer->getCamera();
        if (camera) {
            m_camera->setPosition(camera->getPosition());
            m_camera->setOrientation(camera->getOrientation());
        }
    }
}

//!
//! Synchronizes the light contained in this container to the light
//! contained in the OgreContainer calling this slot.
//!
void OgreContainer::updateLight ()
{
    if (!m_light)
        return;

    OgreContainer *ogreContainer = dynamic_cast<OgreContainer *>(sender());
    if (ogreContainer) {
        Ogre::Light *light = ogreContainer->getLight();
        if (light) {
            m_light->setType(light->getType());
            m_light->setDiffuseColour(light->getDiffuseColour());
            m_light->setSpecularColour(light->getSpecularColour());
            m_light->setAttenuation(light->getAttenuationRange(), light->getAttenuationConstant(), light->getAttenuationLinear(), light->getAttenuationQuadric());
            m_light->setPosition(light->getPosition());
            m_light->setDirection(light->getDirection());
            if (m_light->getType() == Ogre::Light::LT_SPOTLIGHT)
                m_light->setSpotlightRange(light->getSpotlightInnerAngle(), light->getSpotlightOuterAngle(), light->getSpotlightFalloff());
            m_light->setPowerScale(light->getPowerScale());
            m_light->setCastShadows(light->getCastShadows());
            emit sceneNodeUpdated();
        }
    }
}


//!
//! Updates the animation state with the given name for the entity
//! contained in this container to the given progress.
//!
//! \param name The name of the animation state to update.
//! \param progress The progress value for the animation state to update.
//!
void OgreContainer::updateAnimationState ( const QString &name, double progress )
{
    // check if an entity is contained in this container
    if (m_entity) {
        Ogre::AnimationState *animationState = m_entity->getAnimationState(name.toStdString());
        if (animationState) {
            animationState->setTimePosition(progress);

            // notify connected objects that the animation state has been updated
            emit animationStateUpdated(name, progress);
        }
    }
}

//!
//! Transforms a given bone.
//! 
//! \param name The name of the bone.
//! \param tx Translation x axis.
//! \param ty Translation y axis.
//! \param tz Translation z axis.
//! \param rx Rotation x axis.
//! \param ry Rotation y axis.
//! \param rz Rotation z axis.
//!
void OgreContainer::updateBoneTransform ( const QString &name, double tx, double ty, double tz, double rx, double ry, double rz )
{
	if (m_entity->hasSkeleton()) {
		Ogre::SkeletonInstance *skeletonInstance = m_entity->getSkeleton();
		if (skeletonInstance->hasBone(name.toStdString())) {
			Ogre::Bone *bone = skeletonInstance->getBone(name.toStdString());
			bone->setManuallyControlled(true);
			bone->reset();
			bone->translate(tx, ty, tz);
			bone->rotate(Ogre::Vector3::UNIT_X, Ogre::Radian(rx));
			bone->rotate(Ogre::Vector3::UNIT_Y, Ogre::Radian(ry));
			bone->rotate(Ogre::Vector3::UNIT_Z, Ogre::Radian(rz));
			emit boneTransformUpdated(name, tx, ty, tz, rx, ry, rz);
		}
	}
}


