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
//! \file "OgreTools.cpp"
//! \brief Implementation file for OgreTools class.
//!
//! \author     Stefan Habel <stefan.habel@filmakademie.de>
//! \author     Nils Zweiling <nils.zweiling@filmakademie.de>
//! \version    1.0
//! \date       29.06.2009 (last updated)
//!

#include "OgreTools.h"
#include "OgreContainer.h"


///
/// Public Static Functions
///


//!
//! Clones an Ogre::MovableObject.
//!
//! Is needed because OGRE does not provide clone functions for cameras and
//! lights.
//!
//! \param movableObject The object to clone.
//! \param name The name to use for the object.
//! \param sceneManager The scene manager to use for creating the object.
//! \return The cloned object.
//!
Ogre::MovableObject * OgreTools::cloneMovableObject ( Ogre::MovableObject *movableObject, const QString &name, Ogre::SceneManager *sceneManager /* =  0 */ )
{
    // make sure the given object is valid
    if (!movableObject) {
        Log::error("The given movable object is invalid.", "OgreTools::cloneMovableObject");
        return 0;
    }

    // make sure a valid scene manager is available
    if (!sceneManager)
        sceneManager = movableObject->_getManager();
    if (!sceneManager) {
        Log::error("No valid scene manager available.", "OgreTools::cloneMovableObject");
        return 0;
    }

    Ogre::MovableObject *result = 0;
    Ogre::String typeName = movableObject->getMovableType();
    if (typeName == "Entity") {
        // clone entity
        Ogre::Entity *entity = dynamic_cast<Ogre::Entity *>(movableObject);
        //movableObjectCopy = entity->clone(name.toStdString());
        Ogre::Entity *entityCopy = sceneManager->createEntity(name.toStdString(), entity->getMesh()->getName());
        Ogre::AnimationStateSet *animationStateSet = entity->getAllAnimationStates();
        Ogre::AnimationStateSet *animationStateSetCopy  = entityCopy->getAllAnimationStates();
        // set the same blend mode on entity copy
        if (entity && entityCopy) {
            if (entity->hasSkeleton() && entityCopy->hasSkeleton()) {
                Ogre::Skeleton *skeleton = entity->getSkeleton();
                Ogre::Skeleton *skeletonCopy = entityCopy->getSkeleton();
                skeletonCopy->setBlendMode(skeleton->getBlendMode());
            }
        }
        // copy all animation states
        if (animationStateSet && animationStateSetCopy) {
            Ogre::AnimationStateIterator animationStateIter = animationStateSet->getAnimationStateIterator();
            Ogre::AnimationStateIterator animationStateCopyIter = animationStateSetCopy->getAnimationStateIterator();
            while (animationStateIter.hasMoreElements()) {
                if (!animationStateCopyIter.hasMoreElements())
                    break;
                Ogre::AnimationState *animationState = animationStateIter.getNext();
                Ogre::AnimationState *animationStateCopy = animationStateCopyIter.getNext();
                animationStateCopy->setLoop(animationState->getLoop());
                //bool enabled = animationState->getEnabled();
                //animationStateCopy->setEnabled(animationState->getEnabled());
                animationStateCopy->setEnabled(true);
                animationStateCopy->setTimePosition(animationState->getTimePosition());
            }
        }

        // create a new container for the cloned entity
        OgreContainer *entityCopyContainer = new OgreContainer(entityCopy);
        entityCopy->setUserAny(Ogre::Any(entityCopyContainer));
        if (!entity->getUserAny().isEmpty()) {
            OgreContainer *entityContainer = Ogre::any_cast<OgreContainer *>(entity->getUserAny());
			if (entityContainer) {
                QObject::connect(entityContainer, SIGNAL(animationStateUpdated(const QString &, double)), entityCopyContainer, SLOT(updateAnimationState(const QString &, double)));
				QObject::connect(entityContainer, SIGNAL(boneTransformUpdated(const QString &, double, double, double, double, double, double)), entityCopyContainer, SLOT(updateBoneTransform(const QString &, double, double, double, double, double, double)));
			}
        }
        result = dynamic_cast<Ogre::MovableObject *>(entityCopy);
    } else if (typeName == "Light") {
        // clone light
        Ogre::Light *light = dynamic_cast<Ogre::Light *>(movableObject);
        Ogre::Light *lightCopy = sceneManager->createLight(name.toStdString());
        lightCopy->setType(light->getType());
        lightCopy->setDiffuseColour(light->getDiffuseColour());
        lightCopy->setSpecularColour(light->getSpecularColour());
        lightCopy->setAttenuation(light->getAttenuationRange(), light->getAttenuationConstant(), light->getAttenuationLinear(), light->getAttenuationQuadric());
        lightCopy->setPosition(light->getPosition());
        lightCopy->setDirection(light->getDirection());
        if (lightCopy->getType() == Ogre::Light::LT_SPOTLIGHT)
            lightCopy->setSpotlightRange(light->getSpotlightInnerAngle(), light->getSpotlightOuterAngle(), light->getSpotlightFalloff());
        lightCopy->setPowerScale(light->getPowerScale());
        lightCopy->setCastShadows(light->getCastShadows());

        // create a new container for the cloned light
        OgreContainer *lightCopyContainer = new OgreContainer(lightCopy);
        lightCopy->setUserAny(Ogre::Any(lightCopyContainer));
        if (!light->getUserAny().isEmpty()) {
            OgreContainer *lightContainer = Ogre::any_cast<OgreContainer *>(light->getUserAny());
            if (lightContainer)
                QObject::connect(lightContainer, SIGNAL(sceneNodeUpdated()), lightCopyContainer, SLOT(updateLight()));
        }
        result = dynamic_cast<Ogre::MovableObject *>(lightCopy);
    } else if (typeName == "Camera") {
        // clone camera
        Ogre::Camera *camera = dynamic_cast<Ogre::Camera *>(movableObject);
        Ogre::Camera *cameraCopy = sceneManager->createCamera(name.toStdString());
        //cameraCopy->setCustomParameter(0, camera->getCustomParameter(0));
        cameraCopy->setAspectRatio(camera->getAspectRatio());
        cameraCopy->setAutoAspectRatio(camera->getAutoAspectRatio());
        //cameraCopy->setAutoTracking(...);
        cameraCopy->setCastShadows(camera->getCastsShadows());
        //cameraCopy->setCullingFrustum(camera->getCullingFrustum());
        //cameraCopy->setCustomParameter(...);
        //cameraCopy->setCustomProjectionMatrix(..);
        //cameraCopy->setCustomViewMatrix(..);
        //cameraCopy->setDebugDisplayEnabled(...);
        //cameraCopy->setDefaultQueryFlags(...);
        //cameraCopy->setDefaultVisibilityFlags(...);
        cameraCopy->setDirection(camera->getDirection());
        //cameraCopy->setFixedYawAxis(...);
        cameraCopy->setFocalLength(camera->getFocalLength());
        cameraCopy->setFOVy(camera->getFOVy());

        //Ogre::Real left;
        //Ogre::Real right;
        //Ogre::Real top;
        //Ogre::Real bottom;
        //camera->getFrustumExtents(left, right, top, bottom);
        //cameraCopy->setFrustumExtents(left, right, top, bottom);
        //cameraCopy->setFrustumOffset(camera->getFrustumOffset());
        //cameraCopy->setListener(camera->getListener());
        cameraCopy->setLodBias(camera->getLodBias());
        //cameraCopy->setLodCamera(camera->getLodCamera());
        cameraCopy->setNearClipDistance(camera->getNearClipDistance());
        cameraCopy->setFarClipDistance(camera->getFarClipDistance());
        cameraCopy->setOrientation(camera->getOrientation());
        //cameraCopy->setOrthoWindow(...);
        //cameraCopy->setOrthoWindowHeight(...);
        //cameraCopy->setOrthoWindowWidth(...);
        cameraCopy->setPolygonMode(camera->getPolygonMode());
        cameraCopy->setPolygonModeOverrideable(camera->getPolygonModeOverrideable());
        cameraCopy->setPosition(camera->getPosition());
        cameraCopy->setProjectionType(camera->getProjectionType());
        cameraCopy->setQueryFlags(camera->getQueryFlags());
        cameraCopy->setRenderingDistance(camera->getRenderingDistance());
        cameraCopy->setRenderQueueGroup(camera->getRenderQueueGroup());
        //cameraCopy->setRenderSystemData(camera->getRenderSystemData());
        cameraCopy->setUseIdentityProjection(camera->getUseIdentityProjection());
        cameraCopy->setUseIdentityView(camera->getUseIdentityView());
        //cameraCopy->setUserAny(camera->getUserAny());
        cameraCopy->setUseRenderingDistance(camera->getUseRenderingDistance());
        //cameraCopy->setUserObject(camera->getUserObject());
        cameraCopy->setVisibilityFlags(camera->getVisibilityFlags());
        cameraCopy->setVisible(camera->getVisible());
        //cameraCopy->setWindow(...);

        if (!movableObject->getUserAny().isEmpty()) {
            CameraInfo *sourceCameraInfo = Ogre::any_cast<CameraInfo *>(movableObject->getUserAny());
            if (sourceCameraInfo) {
                CameraInfo *targetCameraInfo = new CameraInfo();
                targetCameraInfo->width = sourceCameraInfo->width;
                targetCameraInfo->height = sourceCameraInfo->height;
                dynamic_cast<Ogre::MovableObject *>(cameraCopy)->setUserAny(Ogre::Any(targetCameraInfo));
            }
        }

        //// Setup connections for instances
        //SceneNode *targetSceneNode = new SceneNode(cameraCopy);
        //((Ogre::MovableObject *)cameraCopy)->setUserAny(Ogre::Any(targetSceneNode));
        //if (!((Ogre::MovableObject *)camera)->getUserAny().isEmpty()) {
        //    SceneNode *sourceSceneNode = Ogre::any_cast<SceneNode *>(((Ogre::MovableObject *)camera)->getUserAny());
        //    if (sourceSceneNode) {
        //        QObject::connect(sourceSceneNode, SIGNAL(sceneNodeUpdated()), targetSceneNode, SLOT(updateSceneNode()));
        //    }
        //}

        result = dynamic_cast<Ogre::MovableObject *>(cameraCopy);
    }

    if (!result)
        Log::error(QString("Could not clone movable object \"%1\" of type \"%2\".").arg(movableObject->getName().c_str()).arg(typeName.c_str()), "OgreTools::cloneMovableObject");

    return result;
}


