
#include "Model2SceneNode.h"
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
#include "vtkStringArray.h"

INIT_INSTANCE_COUNTER(Model2SceneNode)


///
/// Constructors and Destructors
///


//!
//! Constructor of the Model2SceneNode class.
//!
//! \param name The name to give the new mesh node.
//! \param parameterRoot A copy of the parameter tree specific for the type of the node.
//!
Model2SceneNode::Model2SceneNode ( const QString &name, ParameterGroup *parameterRoot ) :
    GeometryNode(name, parameterRoot, "SceneNode"),
    m_sceneNode(0),
    m_entity(0), 
    m_entityContainer(0),
    m_oldResourceGroupName("")
{
    // create the mandatory vtk table input parameter - multiplicity ONE OR MORE
	VTKTableParameter * inputVTKTableParameter = new VTKTableParameter(m_inputVTKTableParameterName);
	inputVTKTableParameter->setMultiplicity(1);
    inputVTKTableParameter->setPinType(Parameter::PT_Input);
    inputVTKTableParameter->setSelfEvaluating(true);
    parameterRoot->addParameter(inputVTKTableParameter);
    connect(inputVTKTableParameter, SIGNAL(dirtied()), SLOT(processScene()));

	// set affections and functions
    addAffection("Geometry File", m_outputGeometryName);
    setChangeFunction("Geometry File", SLOT(geometryFileChanged()));
    setCommandFunction("Geometry File", SLOT(geometryFileChanged()));
    connect(this, SIGNAL(frameChanged(int)), SLOT(updateAll()));

    INC_INSTANCE_COUNTER
}


//!
//! Destructor of the Model2SceneNode class.
//!
Model2SceneNode::~Model2SceneNode ()
{
    destroyEntity();
    OgreTools::destroyResourceGroup(m_oldResourceGroupName);
    emit viewNodeUpdated();

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
bool Model2SceneNode::loadMesh ()
{
    QString filename = getStringValue("Geometry File");
    if (filename == "") {
        Log::debug(QString("Geometry file has not been set yet. (\"%1\")").arg(m_name), "Model2SceneNode::loadMesh");
        return false;
    }

    // obtain the OGRE scene manager
    Ogre::SceneManager *sceneManager = OgreManager::getSceneManager();
    if (!sceneManager) {
        Log::error("Could not obtain OGRE scene manager.", "Model2SceneNode::loadMesh");
        return false;
    }

    // destroy an existing OGRE entity for the mesh
    destroyEntity();

    // create new scene node
    m_sceneNode = OgreManager::createSceneNode(m_name);
    if (!m_sceneNode) {
        Log::error(QString("Scene node for node \"%1\" could not be created.").arg(m_name), "Model2SceneNode::loadMesh");
        return false;
    }
    setValue(m_outputGeometryName, m_sceneNode, true);

    // check if the file exists
    if (!QFile::exists(filename)) {
        Log::error(QString("Mesh file \"%1\" not found.").arg(filename), "Model2SceneNode::loadMesh");
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
        Log::error("The geometry file has to be an OGRE mesh file.", "Model2SceneNode::loadMesh");
        return false;
    }

    // destroy old resource group and generate new one
    QString resourceGroupName = QString::fromStdString(createUniqueName("ResourceGroup_" + filename + "_Model2SceneNode"));
    OgreTools::destroyResourceGroup(m_oldResourceGroupName);
    m_oldResourceGroupName = resourceGroupName;
    OgreTools::createResourceGroup(resourceGroupName, path);

    // create a new OGRE entity for each vertex
    m_entity = sceneManager->createEntity(m_name.toStdString(), filename.toStdString());
    if (m_entity) {
        // set cumulative blend mode instead of Ogre::ANIMBLEND_AVERAGE which is default
        if (m_entity->hasSkeleton()) {
            Ogre::Skeleton *skeleton = m_entity->getSkeleton();
            skeleton->setBlendMode(Ogre::ANIMBLEND_CUMULATIVE);
        }
        m_sceneNode->attachObject(m_entity);
    }

    // create a container for the entity
    m_entityContainer = new OgreContainer(m_entity);
    m_entity->setUserAny(Ogre::Any(m_entityContainer));

    Log::info(QString("Mesh file \"%1\" loaded.").arg(filename), "Model2SceneNode::loadMesh");
    return true;
}



//!
//! Removes the OGRE entity containing the mesh geometry from the scene and
//! destroys it along with the OGRE scene node.
//!
void Model2SceneNode::destroyEntity ()
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

    if (m_sceneNode) {
        // destroy the scene node through its scene manager
        Ogre::SceneManager *sceneManager = m_sceneNode->getCreator();
        if (sceneManager) {
            sceneManager->destroySceneNode(m_sceneNode);
            m_sceneNode = 0;
            setValue(m_outputGeometryName, m_sceneNode, true);
        }
    }
}



///
/// Private Slots
///

//!
//! Change function for the Geometry File parameter.
//!
void Model2SceneNode::geometryFileChanged ()
{
    // load new mesh and skeleton
    loadMesh();
}

//!
//! Processes the node's input data to generate the node's output table.
//!
void Model2SceneNode::processScene()
{
	// load the input vtk parameter 
	VTKTableParameter * inputParameter = dynamic_cast<VTKTableParameter*>(getParameter(m_inputVTKTableParameterName));
	if (!inputParameter->isConnected())
		return;

	// get the source parameter (output of source node) connected to the input parameter
	VTKTableParameter * sourceParameter = dynamic_cast<VTKTableParameter*>(inputParameter->getConnectedParameter());

	inputParameter->setVTKTable(sourceParameter->getVTKTable());

	vtkTable * xyzTable = inputParameter->getVTKTable();

	//Get columns named "NodeID", "X", "Y" and "Z"
	vtkStringArray *colNodeId = dynamic_cast<vtkStringArray*>(xyzTable->GetColumnByName("NodeId"));
	vtkDoubleArray *colX = dynamic_cast<vtkDoubleArray*>(xyzTable->GetColumnByName("X"));
	vtkDoubleArray *colY = dynamic_cast<vtkDoubleArray*>(xyzTable->GetColumnByName("Y"));
	vtkDoubleArray *colZ = dynamic_cast<vtkDoubleArray*>(xyzTable->GetColumnByName("Z"));

	if (!m_entity)
		return;

	if (m_sceneNode)
		m_sceneNode->removeAndDestroyAllChildren();

	for (int i=0; i<xyzTable->GetNumberOfRows(); i++)
	{
		Ogre::String nodeID(colNodeId->GetValue(i));
		// create new entity for each item
		Ogre::Entity *entityItem = m_entity->clone(nodeID);
		// create new scene node for each item
		Ogre::SceneNode *sceneItem = OgreManager::createSceneNode(QString(colNodeId->GetValue(i)));
		sceneItem->attachObject(entityItem);
		double x = colX->GetValue(i);
		double y = colY->GetValue(i);
		double z = colZ->GetValue(i);
		sceneItem->setPosition(Ogre::Real(x) * 10, Ogre::Real(y) * 10, Ogre::Real(z) * 10);
		m_sceneNode->addChild(sceneItem);
		
	}
}