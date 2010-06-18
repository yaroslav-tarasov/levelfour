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
#include <QFile>

#include "vtkShapefileReader.h"
#include "vtkCellArray.h"
#include "vtkCellData.h"
#include "vtkStringArray.h"
#include "vtkUnsignedIntArray.h"
#include "vtkPolygon.h"
#include "vtkLinearExtrusionFilter.h"
#include "vtkTriangleFilter.h"
#include "vtkMath.h"
#include "vtkPolyDataNormals.h"
#include "vtkPointData.h"

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
	m_outputShapeMapParameterName("ShapeMapOutput"),
	m_polydata(0),
	m_ShapeType(0),
	m_extrude(false)
{
    // create the ShapeMap output parameter - multiplicity ONE OR MORE
	outputShapeMapParameter = new ShapeMapParameter(m_outputShapeMapParameterName);
    outputShapeMapParameter->setPinType(Parameter::PT_Output);
    outputShapeMapParameter->setSelfEvaluating(true);
	outputShapeMapParameter->setShapeType(ShapeMapParameter::ShapeType::GEO);
    parameterRoot->addParameter(outputShapeMapParameter);

	// set affections and functions
    setChangeFunction("Shapefile", SLOT(shapeFileChanged()));
    setCommandFunction("Shapefile", SLOT(shapeFileChanged()));

	setChangeFunction("Extrude", SLOT(processOutput()));
    setCommandFunction("Extrude", SLOT(processOutput()));
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
	Log::info(QString("ShpSourceNode destroyed."), "ShpSourceNode::~ShpSourceNode");

	if (m_polydata)
	{
		m_polydata->DeleteCells();
		m_polydata->DeleteLinks();
		if (m_polydata->GetPoints())
			m_polydata->GetPoints()->Delete();
		if (m_polydata->GetPolys())
			m_polydata->GetPolys()->Delete();
		if (m_polydata->GetStrips())
			m_polydata->GetStrips()->Delete();

		m_polydata = 0;
	}
	m_ShapeType = NULL;

	cleanTable();
	emit destroyed();
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
	shpSource->SetFillPolygons(1);

	shpSource->Update();

	int nrecs = shpSource->GetNumberOfRecords();

	m_polydata = shpSource->GetOutput();
	m_ShapeType = shpSource->GetType();

	processOutput();
}

void ShpSourceNode::processOutput()
{
	m_extrude = getBoolValue("Extrude");

	cleanTable();
	m_table = polydataToMesh(m_polydata, m_ShapeType);

	// if non-empty clean the current table and destroy its meshes pointers
	if (outputShapeMapParameter->getVTKTable() != m_table)
	{
		// outputs the new mesh table
		outputShapeMapParameter->setVTKTable(m_table);
		VTKTableParameter * outputVTKTableParameter = dynamic_cast<VTKTableParameter*>(getParameter(m_outputVTKTableName));
		outputVTKTableParameter->setVTKTable(m_table);

		outputShapeMapParameter->propagateDirty(this);
		outputVTKTableParameter->propagateDirty(this);
	}
}