//!
//! Creates a deep copy of the given scene node.
//!
//! \param sceneNode The scene node to copy.
//! \param sceneNodeCopy The scene node to add the copied objects to (will be created if 0).
//! \param namePrefix The prefix to use for names of copied objects.
//! \param sceneManager The scene manager to use for creating the object.
//!
void OgreTools::deepCopySceneNode ( Ogre::SceneNode *sceneNode, Ogre::SceneNode *&sceneNodeCopy, const QString &namePrefix, Ogre::SceneManager *sceneManager /* = 0 */ )
{
    // make sure the given scene node is valid
    if (!sceneNode) {
        Log::error("The given scene node is invalid.", "OgreTools::deepCopySceneNode");
        return;
    }

    // make sure a valid scene manager is available
    if (!sceneManager)
        sceneManager = sceneNode->getCreator();
    if (!sceneManager) {
        Log::error("No valid scene manager available.", "OgreTools::deepCopySceneNode");
        return;
    }

    // create the target scene node if it doesn't exist yet
    if (!sceneNodeCopy) {
        QString sceneNodeCopyName = QString("%1_%2Copy").arg(namePrefix).arg(sceneNode->getName().c_str());
        sceneNodeCopy = copySceneNode(sceneNode, sceneNodeCopyName, sceneManager);
        if (!sceneNodeCopy) {
            Log::error("The scene node could not be copied.", "OgreTools::deepCopySceneNode");
            return;
        }
    }

    // iterate over the list of attached objects
    Ogre::SceneNode::ObjectIterator objectIterator = sceneNode->getAttachedObjectIterator();
    while (objectIterator.hasMoreElements()) {
        Ogre::MovableObject *movableObject = objectIterator.getNext();
        if (movableObject) {
            QString entityCopyName = QString("%1_%2Copy").arg(namePrefix).arg(movableObject->getName().c_str());
            Ogre::MovableObject *movableObjectCopy = cloneMovableObject(movableObject, entityCopyName, sceneManager);
            if (movableObjectCopy)
                sceneNodeCopy->attachObject(movableObjectCopy);
        }
    }

    // iterate over the list of child nodes
    Ogre::SceneNode::ChildNodeIterator childNodeIterator = sceneNode->getChildIterator();
    while (childNodeIterator.hasMoreElements() ) {
        Ogre::SceneNode *childSceneNode = (Ogre::SceneNode *) childNodeIterator.getNext();
        QString childSceneNodeCopyName = QString("%1_%2Copy").arg(namePrefix).arg(childSceneNode->getName().c_str());
        Ogre::SceneNode *childSceneNodeCopy = copySceneNode(childSceneNode, childSceneNodeCopyName, sceneManager);
        if (childSceneNodeCopy) {
            sceneNodeCopy->addChild(childSceneNodeCopy);
            deepCopySceneNode(childSceneNode, childSceneNodeCopy, namePrefix, sceneManager);
        }
    }
}


