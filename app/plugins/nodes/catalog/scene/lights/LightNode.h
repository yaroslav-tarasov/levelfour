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
//! \file "LightNode.h"
//! \brief Header file for LightNode class.
//!
//! \author     Nils Zweiling <nils.zweiling@filmakademie.de>
//! \author     Stefan Habel <stefan.habel@filmakademie.de>
//! \version    1.0
//! \date       29.06.2009 (last updated)
//!

#ifndef LIGHTNODE_H
#define LIGHTNODE_H

#include "ViewNode.h"
#include "OgreContainer.h"
#include "InstanceCounterMacros.h"

// OGRE
#include <Ogre.h>
#if (OGRE_PLATFORM  == OGRE_PLATFORM_WIN32)
#include <windows.h>
#endif


//!
//! Class representing nodes for lights in a 3D scene.
//!
class LightNode : public ViewNode
{

    Q_OBJECT
    ADD_INSTANCE_COUNTER

public: // constructors and destructors

    //!
    //! Constructor of the LightNode class.
    //!
    //! \param name The name for the new node.
    //! \param parameterRoot A copy of the parameter tree specific for the type of the node.
    //!
    LightNode ( const QString &name, ParameterGroup *parameterRoot );

    //!
    //! Destructor of the LightNode class.
    //!
    //! Defined virtual to guarantee that the destructor of a derived class
    //! will be called if the instance of the derived class is saved in a
    //! variable of its parent class type.
    //!
    virtual ~LightNode ();

public: // functions

    //!
    //! Returns the scene node that contains the light which
    //! visually represents this node.
    //!
    //! \return Scene node containing the entity which visually represents this light.
    //!
    virtual Ogre::SceneNode * getSceneNode ();


private slots: //

    //!
    //! Applies the currently set position for the node to the OGRE scene
    //! objects contained in this node.
    //!
    void applyPosition ();

    //!
    //! Applies the currently set orientation for the node to the OGRE scene
    //! objects contained in this node.
    //!
    void applyOrientation ();

    //!
    //! Applies all additional parameters of the light node.
    //!
    //! Is called when any of the additional parameters of the light node has been
    //! changed
    //!
    void applyParameters ();


private: // data

    //!
    //! The scene manager.
    //!
    Ogre::SceneManager *m_sceneManager;

    //!
    //! The scene node the light is attached to.
    //!
    Ogre::SceneNode *m_sceneNode;

    //!
    //! The ogre scene node container.
    //!
    OgreContainer *m_ogreContainer;

    //!
    //! The ogre light node container.
    //!
    OgreContainer *m_ogreLightContainer;

    //!
    //! The light that this node represents.
    //!
    Ogre::Light *m_light;

    //!
    //! The entity that visually represents this light.
    //!
    Ogre::Entity *m_entity;

    //!
    //! The name of the output light parameter.
    //!
    QString m_outputLightName;
};


#endif