vtkTable * ShpSourceNode::polydataToMesh(vtkPolyData * polydata, int type)
{
	if (!polydata)
		return 0;

	// get the records IDs of the shapefile 
	vtkUnsignedIntArray * recIDs = vtkUnsignedIntArray::SafeDownCast(polydata->GetCellData()->GetScalars("rec_id"));
	
	// the array holding the unique ID of each mesh
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
		primArray = polydata->GetLines();
		rendertype = Ogre::RenderOperation::OT_LINE_LIST;
		break;
	case 5: // polygon
	case 20000: // strips
		primArray = polydata ->GetPolys();
		if (m_extrude)
			rendertype = Ogre::RenderOperation::OT_TRIANGLE_LIST;
		else
			rendertype = Ogre::RenderOperation::OT_LINE_LIST;
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
	int i, npts, totpts = 0, *pts, transparentFlag = 0, trianglePointsIndex = 0;

    Ogre::SceneManager *sceneManager = OgreManager::getSceneManager();

	vtkIdType oldRecID = NULL;
	bool isNewMesh = false;

	// create a ManualObject for each vtkCell
	QString filename = getStringValue("Shapefile");
	Ogre::String meshName;
	Ogre::MeshPtr mesh;

	// go through cells (primitives) and create a mesh for each cell
	if (rendertype != Ogre::RenderOperation::OT_TRIANGLE_LIST)
	{
		table->AddColumn(xCentroids);
		table->AddColumn(yCentroids);
		table->AddColumn(zCentroids);
		for (primArray->InitTraversal(); primArray->GetNextCell(npts, pts); prim++)
		{ 
			// get record id
			vtkIdType recID = recIDs->GetValue(prim);

			// prepare the name for each manual object
			meshName = m_name.toStdString() + ":" + filename.toStdString() + ":" + Ogre::StringConverter::toString(prim);
			Ogre::ManualObject * manual;
			if (sceneManager->hasManualObject(meshName))
				manual = sceneManager->getManualObject(meshName);
			else {
				manual = sceneManager->createManualObject(meshName);
			}
			manual->clear();
			manual->begin("", rendertype);

			// prepare points arrays to calculate the centroid of the cell
			vtkIdTypeArray * pointsIDs = vtkIdTypeArray::New();
			vtkPoints * points = vtkPoints::New();
		
			double * vertex, x, y, z;
			// go through points in cell (verts)
			for (i=0; i < npts; i++)
			{
				vertex = polydata->GetPoint(pts[i]);

				x = vertex[0], y = vertex[1], z = vertex[2];
				points->InsertNextPoint(vertex);
				pointsIDs->InsertNextValue(i);
				manual->position(x, y, z);
				manual->index(i);
			}
			// if it's a polygon, than close it with the first point (don't add it again to points, otherwise centroids are wrong)
			if (npts > 0 && rendertype == Ogre::RenderOperation::OT_LINE_STRIP)
			{
				vertex = polydata->GetPoint(pts[0]);
				x = vertex[0], y = vertex[1], z = vertex[2];
				manual->position(x, y, z);
				manual->index(0);
			}

			manual->end();

			// convert the manual object into a mesh (for multiple instantiation)
			manual->convertToMesh(meshName);

			// store the mesh name 
			meshNames->InsertNextValue(meshName);

			// store record id
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
		table->AddColumn(recIDs);
		outputShapeMapParameter->setHasCentroids(true);
	} else {
		double * vertex, x, y, z;

		// create a polydata for each rec_id
		vtkPolyData * polyGroup = vtkPolyData::New();

		vtkCellArray * cellsGroup = vtkCellArray::New();

		for (primArray->InitTraversal(); primArray->GetNextCell(npts, pts); prim++)
		{ 
			// get record id
			vtkIdType recID = recIDs->GetValue(prim);

			if (oldRecID == NULL)
				oldRecID = recID;

			// create a unique polydata for all cells derived from the same rec_id
			if (recID != oldRecID)
			{
				polyGroup->SetPolys(cellsGroup);
				polyGroup->SetPoints(polydata->GetPoints());

				// extrude the polydata to make it 3D
				// prepare the extrusion in case of polygons or triangulated shape
				double scaleFactor = vtkMath::Random(0,5);
				vtkLinearExtrusionFilter * extrude = vtkLinearExtrusionFilter::New();
				extrude->SetInput(polyGroup);
				extrude->SetScaleFactor(scaleFactor);
				extrude->SetExtrusionTypeToNormalExtrusion();
				extrude->SetVector(0,0,1);
				extrude->CappingOn();
				extrude->Update();

				// create polydata with list of triangles from extruded mesh
				vtkTriangleFilter * triangles = vtkTriangleFilter::New();
				triangles->SetInput(extrude->GetOutput());
				triangles->Update();

				// add normals to the triangles
				vtkPolyDataNormals * normals = vtkPolyDataNormals::New();
				normals->SetInputConnection(triangles->GetOutputPort());
				normals->SetFeatureAngle(60.0);
				normals->Update();

				Ogre::ColourValue color(vtkMath::Random(0,1),vtkMath::Random(0,1),vtkMath::Random(0,1),1);
				// prepare the name for each manual object
				meshName = m_name.toStdString() + ":extruded:" + filename.toStdString() + ":" + Ogre::StringConverter::toString(oldRecID);
				extrudedPolydataToMesh(normals->GetOutput(), meshName, color);

				// store the mesh name 
				meshNames->InsertNextValue(meshName);
				// store id
				id->InsertNextValue(oldRecID);

				triangles->Delete();
				extrude->Delete();
				polyGroup->Delete();
				cellsGroup->Delete();

				polyGroup = vtkPolyData::New();

				// create new polydata for the new rec_id
				polyGroup = vtkPolyData::New();
				cellsGroup = vtkCellArray::New();

				oldRecID = recID;
			}

			cellsGroup->InsertNextCell(npts, pts);
		}
		outputShapeMapParameter->setHasCentroids(false);
		polyGroup->Delete();
		cellsGroup->Delete();
	}

//	primArray->Delete();
//	extrude->Delete();
	id->Delete();
//	recIDs->Delete();
	meshNames->Delete();
	zCentroids->Delete();
	xCentroids->Delete();
	yCentroids->Delete();
	return table;
}

// Empty the table and destroy all meshes contained on it
void ShpSourceNode::cleanTable()
{
	if (!m_table || !outputShapeMapParameter)
		return;

    Ogre::SceneManager *sceneManager = OgreManager::getSceneManager();

	int rows = m_table->GetNumberOfRows();

	for (vtkIdType id = 0; id<rows; id++)
	{
		Ogre::String meshName = outputShapeMapParameter->getShapeName(id).toStdString();

		// if the mesh is already registered than destroy it
		if (sceneManager->hasManualObject(meshName))
			sceneManager->destroyManualObject(meshName);
	}
	m_table->Delete();
	m_table = 0;
	outputShapeMapParameter->setVTKTable(m_table);
}

// Create a mesh from an extruded polydata
void ShpSourceNode::extrudedPolydataToMesh(vtkPolyData * polydata, Ogre::String meshName, Ogre::ColourValue color)
{
	// retrieve cells from polydata
	vtkCellArray * triangles = polydata->GetPolys();

	// retrieve normals from polydata
	vtkDataArray * normals = polydata->GetPointData()->GetNormals();
	bool normalPerVertex = false, normalPerCell = false;
	if (normals)
		normalPerVertex = true;
	else 
	{
		normals = polydata->GetCellData()->GetNormals();
		if (normals)
			normalPerCell = true;
	}

	// if there are no triangles, return
	if (triangles->GetNumberOfCells() <= 0)
		return;

	// create or retrieve manual object
    Ogre::SceneManager *sceneManager = OgreManager::getSceneManager();
	Ogre::ManualObject * manual;
	if (sceneManager->hasManualObject(meshName))
		manual = sceneManager->getManualObject(meshName);
	else {
		manual = sceneManager->createManualObject(meshName);
	}
	manual->clear();
	manual->begin("BaseWhiteNoLighting", Ogre::RenderOperation::OT_TRIANGLE_LIST);

	double * vertex, x, y, z, * pointNormals;
	unsigned int prim = 0;
	int i, npts, *pts;
	for (triangles->InitTraversal(); triangles->GetNextCell(npts, pts); prim+=3)
	{ 
		for (int i=0; i<npts; i++)
		{
			// go through points in each triangle (cell)
			vertex = polydata->GetPoint(pts[i]);
			x = vertex[0], y = vertex[1], z = vertex[2];
			manual->position(x, y, z);
			if (normalPerVertex)
			{
				pointNormals = normals->GetTuple(pts[i]);
				manual->normal(pointNormals[0], pointNormals[1], pointNormals[2]);
				if (pointNormals[2] > 0)
					manual->textureCoord((float)z,(float)0);
				else if (pointNormals[2] < 0)
					manual->textureCoord((float)z,(float)y);
				else
					manual->textureCoord((float)x,(float)z);
			}
			manual->textureCoord((float)0,(float)0);
			manual->index(prim+i);
			manual->colour(color.r,color.g,color.b);
		}
	}

	manual->end();

	manual->setCastShadows(true);
	// convert the manual object into a mesh (for multiple instantiation)
	manual->convertToMesh(meshName);
}