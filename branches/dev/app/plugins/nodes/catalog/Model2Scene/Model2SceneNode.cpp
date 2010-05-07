
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
#include "vtkIdTypeArray.h"

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
    m_oldResourceGroupName(""),
	m_size(1.0,1.0,1.0)
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

	// set affections and functions
    setChangeFunction("Size", SLOT(sizeChanged()));
    setCommandFunction("Size", SLOT(sizeChanged()));

	createSceneNode();

	INC_INSTANCE_COUNTER
}


//!
//! Destructor of the Model2SceneNode class.
//!
Model2SceneNode::~Model2SceneNode ()
{
    destroyEntity();
	destroyAllAttachedMovableObjects(m_sceneNode);
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
    // destroy an existing OGRE entity for the mesh
    destroyEntity();

	QString filename = getStringValue("Geometry File");
    if (filename == "") {
        Log::debug(QString("Geometry file has not been set yet. (\"%1\")").arg(m_name), "Model2SceneNode::loadMesh");
        return false;
    }

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

    // recreating the entity
	createEntity(m_name, filename);

    Log::info(QString("Mesh file \"%1\" loaded.").arg(filename), "Model2SceneNode::loadMesh");
    return true;
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
    if (loadMesh())
		processScene();
}

//!
//! Change size scale of the entity.
//!
void Model2SceneNode::sizeChanged()
{
	m_size.x = m_size.y = m_size.z = getDoubleValue("Size");

	if (m_sceneNode)
		resizeNodes(m_sceneNode);
}

//!
//! Processes the node's input data to generate the node's output table.
//!
void Model2SceneNode::processScene()
{
	// load the input vtk parameter 
	VTKTableParameter * inputParameter = dynamic_cast<VTKTableParameter*>(getParameter(m_inputVTKTableParameterName));
	if (!inputParameter || !inputParameter->isConnected())
		return;

	// get the source parameter (output of source node) connected to the input parameter
	VTKTableParameter * sourceParameter = dynamic_cast<VTKTableParameter*>(inputParameter->getConnectedParameter());

	inputParameter->setVTKTable(sourceParameter->getVTKTable());

	vtkTable * xyzTable = inputParameter->getVTKTable();

	if (!m_sceneNode)
		createSceneNode();


	if (!m_entity || !xyzTable || !m_sceneNode)
		return;

	//Get columns named "NodeID", "X", "Y" and "Z"
	vtkIdTypeArray *colNodeId = dynamic_cast<vtkIdTypeArray*>(xyzTable->GetColumnByName("NodeId"));
	vtkDoubleArray *colX = dynamic_cast<vtkDoubleArray*>(xyzTable->GetColumnByName("X"));
	vtkDoubleArray *colY = dynamic_cast<vtkDoubleArray*>(xyzTable->GetColumnByName("Y"));
	vtkDoubleArray *colZ = dynamic_cast<vtkDoubleArray*>(xyzTable->GetColumnByName("Z"));

	destroyAllAttachedMovableObjects(m_sceneNode);
	destroyAllChildren(m_sceneNode);

	Ogre::String idPrefix(QString(m_name + ":").toStdString());

	Ogre::SceneManager *sceneManager = OgreManager::getSceneManager();

	for (int i=0; i<xyzTable->GetNumberOfRows(); i++)
	{
		int colIDValue = colNodeId->GetValue(i);
		Ogre::String nodeID(idPrefix + Ogre::StringConverter::toString(colIDValue));

		// create new scene node for each item
		Ogre::SceneNode *sceneItem = sceneManager->createSceneNode(nodeID);

		// create new entity for each item
		Ogre::Entity *entityItem = m_entity->clone(nodeID);
		
		sceneItem->attachObject(entityItem);

		double x = colX->GetValue(i);
		double y = colY->GetValue(i);
		double z = colZ->GetValue(i);
		sceneItem->setPosition(Ogre::Real(x), Ogre::Real(y), Ogre::Real(z));
		sceneItem->setScale(m_size);
		m_sceneNode->addChild(sceneItem);
		
	}
}

// private functions

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
}


//!
//! Create new scene.
//! \return True if the scene was successfully created, otherwise False.
//!
bool Model2SceneNode::createEntity(QString name, QString fileName)
{
    // destroy the entity through its scene manager
    Ogre::SceneManager *sceneManager = OgreManager::getSceneManager();
    // create a new OGRE entity for each vertex
    m_entity = sceneManager->createEntity(name.toStdString(), fileName.toStdString());
    if (m_entity) {
        // set cumulative blend mode instead of Ogre::ANIMBLEND_AVERAGE which is default
        if (m_entity->hasSkeleton()) {
            Ogre::Skeleton *skeleton = m_entity->getSkeleton();
            skeleton->setBlendMode(Ogre::ANIMBLEND_CUMULATIVE);
        }
    }

    // create a container for the entity
    m_entityContainer = new OgreContainer(m_entity);
    m_entity->setUserAny(Ogre::Any(m_entityContainer));

	return true;
}

