
#include "PositionMapperNode.h"
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
#include "EntityParameter.h"

INIT_INSTANCE_COUNTER(PositionMapperNode)


///
/// Constructors and Destructors
///


//!
//! Constructor of the PositionMapperNode class.
//!
//! \param name The name to give the new mesh node.
//! \param parameterRoot A copy of the parameter tree specific for the type of the node.
//!
PositionMapperNode::PositionMapperNode ( const QString &name, ParameterGroup *parameterRoot ) :
    GeometryNode(name, parameterRoot, "SceneNode"),
	m_sceneNode(0),
	m_entity(0), 
    m_entityContainer(0),
	m_oldResourceGroupName(""),
	m_inputVTKTableParameterName("VTKTableInput"),
	m_inputShapeMapParameterName("InputShapeMap"),
	m_inputGeometryParameterName("Geometry")
{
    // create the vtk table input parameter - multiplicity ONE OR MORE
	VTKTableParameter * inputVTKTableParameter = new VTKTableParameter(m_inputVTKTableParameterName);
	inputVTKTableParameter->setMultiplicity(1);
    inputVTKTableParameter->setPinType(Parameter::PT_Input);
    inputVTKTableParameter->setSelfEvaluating(true);
    parameterRoot->addParameter(inputVTKTableParameter);
    connect(inputVTKTableParameter, SIGNAL(dirtied()), SLOT(processParameters()));

    // create the ShapeMap input parameter - multiplicity ONE 
	inputShapeMapParameter = new ShapeMapParameter(m_inputShapeMapParameterName);
    inputShapeMapParameter->setPinType(Parameter::PT_Input);
    inputShapeMapParameter->setSelfEvaluating(true);
    parameterRoot->addParameter(inputShapeMapParameter);
    connect(inputShapeMapParameter, SIGNAL(dirtied()), SLOT(processScene()));

	// create the geometry input parameter (shape mapper)
	EntityParameter *inputGeometryParameter = new EntityParameter(m_inputGeometryParameterName);
	inputGeometryParameter->setMultiplicity(1);
	inputGeometryParameter->setPinType(Parameter::PT_Input);
	inputGeometryParameter->setSelfEvaluating(true);
	parameterRoot->addParameter(inputGeometryParameter);
    connect(inputGeometryParameter, SIGNAL(dirtied()), SLOT(setGeometry()));
	
	// set affections and functions
    addAffection("Geometry File", m_outputGeometryName);
    setChangeFunction("Geometry File", SLOT(geometryFileChanged()));
    setCommandFunction("Geometry File", SLOT(geometryFileChanged()));
    connect(this, SIGNAL(frameChanged(int)), SLOT(updateAll()));

	// create the enumeration parameter representing the ID Field of table
	idFieldParameter = new EnumerationParameter("SetIdField", Parameter::getDefaultValue(Parameter::T_Enumeration));
	parameterRoot->addParameter(idFieldParameter);
	connect(idFieldParameter, SIGNAL(dirtied()), this, SLOT(processScene()));
	
    // create the enumeration parameter representing the X Field of table
	xFieldParameter = new EnumerationParameter("SetXField", Parameter::getDefaultValue(Parameter::T_Enumeration));
    parameterRoot->addParameter(xFieldParameter);
	connect(xFieldParameter, SIGNAL(dirtied()), this, SLOT(processScene()));

    // create the enumeration parameter representing the Y Field of table
	yFieldParameter = new EnumerationParameter("SetYField", Parameter::getDefaultValue(Parameter::T_Enumeration));
    parameterRoot->addParameter(yFieldParameter);
	connect(yFieldParameter, SIGNAL(dirtied()), this, SLOT(processScene()));

	// create the enumeration parameter representing the Z Field of table
	zFieldParameter = new EnumerationParameter("SetZField", Parameter::getDefaultValue(Parameter::T_Enumeration));
    parameterRoot->addParameter(zFieldParameter);
	connect(zFieldParameter, SIGNAL(dirtied()), this, SLOT(processScene()));

	createSceneNode();

	INC_INSTANCE_COUNTER
}


