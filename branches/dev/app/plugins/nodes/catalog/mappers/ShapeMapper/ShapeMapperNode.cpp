/*
-----------------------------------------------------------------------------
file "ShapeMapperNode.cpp"
brief Implementation file for ShapeMapperNode class.

version    1.0
date       18.05.2009 (last updated)

Description
-----------


Parameters
----------
      
Returns
-------

    
Notes
-----

Reference
---------

-----------------------------------------------------------------------------
*/


#include "ShapeMapperNode.h"
#include "EntityParameter.h"
#include "ParameterGroup.h"
#include "vtkUnsignedIntArray.h"
#include "OgreSceneManager.h"
#include "OgreManager.h"

INIT_INSTANCE_COUNTER(ShapeMapperNode)

///
/// Constructors and Destructors
///

//!
//! Constructor of the ShapeMapperNode class.
//!
//! \param name The name for the new node.
//! \param parameterRoot A copy of the parameter tree specific for the type of the node.
//!
ShapeMapperNode::ShapeMapperNode ( const QString &name, ParameterGroup *parameterRoot ) :
    Node(name, parameterRoot),
	m_outputShapeMapParameterName("ShapeMapOutput"),
	m_inputShapeMapParameterName("InputShapeMap"),
	m_inputVTKTableName("InputVTKTable"),
	m_entity(0)
{
    // create the ShapeMap input parameter - multiplicity ONE 
	inputShapeMapParameter = new ShapeMapParameter(m_inputShapeMapParameterName);
    inputShapeMapParameter->setPinType(Parameter::PT_Input);
    inputShapeMapParameter->setSelfEvaluating(true);
    parameterRoot->addParameter(inputShapeMapParameter);

	// create the vtk table data input parameter 
	inputVTKTableParameter = new VTKTableParameter(m_inputVTKTableName);
    inputVTKTableParameter->setPinType(Parameter::PT_Input);
    parameterRoot->addParameter(inputVTKTableParameter);

	// create the geometry input parameter (shape mapper)
	inputEntityParameter = new EntityParameter(m_inputEntityParameterName);
	inputEntityParameter->setMultiplicity(1);
	inputEntityParameter->setPinType(Parameter::PT_Input);
	inputEntityParameter->setSelfEvaluating(true);
	parameterRoot->addParameter(inputEntityParameter);
    connect(inputEntityParameter, SIGNAL(dirtied()), SLOT(processShapeMap()));
	
	// create the enumeration parameter representing the ID Field of table
	shapeFieldParameter = new EnumerationParameter("SetShapeField", Parameter::getDefaultValue(Parameter::T_Enumeration));
	parameterRoot->addParameter(shapeFieldParameter);
	connect(shapeFieldParameter, SIGNAL(dirtied()), this, SLOT(processShapeMap()));

    // create the ShapeMap output parameter
	outputShapeMapParameter = new ShapeMapParameter(m_outputShapeMapParameterName);
    outputShapeMapParameter->setPinType(Parameter::PT_Output);
    outputShapeMapParameter->setSelfEvaluating(true);
    parameterRoot->addParameter(outputShapeMapParameter);

	connect(inputVTKTableParameter, SIGNAL(dirtied()), SLOT(processShapeMap()));
    connect(inputEntityParameter, SIGNAL(dirtied()), SLOT(processShapeMap()));
    connect(shapeFieldParameter, SIGNAL(dirtied()), SLOT(processShapeMap()));
    connect(inputShapeMapParameter, SIGNAL(dirtied()), SLOT(processShapeMap()));

	INC_INSTANCE_COUNTER
}

//!
//! Destructor of the ShapeMapperNode class.
//!
//! Defined virtual to guarantee that the destructor of a derived class
//! will be called if the instance of the derived class is saved in a
//! variable of its parent class type.
//!
ShapeMapperNode::~ShapeMapperNode ()
{
	emit destroyed();
	Log::info(QString("ShapeMapperNode destroyed."), "ShapeMapperNode::~ShapeMapperNode");
	DEC_INSTANCE_COUNTER
}

void ShapeMapperNode::processShapeMap()
{
	if (inputShapeMapParameter && inputShapeMapParameter->isDirty())
	{
		// load the source shape map parameter 
		if (!inputShapeMapParameter->isConnected())
			return;

		// get the source parameter (output of source node) connected to the output parameter
		ShapeMapParameter * sourceParameter = dynamic_cast<ShapeMapParameter*>(inputShapeMapParameter->getConnectedParameter());

		// get the vtk table that comes with the source parameter and set it into the output parameter of this node
		outputShapeMapParameter->setVTKTable(sourceParameter->getVTKTable());
		outputShapeMapParameter->setShapeType(sourceParameter->getShapeType());

		outputShapeMapParameter->propagateDirty(0);
		return;
	}

	if (inputEntityParameter && inputVTKTableParameter && 
		(inputEntityParameter->isDirty() || inputVTKTableParameter->isDirty()))
	{
		m_entity = inputEntityParameter->getEntity();

		Ogre::SceneManager *sceneManager = OgreManager::getSceneManager();
		Ogre::String idPrefix = m_entity->getName() + QString(":").toStdString();

		// the mesh pointers array holding the pointers to each mesh
		// this will be replaced by a mesh typed array, when it will be possible to do so with vtkTable,
		// since for the moment only accepts vtkVariantArray as columns
		vtkUnsignedIntArray * meshPts = vtkUnsignedIntArray::New();
		meshPts->SetName("meshes");

		// vtkTable holding the shape IDs and their relative mesh pointer
		vtkTable * table = inputVTKTableParameter->getVTKTable();

		if (table)
		{
			table->AddColumn(meshPts);
			int rows = table->GetNumberOfRows();
			for (vtkIdType id = 0; id<rows; id++)
			{
				Ogre::String shapeID(idPrefix + Ogre::StringConverter::toString(id));

				// create new entity for each item
				Ogre::Entity *entityItem = m_entity->clone(shapeID);

				// store the mesh pointer into the vtkUnsignedIntArray column (which holds mesh pointers)
				unsigned int entityPtr = (unsigned int) & entityItem;
				meshPts->InsertNextValue(entityPtr);
			}
		}

		outputShapeMapParameter->setVTKTable(table);
		outputShapeMapParameter->propagateDirty(0);
	}
}