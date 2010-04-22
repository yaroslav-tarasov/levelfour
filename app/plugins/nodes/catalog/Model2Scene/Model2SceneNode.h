#ifndef Model2SceneNODE_H
#define Model2SceneNODE_H

#include "GeometryNode.h"
#include "OgreContainer.h"
#include "Ogre.h"
#include "OgreTools.h"


//!
//! Class representing nodes that can contain OGRE entities with animation.
//!
class Model2SceneNode : public GeometryNode
{

    Q_OBJECT
        ADD_INSTANCE_COUNTER

public: // nested type definitions

public: // constructors and destructors

    //!
    //! Constructor of the Model2SceneNode class.
    //!
    //! \param name The name to give the new mesh node.
    //! \param parameterRoot A copy of the parameter tree specific for the type of the node.
    //!
    Model2SceneNode ( const QString &name, ParameterGroup *parameterRoot );

    //!
    //! Destructor of the Model2SceneNode class.
    //!
    ~Model2SceneNode ();

public slots: //

    //!
    //! Processes the node's input data to generate the node's output table.
    //!
    void processScene();


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

private slots: //

    //!
    //! Change function for the Geometry File parameter.
    //!
    void geometryFileChanged ();

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
    //! The name of the input vtk table parameter.
    //!
    QString m_inputVTKTableParameterName;
};


#endif