//!
//! Creates a copy of the given scene node.
//!
//! \param sceneNode The scene node to copy.
//! \param name The name to use for the copied scene node.
//! \param sceneManager The scene manager to use for creating the scene node.
//! \return A copy of the given scene node.
//!
Ogre::SceneNode * OgreTools::copySceneNode ( Ogre::SceneNode *sceneNode, const QString &name, Ogre::SceneManager *sceneManager /* = 0 */ )
{
    // make sure the given scene node is valid
    if (!sceneNode) {
        Log::error("The given scene node is invalid.", "OgreTools::copySceneNode");
        return 0;
    }

    // make sure a valid scene manager is available
    if (!sceneManager)
        sceneManager = sceneNode->getCreator();
    if (!sceneManager) {
        Log::error("No valid scene manager available.", "OgreTools::copySceneNode");
        return 0;
    }

    // check if a scene node of the given name already exists
    if (sceneManager->hasSceneNode(name.toStdString())) {
        Log::error(QString("The scene manager already contains a scene node named \"%1\".").arg(name), "OgreTools::copySceneNode");
        return 0;
    }

    // create the scene node copy
    Ogre::SceneNode *sceneNodeCopy = sceneManager->createSceneNode(name.toStdString());
    if (!sceneNodeCopy) {
        Log::error("The scene node copy could not be created.", "OgreTools::copySceneNode");
        return 0;
    }

    // create a container for the scene node copy
    OgreContainer *sceneNodeCopyContainer = new OgreContainer(sceneNodeCopy);
    sceneNodeCopy->setUserAny(Ogre::Any(sceneNodeCopyContainer));

    const Ogre::Any &userAny = sceneNode->getUserAny();
    userAny.isEmpty();
    if (!sceneNode->getUserAny().isEmpty()) {
        OgreContainer *sceneNodeContainer = Ogre::any_cast<OgreContainer *>(sceneNode->getUserAny());
        if (sceneNodeContainer)
            QObject::connect(sceneNodeContainer, SIGNAL(sceneNodeUpdated()), sceneNodeCopyContainer, SLOT(updateSceneNode()));
    }

    // copy parameters from scene node to scene node copy
    //sceneNodeCopy->setAutoTracking(...);
    //sceneNodeCopy->setCustomParameter(...);
    //sceneNodeCopy->setDebugDisplayEnabled(...);
    //sceneNodeCopy->setDirection(...);
    //sceneNodeCopy->setFixedYawAxis(...);
    sceneNodeCopy->setInheritOrientation(sceneNode->getInheritOrientation());
    sceneNodeCopy->setInheritScale(sceneNode->getInheritScale());
    //sceneNodeCopy->setInitialState(...);
    //sceneNodeCopy->setInSceneGraph(...);
    sceneNodeCopy->setListener(sceneNode->getListener());
    sceneNodeCopy->setOrientation(sceneNode->getOrientation());
    //sceneNodeCopy->setParent(...);
    sceneNodeCopy->setPolygonModeOverrideable(sceneNode->getPolygonModeOverrideable());
    sceneNodeCopy->setPosition(sceneNode->getPosition());
    //sceneNodeCopy->setRenderSystemData(...);
    sceneNodeCopy->setScale(sceneNode->getScale());
    sceneNodeCopy->setUseIdentityProjection(sceneNode->getUseIdentityProjection());
    sceneNodeCopy->setUseIdentityView(sceneNode->getUseIdentityView());
    //sceneNodeCopy->getUserAny(...);
    //sceneNodeCopy->setVisible(...);
    return sceneNodeCopy;
}

