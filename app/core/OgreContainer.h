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
//! \file "OgreContainer.h"
//! \brief Header file for OgreContainer class.
//!
//! \author     Stefan Habel <stefan.habel@filmakademie.de>
//! \author     Nils Zweiling <nils.zweiling@filmakademie.de>
//! \version    1.0
//! \date       29.06.2009 (last updated)
//!

#ifndef OGRECONTAINER_H
#define OGRECONTAINER_H

#include "FrapperPrerequisites.h"
#include <QtCore/QObject>
#include "InstanceCounterMacros.h"
#include <Ogre.h>
#if (OGRE_PLATFORM  == OGRE_PLATFORM_WIN32)
#include <windows.h>
#endif


//!
//! Class representing a container for OGRE objects like scene nodes and
//! various types of movable objects.
//!
class FRAPPER_CORE_EXPORT OgreContainer : public QObject
{

    Q_OBJECT
    ADD_INSTANCE_COUNTER

public: // constructors and destructors

    //!
    //! Constructor of the OgreContainer class.
    //!
    //! \param sceneNode The scene node to be contained in the container.
    //!
    OgreContainer ( Ogre::SceneNode *sceneNode );

    //!
    //! Constructor of the OgreContainer class.
    //!
    //! \param entity The entity to be contained in the container.
    //!
    OgreContainer ( Ogre::Entity *entity );

    //!
    //! Constructor of the OgreContainer class.
    //!
    //! \param light The light to be contained in the container.
    //!
    OgreContainer ( Ogre::Light *light );

    //!
    //! Constructor of the OgreContainer class.
    //!
    //! \param camera The camera to be contained in the container.
    //!
    OgreContainer ( Ogre::Camera *camera );

    //!
    //! Destructor of the OgreContainer class.
    //!
    ~OgreContainer ();

public: // functions

    //!
    //! Returns the scene node contained in this container.
    //!
    //! \return The scene node contained in this container.
    //!
    Ogre::SceneNode * getSceneNode ();

    //!
    //! Sets the scene node to be contained in this container.
    //!
    //! \param sceneNode The scene node to be contained in this container.
    //!
    void setSceneNode ( Ogre::SceneNode *sceneNode );

    //!
    //! Returns the entity contained in this container.
    //!
    //! \return The entity contained in this container.
    //!
    Ogre::Entity * getEntity ();

    //!
    //! Sets the entity to be contained in this container.
    //!
    //! \param entity The entity to be contained in this container.
    //!
    void setEntity ( Ogre::Entity *entity );

    //!
    //! Returns the light contained in this container.
    //!
    //! \return The light contained in this container.
    //!
    Ogre::Light * getLight ();

    //!
    //! Sets the light to be contained in this container.
    //!
    //! \param light The light to be contained in this container.
    //!
    void setLight ( Ogre::Light *light );

    //!
    //! Returns the camera contained in this container.
    //!
    //! \return The camera contained in this container.
    //!
    Ogre::Camera * getCamera ();

    //!
    //! Sets the camera to be contained in this container.
    //!
    //! \param camera The camera to be contained in this container.
    //!
    void setCamera ( Ogre::Camera *camera );

    //!
    //! Emits the signal to notify connected objects that the scene node
    //! contained in this container has been updated.
    //!
    void updateCopies ();

    //!
    //! Emits the signal to notify connected objects that the animation state
    //! of the entity contained in this container has been updated.
    //!
    //! \param name The name of the animation state that has been updated.
    //! \param progress The progress value for the animation state that has been updated.
    //!
    void updateCopies ( const QString &name, double progress );

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
	void updateCopies ( const QString &name, double tx, double ty, double tz, double rx, double ry, double rz );

signals: //

    //!
    //! Signal to notify connected objects that the scene node contained in
    //! this container has changed.
    //!
    void sceneNodeUpdated ();

    //!
    //! Signal to notify connected objects that the animation state for the
    //! entity contained in this container has been updated.
    //!
    //! \param name The name of the animation state that was updated.
    //! \param progress The progress value for the animation state that was updated.
    //!
    void animationStateUpdated ( const QString &name, double progress );

	//!
	//! Signal to notify connected objects that the bone transformation
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
	void boneTransformUpdated ( const QString &name, double tx, double ty, double tz, double rx, double ry, double rz );

public slots: //

    //!
    //! Synchronizes the scene node contained in this container to the scene
    //! node contained in the OgreContainer calling this slot.
    //!
    void updateSceneNode ();

    //!
    //! Synchronizes the camera contained in this container to the camera
    //! contained in the OgreContainer calling this slot.
    //!
    void updateCamera ();

    //!
    //! Synchronizes the light contained in this container to the light
    //! contained in the OgreContainer calling this slot.
    //!
    void updateLight ();

    //!
    //! Updates the animation state with the given name for the entity
    //! contained in this container to the given progress.
    //!
    //! \param name The name of the animation state to update.
    //! \param progress The progress value for the animation state to update.
    //!
    void updateAnimationState ( const QString &name, double progress );

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
	void updateBoneTransform ( const QString &name, double tx, double ty, double tz, double rx, double ry, double rz );

private: // data

    //!
    //! The container associated with this container.
    //!
    OgreContainer *m_ogreContainer;

    //!
    //! The scene node contained in the container.
    //!
    Ogre::SceneNode *m_sceneNode;

    //!
    //! The entity contained in the container.
    //!
    Ogre::Entity *m_entity;

    //!
    //! The light contained in the container.
    //!
    Ogre::Light *m_light;

    //!
    //! The camera contained in the container.
    //!
    Ogre::Camera *m_camera;

};

#endif