//!
//! Destructor of the PositionMapperNode class.
//!
PositionMapperNode::~PositionMapperNode ()
{
    destroyEntity();
	destroyAllAttachedMovableObjects(m_sceneNode);
	OgreTools::destroyResourceGroup(m_oldResourceGroupName);

/*	if (m_sceneNode) {
        // destroy the scene node through its scene manager
        Ogre::SceneManager *sceneManager = m_sceneNode->getCreator();
        if (sceneManager) {
            sceneManager->destroySceneNode(m_sceneNode);
            m_sceneNode = 0;
            setValue(m_outputGeometryName, m_sceneNode);
        }
    }
*/
	emit destroyed();
	Log::info(QString("PositionMapperNode destroyed."), "PositionMapperNode::~PositionMapperNode");

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
bool PositionMapperNode::loadMesh ()
{
    // destroy an existing OGRE entity for the mesh
    destroyEntity();

	QString filename = getStringValue("Geometry File");
    if (filename == "") {
        Log::debug(QString("Geometry file has not been set yet. (\"%1\")").arg(m_name), "PositionMapperNode::loadMesh");
        return false;
    }

	// check if the file exists
    if (!QFile::exists(filename)) {
        Log::error(QString("Mesh file \"%1\" not found.").arg(filename), "PositionMapperNode::loadMesh");
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
        Log::error("The geometry file has to be an OGRE mesh file.", "PositionMapperNode::loadMesh");
        return false;
    }
	// destroy old resource group and generate new one
    QString resourceGroupName = QString::fromStdString(createUniqueName("ResourceGroup_" + filename + "_PositionMapperNode"));
    OgreTools::destroyResourceGroup(m_oldResourceGroupName);
    m_oldResourceGroupName = resourceGroupName;
    OgreTools::createResourceGroup(resourceGroupName, path);

    // recreating the entity
	createEntity(m_name, filename);

    Log::info(QString("Mesh file \"%1\" loaded.").arg(filename), "PositionMapperNode::loadMesh");
    return true;
}



///
/// Private Slots
///

//!
//! Change function for the Geometry File parameter.
//!
void PositionMapperNode::geometryFileChanged ()
{
    // load new mesh and skeleton
    if (loadMesh())
		processScene();
}

//!
//! Processes the node's input data to generate the node's output table.
//!
void PositionMapperNode::processParameters()
{
	if (!updateTable())
		return;

	int numColumns = m_inputTable->GetNumberOfColumns();
	// Get field assignments for id, x, y. z
	QStringList literals;
	QString name;
	for (int i = 0; i < numColumns; i++)
	{
		name = QString(m_inputTable->GetColumnName(i));
		literals << name;
	}
	idFieldParameter->setLiterals(literals);
	idFieldParameter->setValues(literals);

	xFieldParameter->setLiterals(literals);
	xFieldParameter->setValues(literals);

	yFieldParameter->setLiterals(literals);
	yFieldParameter->setValues(literals);

	zFieldParameter->setLiterals(literals);
	zFieldParameter->setValues(literals);

	processScene();
}


//!
//! Processes the node's input data to generate the node's output table.
//!
void PositionMapperNode::processScene()
{
	Ogre::SceneManager *sceneManager = OgreManager::getSceneManager();
	Ogre::String idPrefix(QString(m_name + ":").toStdString());

//	destroyAllAttachedMovableObjects(m_sceneNode);
	destroyAllChildren(m_sceneNode);

	// load the source shape map parameter 
	if (!inputShapeMapParameter->isConnected())
		return;

	if (!m_sceneNode)
		return;

	vtkTable * shapeTable = 0;

	// get the source parameter (output of source node) connected to the input parameter
	ShapeMapParameter * sourceParameter = dynamic_cast<ShapeMapParameter*>(inputShapeMapParameter->getConnectedParameter());

	// get the vtk table that comes with the source parameter and set it into the input parameter of this node
	inputShapeMapParameter->copyShapeParameter(sourceParameter);

	// if the shape table is null than do nothing
	shapeTable = inputShapeMapParameter->getVTKTable();
	if (!shapeTable)
		return;

	// check if there is a valid layout to position items
	bool isLayoutedPositionable = false;

	// in case there is no valid position layout && items have a centroid && their shape is not GEO
	// than we can still position items in geo located centroids (geo symbols)
	bool isGeoPositionable = inputShapeMapParameter->hasCentroids() && 
							(inputShapeMapParameter->getShapeType() != ShapeMapParameter::GEO);

	// update position table
	isLayoutedPositionable = updateTable();

	QString setIdField, setXField, setYField, setZField;
	vtkIdTypeArray *colNodeId = 0;
	vtkDoubleArray *colX = 0, *colY = 0, *colZ = 0;
	if (isLayoutedPositionable)
	{
		setIdField = idFieldParameter->getCurrentValue();
		setXField = xFieldParameter->getCurrentValue();
		setYField = yFieldParameter->getCurrentValue();
		setZField = zFieldParameter->getCurrentValue();

		//Get columns specified from assignments
		colNodeId = dynamic_cast<vtkIdTypeArray*>(m_inputTable->GetColumnByName(setIdField.toLatin1()));
		colX = dynamic_cast<vtkDoubleArray*>(m_inputTable->GetColumnByName(setXField.toLatin1()));
		colY = dynamic_cast<vtkDoubleArray*>(m_inputTable->GetColumnByName(setYField.toLatin1()));
		colZ = dynamic_cast<vtkDoubleArray*>(m_inputTable->GetColumnByName(setZField.toLatin1()));
	}

	if (!colX || !colY || !colZ)
		isLayoutedPositionable = false;

	int nShapes = shapeTable->GetNumberOfRows();

	for (vtkIdType id=0; id<nShapes; id++)
	{
		Ogre::String nodeID(idPrefix + inputShapeMapParameter->getShapeName(id).toStdString() + ":" + Ogre::StringConverter::toString((int)id));

		// create new scene node for each item
		Ogre::SceneNode *sceneItem = sceneManager->createSceneNode(nodeID);
		// create new entity for each item
		Ogre::Entity *entityItem;
		if (sceneManager->hasEntity(nodeID))
			entityItem = sceneManager->getEntity(nodeID);
		else
			entityItem = sceneManager->createEntity(nodeID, inputShapeMapParameter->getShapeName(id).toStdString());

		// add item node to the scene
		sceneItem->attachObject(entityItem);
		m_sceneNode->addChild(sceneItem);

		if (isLayoutedPositionable)
		{
			// retrieve shape centroid and consider it as an offset to reposition the scene node properly
			double * centroid = inputShapeMapParameter->getCentroid(id);

			double x = colX->GetValue(id) - centroid[0];
			double y = colY->GetValue(id) - centroid[1];
			double z = colZ->GetValue(id) - centroid[2];
			sceneItem->setPosition(Ogre::Real(x), Ogre::Real(y), Ogre::Real(z));
		} else if (isGeoPositionable) {
			// in case there is no valid position layout && items have a centroid && their shape is not GEO
			// than we can still position items in geo located centroids (geo symbols)
			double * centroid = inputShapeMapParameter->getCentroid(id);
			sceneItem->setPosition(centroid[0], centroid[1], centroid[2]);
		}
		// if shape parameter is geo-typed than it can still create the scene without positioning items
	}
}

// private functions

//!
//! Removes the OGRE entity containing the mesh geometry from the scene and
//! destroys it along with the OGRE scene node.
//!
void PositionMapperNode::destroyEntity ()
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
bool PositionMapperNode::createEntity(QString name, QString fileName)
{
    // destroy the entity through its scene manager
    Ogre::SceneManager *sceneManager = OgreManager::getSceneManager();
    // create a new OGRE entity for each vertex
    m_entity = sceneManager->createEntity(name.toStdString(), fileName.toStdString());

	// create a container for the entity
    m_entityContainer = new OgreContainer(m_entity);
    m_entity->setUserAny(Ogre::Any(m_entityContainer));

	return true;
}

//!
//! Remove and destroy this scene.
//!
void PositionMapperNode::destroySceneNode()
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
bool PositionMapperNode::createSceneNode()
{
    // obtain the OGRE scene manager
    Ogre::SceneManager *sceneManager = OgreManager::getSceneManager();
    if (!sceneManager) {
        Log::error("Could not obtain OGRE scene manager.", "PositionMapperNode::loadMesh");
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
        Log::error(QString("Scene node for node \"%1\" could not be created.").arg(m_name), "PositionMapperNode::loadMesh");
        return false;
    }
    setValue(m_outputGeometryName, m_sceneNode, true);
	return m_sceneNode != 0;
}


//!
//! Remove and destroy all movable objects of this scene.
//! \param The scenenode to be destroyed
//!
void PositionMapperNode::destroyAllAttachedMovableObjects( Ogre::SceneNode* i_pSceneNode )
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
void PositionMapperNode::destroyAllChildren( Ogre::SceneNode* i_pSceneNode )
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
//      destroyAllAttachedMovableObjects( pChildNode );
	  // obtain the OGRE scene manager
	  Ogre::SceneManager *sceneManager = OgreManager::getSceneManager();
	  if (pChildNode != m_sceneNode)
		  sceneManager->destroySceneNode( pChildNode->getName() );
   }
}