//!
//! Deletes a whole scene node tree (including attached objects).
//!
//! \param sceneNode The scene node containing the tree to delete.
//! \param sceneManager The scene manager to use for destroying the scene node.
//! \param deleteRoot Flag to control whether to delete the given scene node.
//!
void OgreTools::deepDeleteSceneNode ( Ogre::SceneNode *sceneNode, Ogre::SceneManager *sceneManager /* = 0 */, bool deleteRoot /* = false */ )
{
    // make sure the given scene node is valid
    if (!sceneNode) {
        Log::error("The given scene node is invalid.", "OgreTools::deepDeleteSceneNode");
        return;
    }

    // make sure a valid scene manager is available
    if (!sceneManager)
        sceneManager = sceneNode->getCreator();
    if (!sceneManager) {
        Log::error("No valid scene manager available.", "OgreTools::deepDeleteSceneNode");
        return;
    }

    // iterate over the list of attached objects
    Ogre::SceneNode::ObjectIterator objectIterator = sceneNode->getAttachedObjectIterator();
    while (objectIterator.hasMoreElements()) {
        Ogre::MovableObject *movableObject = objectIterator.getNext();
        sceneNode->detachObject(movableObject);
        Ogre::SceneManager *movableSceneManager = movableObject->_getManager();
        if (!movableSceneManager) {
            Log::error("The Ogre scene manager could not be obtained.", "OgreTools::deepDeleteSceneNode");
            return;
        }
        Ogre::Any customData = movableObject->getUserAny();
        if (!customData.isEmpty()) {
            OgreContainer *ogreContainer = Ogre::any_cast<OgreContainer *>(customData);
            if (ogreContainer) {
                delete ogreContainer;
                ogreContainer = 0;
            }
        }
        movableSceneManager->destroyMovableObject(movableObject);
    }

    // iterate over the list of child nodes
    Ogre::SceneNode::ChildNodeIterator childNodeIterator = sceneNode->getChildIterator();
    while (childNodeIterator.hasMoreElements()) {
        Ogre::SceneNode *childSceneNode = dynamic_cast<Ogre::SceneNode *>(childNodeIterator.getNext());
        if (childSceneNode) {
            Ogre::Any customData = childSceneNode->getUserAny();
            if (!customData.isEmpty()) {
                OgreContainer *ogreContainer = Ogre::any_cast<OgreContainer *>(customData);
                if (ogreContainer) {
                    delete ogreContainer;
                    ogreContainer = 0;
                } else {
                    CameraInfo *cameraInfo = Ogre::any_cast<CameraInfo *>(customData);
                    if (cameraInfo) {
                        delete cameraInfo;
                        cameraInfo = 0;
                    }
                }
            }
            deepDeleteSceneNode(childSceneNode, sceneManager);
        }
    }

    // destroy all child nodes of the given scene node
    sceneNode->removeAndDestroyAllChildren();

    // check if the given scene node should be destroyed as well
    if (deleteRoot) {
        Ogre::Any customData = sceneNode->getUserAny();
        if (!customData.isEmpty()) {
            OgreContainer *ogreContainer = Ogre::any_cast<OgreContainer *>(customData);
            if (ogreContainer) {
                delete ogreContainer;
                ogreContainer = 0;
            } else {
                CameraInfo *cameraInfo = Ogre::any_cast<CameraInfo *>(customData);
                if (cameraInfo) {
                    delete cameraInfo;
                    cameraInfo = 0;
                }
            }
        }
        sceneManager->destroySceneNode(sceneNode);
    }
}

