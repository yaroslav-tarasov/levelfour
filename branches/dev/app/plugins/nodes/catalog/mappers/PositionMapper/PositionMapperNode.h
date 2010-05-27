#ifndef PositionMapperNODE_H
#define PositionMapperNODE_H

#include "GeometryNode.h"
#include "OgreContainer.h"
#include "Ogre.h"
#include "OgreTools.h"
#include "vtkTable.h"


//!
//! Class representing nodes that can contain OGRE entities with animation.
//!
class PositionMapperNode : public GeometryNode
{

    Q_OBJECT
        ADD_INSTANCE_COUNTER

public: // nested type definitions

public: // constructors and destructors

    //!
    //! Constructor of the PositionMapperNode class.
    //!
    //! \param name The name to give the new mesh node.
    //! \param parameterRoot A copy of the parameter tree specific for the type of the node.
    //!
    PositionMapperNode ( const QString &name, ParameterGroup *parameterRoot );

    //!
    //! Destructor of the PositionMapperNode class.
    //!
    ~PositionMapperNode ();

public slots: //

    //!
    //! Processes the node's input data to generate the node's output table.
    //!
    void processScene();

    //!
    //! Processes the node's parameters.
    //!
    void processParameters();

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

    //!
    //! Create new scene.
    //! \return True if the scene was successfully created, otherwise False.
    //!
	bool createEntity(QString name, QString fileName);

	//!
    //! Remove and destroy this scene.
    //!
	void destroySceneNode();

	//!
    //! Remove and destroy all movable objects of this scene.
    //! \param The scenenode to be destroyed
    //!
	void destroyAllAttachedMovableObjects( Ogre::SceneNode* i_pSceneNode );

	//!
    //! Remove and destroy this scene and all its children.
    //! \param The scenenode to be destroyed
    //!
	void destroyAllChildren( Ogre::SceneNode* i_pSceneNode );

	//!
    //! Create new scene.
    //! \return True if the scene was successfully created, otherwise False.
    //!
	bool createSceneNode();

private slots: //

    //!
    //! Change function for the Geometry File parameter.
    //!
    void geometryFileChanged ();

	//!
    //! Set the Geometry input.
    //!
    void setGeometry();

	//!
    //! Update the input table 
    //! \return True if it succeeds, otherwise false
    //!
    bool updateTable ();
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

    //!
    //! The input table 
    //!
	vtkTable * m_inputTable;

	//!
    //! The name of the input geometry parameter.
    //!
    QString m_inputGeometryParameterName;

	//!
    //! The parameters that defines the edgeFrom/To and vertices for the graph.
    //!
	EnumerationParameter * idFieldParameter, * xFieldParameter, * yFieldParameter, * zFieldParameter;
};


#endif
