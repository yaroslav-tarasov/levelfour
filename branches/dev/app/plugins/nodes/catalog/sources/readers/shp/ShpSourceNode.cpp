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
//! \file "ShpSourceNode.cpp"
//! \brief Implementation file for ShpSourceNode class.
//!
//! \author     Stefan Habel <stefan.habel@filmakademie.de>
//! \version    1.0
//! \date       18.05.2009 (last updated)
//!

#include "ShpSourceNode.h"
#include "vtkShapefileReader.h"
#include <QFile>
#include "vtkCellArray.h";
#include "vtkStringArray.h"
#include "vtkUnsignedIntArray.h"
#include "vtkPolygon.h"
#include "OgreRenderOperation.h"
#include "OgreSceneManager.h"
#include "OgreManager.h"
#include "OgreEntity.h"
#include "OgreSceneNode.h"

///
/// Constructors and Destructors
///

INIT_INSTANCE_COUNTER(ShpSourceNode)


//!
//! Constructor of the ShpSourceNode class.
//!
//! \param name The name for the new node.
//! \param parameterRoot A copy of the parameter tree specific for the type of the node.
//!
ShpSourceNode::ShpSourceNode ( const QString &name, ParameterGroup *parameterRoot ) :
	VTKTableNode(name, parameterRoot, "VTKTable"),
	m_outputShapeMapParameterName("ShapeMapOutput")
{
    // create the ShapeMap output parameter - multiplicity ONE OR MORE
	outputShapeMapParameter = new ShapeMapParameter(m_outputShapeMapParameterName);
    outputShapeMapParameter->setPinType(Parameter::PT_Output);
    outputShapeMapParameter->setSelfEvaluating(true);
	outputShapeMapParameter->setShapeType(ShapeMapParameter::ShapeType::GEO);
    parameterRoot->addParameter(outputShapeMapParameter);

	// set affections and functions
    addAffection("Shapefile", m_outputShapeMapParameterName);
    setChangeFunction("Shapefile", SLOT(shapeFileChanged()));
    setCommandFunction("Shapefile", SLOT(shapeFileChanged()));
}


//!
//! Destructor of the ShpSourceNode class.
//!
//! Defined virtual to guarantee that the destructor of a derived class
//! will be called if the instance of the derived class is saved in a
//! variable of its parent class type.
//!
ShpSourceNode::~ShpSourceNode ()
{
	emit destroyed();
	Log::info(QString("ShpSourceNode destroyed."), "ShpSourceNode::~ShpSourceNode");

	cleanTable();
    DEC_INSTANCE_COUNTER
}

void ShpSourceNode::shapeFileChanged()
{
	QString filename = getStringValue("Shapefile");
    if (filename == "") {
        Log::debug(QString("Shapefile has not been set yet. (\"%1\")").arg(m_name), "ShpSourceNode::shapeFileChanged");
        return;
    }

	// check if the file exists
    if (!QFile::exists(filename)) {
        Log::error(QString("Shapefile file \"%1\" not found.").arg(filename), "ShpSourceNode::shapeFileChanged");
        return;
    }

    if (!filename.endsWith(".shp")) {
        Log::error("Shapefile file has to have an .shp extension.", "ShpSourceNode::shapeFileChanged");
        return;
    }

	vtkShapefileReader * shpSource = vtkShapefileReader::New();
	shpSource->SetFileName(filename.toLatin1());

	shpSource->Update();

	int nrecs = shpSource->GetNumberOfRecords();
	vtkPolyData * polydata = shpSource->GetOutput();

	// if non-empty clean the current table and destroy its meshes pointers
	if (outputShapeMapParameter->getVTKTable())
	{
		m_table->Delete();
		m_table = 0;
		outputShapeMapParameter->setVTKTable(m_table);
//		cleanTable();
	}

	// outputs the new mesh table
	m_table = polydataToMesh(polydata, shpSource->GetType());
	outputShapeMapParameter->setVTKTable(m_table);

	VTKTableParameter * outputVTKTableParameter = dynamic_cast<VTKTableParameter*>(getParameter(m_outputVTKTableName));
	outputVTKTableParameter->setVTKTable(m_table);
}