//!
//! Returns the first entity attached to the given scene node.
//!
//! \param sceneNode The scene node to return the entity from.
//! \return The first entity attached to the given scene node.
//!
Ogre::Entity * OgreTools::getFirstEntity ( Ogre::SceneNode *sceneNode )
{
    return dynamic_cast<Ogre::Entity *>(findFirstObject(sceneNode, "Entity"));
}

//!
//! Returns the first camera attached to the given scene node.
//!
//! \param sceneNode The scene node to return the camera from.
//! \return The first camera attached to the given scene node.
//!
Ogre::Camera * OgreTools::getFirstCamera ( Ogre::SceneNode *sceneNode )
{
    return dynamic_cast<Ogre::Camera *>(findFirstObject(sceneNode, "Camera"));
}

//!
//! Returns the first light attached to the given scene node.
//!
//! \param sceneNode The scene node to return the light from.
//! \return The first light attached to the given scene node.
//!
Ogre::Light * OgreTools::getFirstLight ( Ogre::SceneNode *sceneNode )
{
    return dynamic_cast<Ogre::Light *>(findFirstObject(sceneNode, "Light"));
}


///
/// Private Static Functions
///


//!
//! Returns the first movable object of the given type name contained in
//! the given scene node.
//!
//! \param sceneNode The scene node to find the first object in.
//! \param typeName The name of the type of objects to look for.
//! \return The first movable object of the given type name contained in the given scene node.
//!
Ogre::MovableObject * OgreTools::findFirstObject ( Ogre::SceneNode *sceneNode, const QString &typeName )
{
    // make sure the given scene node is valid
    if (!sceneNode) {
        Log::error("The given scene node is invalid.", "OgreTools::findFirstObject");
        return 0;
    }

    // iterate over the list of attached objects
    Ogre::SceneNode::ObjectIterator objectIterator = sceneNode->getAttachedObjectIterator();
    while (objectIterator.hasMoreElements()) {
        Ogre::MovableObject *movableObject = objectIterator.getNext();
        if (movableObject && QString::compare(movableObject->getMovableType().c_str(), typeName) == 0)
            return movableObject;
    }

    // iterate over the list of child nodes
    Ogre::SceneNode::ChildNodeIterator childIter = sceneNode->getChildIterator();
    while( childIter.hasMoreElements() ) {
        Ogre::SceneNode *childSceneNode = (Ogre::SceneNode *) childIter.getNext();
        return findFirstObject(childSceneNode, typeName);
    }

    return 0;
}

//!
//! Destroy the node's Ogre resource group.
//!
void OgreTools::destroyResourceGroup ( const QString &name )
{
	Ogre::StringVector groupNames = Ogre::ResourceGroupManager::getSingleton().getResourceGroups();
	if (std::find(groupNames.begin(), groupNames.end(), name.toStdString()) != groupNames.end()) {
		Ogre::ResourceGroupManager::getSingleton().destroyResourceGroup(name.toStdString());
	}
}

//!
//! Create the node's Ogre resource group.
//!
void OgreTools::createResourceGroup ( const QString &name, const QString &path )
{
	Ogre::ResourceGroupManager::getSingleton().createResourceGroup(name.toStdString());
	Ogre::ResourceGroupManager::getSingleton().addResourceLocation(path.toStdString(), "FileSystem", name.toStdString());
	Ogre::ResourceGroupManager::getSingleton().initialiseResourceGroup(name.toStdString());
	Ogre::ResourceGroupManager::getSingleton().loadResourceGroup(name.toStdString());
}

