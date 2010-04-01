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
//! \file "AnimatableMeshNode.h"
//! \brief Header file for AnimatableMeshNode class.
//!
//! \author     Nils Zweiling <nils.zweiling@filmakademie.de>
//! \author     Stefan Habel <stefan.habel@filmakademie.de>
//! \version    1.0
//! \date       29.06.2009 (last updated)
//!

#ifndef ANIMATABLEMESHNODE_H
#define ANIMATABLEMESHNODE_H

#include "GeometryNode.h"
#include "OgreContainer.h"
#include "Ogre.h"
#include "OgreTools.h"


//!
//! Class representing nodes that can contain OGRE entities with animation.
//!
class AnimatableMeshNode : public GeometryNode
{

    Q_OBJECT
        ADD_INSTANCE_COUNTER

public: // nested type definitions

    //!
    //! Bone transformation struct
    //!
    struct BoneTransform {
        Ogre::Vector3 translate;
        Ogre::Vector3 rotate;
        Ogre::Vector3 size;
    };

public: // constructors and destructors

    //!
    //! Constructor of the AnimatableMeshNode class.
    //!
    //! \param name The name to give the new mesh node.
    //! \param parameterRoot A copy of the parameter tree specific for the type of the node.
    //!
    AnimatableMeshNode ( const QString &name, ParameterGroup *parameterRoot );

    //!
    //! Destructor of the AnimatableMeshNode class.
    //!
    ~AnimatableMeshNode ();

public slots: //

    //!
    //! Update an animation.
    //!
    void updateAnimation ();

    //!
    //! Update bones.
    //!
    void updateBones ();

    //!
    //! Update poses.
    //!
    void updatePoses ();

    //!
    //! Update animations, bones and poses.
    //!
    void updateAll ();

private: // functions

    //!
    //! Destroy the node's Ogre resource group.
    //!
    void destroyResourceGroup ();

    //!
    //! Create the node's Ogre resource group.
    //!
    void createResourceGroup ( const QString &name, const QString &path );

    //!
    //! Initialize all animation states of this object (OGRE-specific).
    //!
    //! \return True if the animation states where successfully initialized, otherwise False.
    //!
    bool initAnimationStates ();

    //!
    //! Retrieve the names of all animations of skeleton.
    //!
    //! \return List with names of animations.
    //!
    QStringList getAnimationNames ();

    //!
    //! Returns the names of all bones in the skeleton for the mesh.
    //!
    //! \return The names of all bones in the skeleton for the mesh.
    //!
    QStringList getBoneNames ();

    //!
    //! Initialize a bunch of bones.
    //!
    //! \param boneNames The list of names of bones to initialize.
    //!
    void initializeBones ( QStringList boneNames );

    //!
    //! Loads the animation mesh from file.
    //!
    //! \return True if the animation mesh was successfully loaded, otherwise False.
    //!
    bool loadMesh ();

    //!
    //! Retrieves the numbers of vertices and triangles from the mesh and stores
    //! them in parameters of this node.
    //!
    void updateStatistics ();

    //!
    //! Removes the OGRE entity containing the mesh geometry from the scene and
    //! destroys it along with the OGRE scene node.
    //!
    void destroyEntity ();

    //!
    //! Adds the given time value to the animation state with the given name in
    //! order to progess the animation (OGRE-specific).
    //!
    //! \param aUnitName The name of the animation state to progress.
    //! \param timeToAdd The time to add to the animation state with the given name.
    //!
    void progressAnimation ( const QString &aUnitName, double timeToAdd );

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
    void transformBone ( const QString &name, double tx, double ty, double tz, double rx, double ry, double rz );

    //!
    //! Parse the Material Parameters and add to Material Parameter Group.
    //!
    void parseMaterialParameters ();


private slots: //

    //!
    //! Change function for the Geometry File parameter.
    //!
    void geometryFileChanged ();

    //!
    //! Change function for the Material parameters.
    //!
    void setMaterialParameter ();

    //!
    //! Check existing material for BRDF shader and set the loaded light textures.
    //! 
    //! \return True when BRDF material was found.
    //!
    bool setupMaterial ();

    //!
    //! Loads an light description file.
    //!
    //! \return True when light description file has been successfully loaded.
    //!
    bool loadLightDescriptionFile ();

private: // data

    //!
    //! OGRE scene node.
    //!
    Ogre::SceneNode *m_sceneNode;

    //!
    //! OGRE entity.
    //!
    Ogre::Entity *m_entity;

    //!
    //! Old resource group name.
    //!
    QString m_oldResourceGroupName;

    //!
    //! The container for the entity.
    //!
    OgreContainer *m_entityContainer;

    //!
    //! OGRE animation states.
    //!
    QMap<QString, Ogre::AnimationState *> m_animStates;

    //!
    //! Animation progresses.
    //!
    QMap<QString, double> m_animProgresses;

    //!
    //! List of animation names.
    //!
    QStringList m_animationNames;

    //!
    //! List of pose names.
    //!
    QStringList m_poseNames;

    //!
    //! List of bone names.
    //!
    QStringList m_boneNames;

    //!
    //! OGRE animation states for vertex animations.
    //!
    QMap<QString, Ogre::AnimationState *> m_animStatesVertex;

    //!
    //! Parameter group for skeletal animations
    //!
    ParameterGroup *m_animationGroup;

    //!
    //! Parameter group for pose animations
    //!
    ParameterGroup *m_poseGroup;

    //!
    //! Parameter group bone groups
    //!
    ParameterGroup *m_boneGroup;

    //!
    //! Parameter group material parameters
    //!
    ParameterGroup *m_materialGroup;

    //!
    //! Texture containing light directions (BRDF)
    //!
    Ogre::TexturePtr m_lightDirsTexture;

    //!
    //! Texture containing light powers (BRDF)
    //!
    Ogre::TexturePtr m_lightPowsTexture;

};


#endif
