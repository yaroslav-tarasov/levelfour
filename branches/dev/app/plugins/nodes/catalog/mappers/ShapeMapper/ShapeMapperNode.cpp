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
#include "vtkStringArray.h"
#include "vtkDoubleArray.h"
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
	inputShapeMapParameter->setMultiplicity(1);
    inputShapeMapParameter->setPinType(Parameter::PT_Input);
    inputShapeMapParameter->setSelfEvaluating(true);
    parameterRoot->addParameter(inputShapeMapParameter);

	// create the vtk table data input parameter 
	inputVTKTableParameter = new VTKTableParameter(m_inputVTKTableName);
	inputVTKTableParameter->setMultiplicity(1);
    inputVTKTableParameter->setPinType(Parameter::PT_Input);
    inputVTKTableParameter->setSelfEvaluating(true);
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
	if (inputEntityParameter && inputShapeMapParameter && 
		inputEntityParameter->isConnected() && inputShapeMapParameter->isConnected())
	{
		// get the source parameter (output of source node) connected to the output parameter
		ShapeMapParameter * sourceShapeParameter = dynamic_cast<ShapeMapParameter*>(inputShapeMapParameter->getConnectedParameter());

		// get the source parameter (output of source node) connected to the output parameter
		EntityParameter * sourceEntityParameter = dynamic_cast<EntityParameter*>(inputEntityParameter->getConnectedParameter());

		m_entity = sourceEntityParameter->getEntity();
		if (!m_entity)
			return;

		Ogre::String meshName = m_entity->getName();

		// the mesh pointers array holding the names for each mesh
		vtkStringArray * meshNames = vtkStringArray::New();
		meshNames->SetName("mesh_names");

		// vtkTable holding the shape IDs and their relative mesh pointer
		vtkTable * sourceTable = sourceShapeParameter->getVTKTable();
		vtkTable * outputTable = vtkTable::New();

		// if source shapes derived from geo shapes, than they probably have a centroid
		bool useCentroids = useCentroids = sourceShapeParameter->hasCentroids();

		// the centroids arrays
		vtkDoubleArray * xCentroids = vtkDoubleArray::New();
		xCentroids->SetName("x_centroid");
		vtkDoubleArray * yCentroids = vtkDoubleArray::New();
		yCentroids->SetName("y_centroid");
		vtkDoubleArray * zCentroids = vtkDoubleArray::New();
		zCentroids->SetName("z_centroid");

		if (sourceTable)
		{
			int rows = sourceTable->GetNumberOfRows();
			// store the mesh name 
			for (vtkIdType id = 0; id<rows; id++)
			{
				meshNames->InsertNextValue(meshName);
				if (useCentroids)
				{
					double * centroid = sourceShapeParameter->getCentroid(id);
					double x = centroid[0], y = centroid[1], z = centroid[2];
					xCentroids->InsertNextValue(x);
					yCentroids->InsertNextValue(y);
					zCentroids->InsertNextValue(z);
				}
			}

			outputTable->AddColumn(meshNames);
			if (useCentroids)
			{
				outputTable->AddColumn(xCentroids);
				outputTable->AddColumn(yCentroids);
				outputTable->AddColumn(zCentroids);
			}
			meshNames->Delete();
			xCentroids->Delete();
			yCentroids->Delete();
			zCentroids->Delete();
		}
		outputShapeMapParameter->setHasCentroids(useCentroids);
		outputShapeMapParameter->setVTKTable(outputTable);
		outputShapeMapParameter->setShapeType(ShapeMapParameter::ShapeType::PRIMITIVE);
		outputShapeMapParameter->propagateDirty(0);
		return;
	}

	if (inputShapeMapParameter && inputShapeMapParameter->isConnected())
	{
		// get the source parameter (output of source node) connected to the output parameter
		ShapeMapParameter * sourceParameter = dynamic_cast<ShapeMapParameter*>(inputShapeMapParameter->getConnectedParameter());

		// get the vtk table that comes with the source parameter and set it into the output parameter of this node
		outputShapeMapParameter->copyShapeParameter(sourceParameter);

		outputShapeMapParameter->propagateDirty(0);
		return;
	}

	if (inputEntityParameter && inputVTKTableParameter && 
		inputEntityParameter->isConnected() && inputVTKTableParameter->isConnected())
	{
		// get the source parameter (output of source node) connected to the output parameter
		EntityParameter * sourceEntityParameter = dynamic_cast<EntityParameter*>(inputEntityParameter->getConnectedParameter());

		m_entity = sourceEntityParameter->getEntity();
		Ogre::String meshName = m_entity->getName();

		// the mesh pointers array holding the names for each mesh
		vtkStringArray * meshNames = vtkStringArray::New();
		meshNames->SetName("mesh_names");

		// get the source parameter (output of source node) connected to the inputVTKTableParameter
		VTKTableParameter * sourceTableParameter = dynamic_cast<VTKTableParameter*>(inputVTKTableParameter->getConnectedParameter());

		// vtkTable holding the shape IDs and their relative mesh pointer
		vtkTable * sourceTable = sourceTableParameter->getVTKTable();
		vtkTable * outputTable = vtkTable::New();

		if (sourceTable)
		{
			int rows = sourceTable->GetNumberOfRows();
			// store the mesh name 
			for (vtkIdType id = 0; id<rows; id++)
				meshNames->InsertNextValue(meshName);

			outputTable->AddColumn(meshNames);
			meshNames->Delete();
		}

		outputShapeMapParameter->setVTKTable(outputTable);
		outputShapeMapParameter->setShapeType(ShapeMapParameter::ShapeType::PRIMITIVE);
		outputShapeMapParameter->propagateDirty(0);
	}
}