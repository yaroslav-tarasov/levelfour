
#include "EntityNode.h"
#include "ParameterGroup.h"
#include "NumberParameter.h"
#include "SceneNodeParameter.h"
#include "FilenameParameter.h"
#include "OgreManager.h"
#include "OgreTools.h"
#include <QtCore/QFile>
#include <QtCore/QDir>
#include <QtCore/QTime>
#include "VTKTableParameter.h"
#include "vtkVariantArray.h"
#include "vtkDoubleArray.h"
#include "vtkIdTypeArray.h"
#include "OgreManualObject.h"
#include "OgreRenderOperation.h"
INIT_INSTANCE_COUNTER(EntityNode)


///
/// Constructors and Destructors
///


//!
//! Constructor of the EntityNode class.
//!
//! \param name The name to give the new mesh node.
//! \param parameterRoot A copy of the parameter tree specific for the type of the node.
//!
EntityNode::EntityNode ( const QString &name, ParameterGroup *parameterRoot ) :
    Node(name, parameterRoot),
	m_entity(0), 
    m_entityContainer(0),
    m_oldResourceGroupName("")
{
	// create the geometry output parameter (shape mapper)
	m_outputGeometryParameter = new EntityParameter(m_outputGeometryParameterName);
	m_outputGeometryParameter->setPinType(Parameter::PT_Output);
	parameterRoot->addParameter(m_outputGeometryParameter);

	// set affections and functions
    addAffection("Entity File", m_outputGeometryParameterName);
    setChangeFunction("Entity File", SLOT(geometryFileChanged()));

	INC_INSTANCE_COUNTER
}


//!
//! Destructor of the EntityNode class.
//!
EntityNode::~EntityNode ()
{
	destroyEntity();
    OgreTools::destroyResourceGroup(m_oldResourceGroupName);

	emit destroyed();
	Log::info(QString("EntityNode destroyed."), "EntityNode::~EntityNode");

    DEC_INSTANCE_COUNTER
}


///
/// Public Slots
///

///
/// Private Functions
///

//!
//!
//! \return True, if successful loading of ogre mesh
//!            False, otherwise.
//!
bool EntityNode::loadMesh ()
{
    // destroy an existing OGRE entity for the mesh
    destroyEntity();

	QString filename = getStringValue("Entity File");
    if (filename == "") {
        Log::debug(QString("Entity file has not been set yet. (\"%1\")").arg(m_name), "EntityNode::loadMesh");
        return false;
    }

	// check if the file exists
    if (!QFile::exists(filename)) {
        Log::error(QString("Mesh file \"%1\" not found.").arg(filename), "EntityNode::loadMesh");
        return false;
    }

    // split the absolute filename to path and base filename
    int lastSlashIndex = filename.lastIndexOf('/');
    QString path = "";
    if (lastSlashIndex > -1) {
        path = filename.mid(0, lastSlashIndex);
        filename = filename.mid(lastSlashIndex + 1);
    }
    if (!filename.endsWith(".mesh")) {
        Log::error("The Entity file has to be an OGRE mesh file.", "EntityNode::loadMesh");
        return false;
    }
	// destroy old resource group and generate new one
    QString resourceGroupName = QString::fromStdString(createUniqueName("ResourceGroup_" + filename + "_EntityNode"));
    OgreTools::destroyResourceGroup(m_oldResourceGroupName);
    m_oldResourceGroupName = resourceGroupName;
    OgreTools::createResourceGroup(resourceGroupName, path);

    // recreating the entity
	createEntity(m_name, filename);

    Log::info(QString("Mesh file \"%1\" loaded.").arg(filename), "EntityNode::loadMesh");
    return true;
}



///
/// Private Slots
///

//!
//! Change function for the Geometry File parameter.
//!
void EntityNode::geometryFileChanged ()
{
    // load new mesh and skeleton
    if (loadMesh())
		processScene();
}

//!
//! Processes the node's input data to generate the node's output table.
//!
void EntityNode::processScene()
{
	if (m_entity)
	{
		m_outputGeometryParameter->setEntity(m_entity);
		m_outputGeometryParameter->propagateDirty(0);
	}
}

// private functions

//!
//! Removes the OGRE entity containing the mesh geometry from the scene and
//! destroys it along with the OGRE scene node.
//!
void EntityNode::destroyEntity ()
{
    if (m_entity) {
        // delete entity container
        if (m_entityContainer) {
            delete m_entityContainer;
            m_entityContainer = 0;
        }

        // remove the entity from the scene node it is attached to
        Ogre::SceneNode *parentSceneNode = m_entity->getParentSceneNode();
        if (parentSceneNode)
            parentSceneNode->detachObject(m_entity);

        // destroy the entity through its scene manager
        Ogre::SceneManager *sceneManager = OgreManager::getSceneManager();
        if (sceneManager) {
            sceneManager->destroyEntity(m_entity);
            m_entity = 0;
        }
    }
}


//!
//! Create new scene.
//! \return True if the scene was successfully created, otherwise False.
//!
bool EntityNode::createEntity(QString name, QString fileName)
{
    // destroy the entity through its scene manager
    Ogre::SceneManager *sceneManager = OgreManager::getSceneManager();
    // create a new OGRE entity for each vertex
	m_entity = sceneManager->createEntity(fileName.toStdString(), fileName.toStdString());

	// create a container for the entity
    m_entityContainer = new OgreContainer(m_entity);
    m_entity->setUserAny(Ogre::Any(m_entityContainer));

	return true;
}