//!
//! Remove and destroy this scene.
//!
void Model2SceneNode::destroySceneNode()
{
    if (m_sceneNode) {
        // destroy the scene node through its scene manager
        Ogre::SceneManager *sceneManager =OgreManager::getSceneManager();
        if (sceneManager) {
            sceneManager->destroySceneNode(m_sceneNode);
            m_sceneNode = 0;
            setValue(m_outputGeometryName, m_sceneNode, true);
        }
    }
}

//!
//! Create new scene.
//! \return True if the scene was successfully created, otherwise False.
//!
bool Model2SceneNode::createSceneNode()
{
    // obtain the OGRE scene manager
    Ogre::SceneManager *sceneManager = OgreManager::getSceneManager();
    if (!sceneManager) {
        Log::error("Could not obtain OGRE scene manager.", "Model2SceneNode::loadMesh");
        return false;
    }

	// create new scene node
    m_sceneNode = OgreManager::createSceneNode(m_name);

	// get the scene if it could not create it and if it already exists
    Ogre::String sceneNodeName = QString("%1SceneNode").arg(m_name).toStdString();  
	if (!m_sceneNode && sceneManager->hasSceneNode(sceneNodeName))
		m_sceneNode = sceneManager->getSceneNode(sceneNodeName);

	// alert in case it could not be either created or loaded
    if (!m_sceneNode) {
        Log::error(QString("Scene node for node \"%1\" could not be created.").arg(m_name), "Model2SceneNode::loadMesh");
        return false;
    }
    setValue(m_outputGeometryName, m_sceneNode, true);
	return m_sceneNode != 0;
}


//!
//! Remove and destroy all movable objects of this scene.
//! \param The scenenode to be destroyed
//!
void Model2SceneNode::destroyAllAttachedMovableObjects( Ogre::SceneNode* i_pSceneNode )
{
   if ( !i_pSceneNode )
      return;

   // Destroy all the attached objects
   Ogre::SceneNode::ObjectIterator itObject = i_pSceneNode->getAttachedObjectIterator();

   while ( itObject.hasMoreElements() )
   {
      Ogre::MovableObject* pObject = static_cast<Ogre::MovableObject*>(itObject.getNext());
      i_pSceneNode->getCreator()->destroyMovableObject( pObject );
   }

   // Recurse to child SceneNodes
   Ogre::SceneNode::ChildNodeIterator itChild = i_pSceneNode->getChildIterator();

   while ( itChild.hasMoreElements() )
   {
      Ogre::SceneNode* pChildNode = static_cast<Ogre::SceneNode*>(itChild.getNext());
      destroyAllAttachedMovableObjects( pChildNode );
   }
}

//!
//! Remove and destroy this scene children.
//! \param The scenenode to be destroyed
//!
void Model2SceneNode::destroyAllChildren( Ogre::SceneNode* i_pSceneNode )
{
	if ( !i_pSceneNode || i_pSceneNode->numChildren() == 0 )
      return;

   // Destroy all the attached objects
   Ogre::SceneNode::ObjectIterator itObject = i_pSceneNode->getAttachedObjectIterator();

   // Recurse to child SceneNodes
   Ogre::SceneNode::ChildNodeIterator itChild = i_pSceneNode->getChildIterator();

   while ( itChild.hasMoreElements() )
   {
      Ogre::SceneNode* pChildNode = static_cast<Ogre::SceneNode*>(itChild.getNext());
      destroyAllAttachedMovableObjects( pChildNode );
	  // obtain the OGRE scene manager
	  Ogre::SceneManager *sceneManager = OgreManager::getSceneManager();
	  if (pChildNode != m_sceneNode)
		  sceneManager->destroySceneNode( pChildNode->getName() );
   }
}

//!
//! Remove and destroy this scene children.
//! \param The scenenode to be destroyed
//!
void Model2SceneNode::resizeNodes( Ogre::SceneNode* i_pSceneNode )
{
	if ( !i_pSceneNode || i_pSceneNode->numChildren() == 0 )
      return;

   // Destroy all the attached objects
   Ogre::SceneNode::ObjectIterator itObject = i_pSceneNode->getAttachedObjectIterator();

   // Recurse to child SceneNodes
   Ogre::SceneNode::ChildNodeIterator itChild = i_pSceneNode->getChildIterator();

   while ( itChild.hasMoreElements() )
   {
      Ogre::SceneNode* pChildNode = static_cast<Ogre::SceneNode*>(itChild.getNext());

	  // obtain the OGRE scene manager
	  Ogre::SceneManager *sceneManager = OgreManager::getSceneManager();
	  if (pChildNode != m_sceneNode)
		  pChildNode->setScale(m_size);
   }
}