//!
//! Update the input table 
//!
bool PositionMapperNode::updateTable ()
{
	// load the input vtk parameter 
	VTKTableParameter * inputParameter = dynamic_cast<VTKTableParameter*>(getParameter(m_inputVTKTableParameterName));
	if (!inputParameter->isConnected())
		return false;

	// get the source parameter (output of source node) connected to the input parameter
	VTKTableParameter * sourceParameter = dynamic_cast<VTKTableParameter*>(inputParameter->getConnectedParameter());

	// get the vtk table that comes with the source parameter and set it into the input parameter of this node
	m_inputTable = sourceParameter->getVTKTable();
	inputParameter->setVTKTable(m_inputTable);

	return (m_inputTable != 0);
}


//!
//! Update the input table 
//!
void PositionMapperNode::setGeometry()
{
	// load the input vtk parameter 
	EntityParameter * inputParameter = dynamic_cast<EntityParameter*>(getParameter(m_inputGeometryParameterName));
	if (!inputParameter->isConnected())
		return;

	// get the source parameter (output of source node) connected to the input parameter
	EntityParameter * sourceParameter = dynamic_cast<EntityParameter*>(inputParameter->getConnectedParameter());


	// get the vtk table that comes with the source parameter and set it into the input parameter of this node
	m_entity = sourceParameter->getEntity();

	processScene();
}