vtkTable * ShpSourceNode::polydataToMesh(vtkPolyData * polydata, int type)
{
	// the array holding the ID of each mesh
	vtkIdTypeArray * id = vtkIdTypeArray::New();
	id->SetName("id");

	// the array holding the X centroids for each mesh
	vtkDoubleArray * xCentroids = vtkDoubleArray::New();
	xCentroids->SetName("x_centroid");

	// the array holding the Y centroids for each mesh
	vtkDoubleArray * yCentroids = vtkDoubleArray::New();
	yCentroids->SetName("y_centroid");

	// the array holding the Z centroids for each mesh
	vtkDoubleArray * zCentroids = vtkDoubleArray::New();
	zCentroids->SetName("z_centroid");

	// the mesh names 
	vtkStringArray * meshNames = vtkStringArray::New();
	meshNames->SetName("mesh_names");

	// vtkTable holding the shape IDs and their relative mesh pointer
	vtkTable * table = vtkTable::New();

	table->AddColumn(id);
	table->AddColumn(meshNames);
	table->AddColumn(xCentroids);
	table->AddColumn(yCentroids);
	table->AddColumn(zCentroids);

	// create a mesh for each cell
	vtkCellArray * primArray;
	Ogre::RenderOperation::OperationType rendertype;
	switch (type)
	{
	case 1: // point
//		primArray = polydata->GetPoints();
		rendertype = Ogre::RenderOperation::OT_POINT_LIST;
		break;
	case 3: // polyline
	case 5: 
		primArray = polydata->GetLines();
		rendertype = Ogre::RenderOperation::OT_LINE_LIST;
		break;
	case 10000: // polygon
		primArray = polydata->GetLines();
		rendertype = Ogre::RenderOperation::OT_LINE_STRIP;
		break;
	case 20000: // strips
		break;
	}

	// get number of indices in the vtk prim array. Each vtkCell has the length
	// (not counted), followed by the indices.
	int numPrimitives = primArray->GetNumberOfCells();
	if (numPrimitives == 0) 
		return 0;

	int primArraySize = primArray->GetNumberOfConnectivityEntries();
	int numIndices = primArraySize - numPrimitives;

	// copy data from vtk prim array to osg Geometry
	unsigned int prim = 0, vert = 0;
	int i, npts, totpts = 0, *pts, transparentFlag = 0;;

    Ogre::SceneManager *sceneManager = OgreManager::getSceneManager();

	// create a ManualObject for each vtkCell
	QString filename = getStringValue("Shapefile");
	Ogre::String meshName;
	Ogre::MeshPtr mesh;
	// go through cells (primitives) and create a mesh for each cell
	for (primArray->InitTraversal(); primArray->GetNextCell(npts, pts); prim++)
	{ 
		// prepare the name for each manual object
		meshName = m_name.toStdString() + ":" + filename.toStdString() + ":" + Ogre::StringConverter::toString(prim);
		Ogre::ManualObject * manual;
		if (sceneManager->hasManualObject(meshName))
			manual = sceneManager->getManualObject(meshName);
		else
			manual = sceneManager->createManualObject(meshName);
		manual->begin("BaseWhiteNoLighting", rendertype);

		// prepare points arrays to calculate the centroid of the cell
		vtkIdTypeArray * pointsIDs = vtkIdTypeArray::New();
		vtkPoints * points = vtkPoints::New();

		// go through points in cell (verts)
		for (i=0; i < npts; i++)
		{
			double * vertex = polydata->GetPoint(pts[i]);
			double x = vertex[0], y = vertex[1], z = vertex[2];
			manual->position(x, y, z);
			points->InsertNextPoint(vertex);
			pointsIDs->InsertNextValue(i);
		}
		manual->end();

		// convert the manual object into a mesh (for multiple instantiation)
		manual->convertToMesh(meshName);

		// store the mesh name 
		meshNames->InsertNextValue(meshName);

		// store id
		id->InsertNextValue(prim);

		// calculate and store centroid
		double polyCentroid[3];
		vtkPolygon::ComputeCentroid(pointsIDs, points, polyCentroid);
		xCentroids->InsertNextValue(polyCentroid[0]);
		yCentroids->InsertNextValue(polyCentroid[1]);
		zCentroids->InsertNextValue(polyCentroid[2]);
		pointsIDs->Delete();
		points->Delete();
	}

	id->Delete();
	meshNames->Delete();
	zCentroids->Delete();
	xCentroids->Delete();
	yCentroids->Delete();
	return table;
}

void ShpSourceNode::cleanTable()
{
	vtkUnsignedIntArray * meshArray = vtkUnsignedIntArray::SafeDownCast(m_table->GetColumnByName("mesh_pointers"));
	vtkUnsignedIntArray * entityArray = vtkUnsignedIntArray::SafeDownCast(m_table->GetColumnByName("entity_pointers"));
	
    Ogre::SceneManager *sceneManager = OgreManager::getSceneManager();

	int rows = m_table->GetNumberOfRows();
	for (vtkIdType id = 0; id<rows; id++)
	{
		// retreive the meshes from the vtkUnsignedIntArray pointers
		Ogre::Mesh * mesh = reinterpret_cast<Ogre::Mesh*>(meshArray->GetValue(id));
		Ogre::String meshName = mesh->getName();

		// if the mesh is already registered than destroy it
		if (sceneManager->hasManualObject(meshName))
			sceneManager->destroyManualObject(meshName);
		delete mesh;
	}
	m_table->Delete();
	m_table = 0;
	outputShapeMapParameter->setVTKTable(m_table);
}