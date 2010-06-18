/*=========================================================================

  Program:   Visualization Toolkit
  Module:    $RCSfile: vtkShapefileReader.cxx,v $
  Language:  C++
  Date:      $Date: 2004/04/12 00:00:01 $
  Version:   $Revision: 0.01 $

  Copyright (c) 1993-2002 Ken Martin, Will Schroeder, Bill Lorensen 
  All rights reserved.
  See Copyright.txt or http://www.kitware.com/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notice for more information.

=========================================================================*/
#include "vtkShapefileReader.h"

#include "vtkCellArray.h"
#include "vtkUnsignedIntArray.h"
#include "vtkPoints.h"
#include "vtkDoubleArray.h"
#include "vtkObjectFactory.h"
#include "vtkPointData.h"
#include "vtkPolyData.h"
#include "vtkByteSwap.h"
#include "vtkCellData.h"

vtkCxxRevisionMacro(vtkShapefileReader, "$Revision: 0.01 $");
vtkStandardNewMacro(vtkShapefileReader);

// Description:
// Instantiate object with NULL filename.
vtkShapefileReader::vtkShapefileReader()
{
	this->FileName = NULL;
	this->FillPolygons = 0;
	this->GenerateIds = 1;
	this->Type = 0;
	this->NumberOfRecords = 0;
	this->IdList = NULL;
	this->TypeAsString = "";
}

vtkShapefileReader::~vtkShapefileReader()
{
	if (this->FileName)
	{
		delete [] this->FileName;
		this->FileName = NULL;
	}
	delete [] this->TypeAsString;
}

void vtkShapefileReader::SetIdList(vtkIdList *ids)
{
	this->IdList = ids;
}

vtkIdList* vtkShapefileReader::GetIdList()
{
	return this->IdList;
}

int vtkShapefileReader::SetParams(int type,ReadMethod &rm,CellMethod &cm,int &is_z,int &is_m,char* &typestr)
{	
	int OK = 1;
	switch (type) {
	case 1:
		{
			rm = &vtkShapefileReader::ReadPoint;
			cm = &vtkPolyData::SetVerts;
			is_z = 0;
			is_m = 0;
			typestr = "Point";
			break;
		}
	case 3:
		{
			rm = &vtkShapefileReader::ReadPolyLine;
			cm = &vtkPolyData::SetLines;
			is_z = 0;
			is_m = 0;
			typestr = "PolyLine";
			break;
		}
	case 5:
		{
			if (this->FillPolygons) {
				rm = &vtkShapefileReader::ReadPolygon;
				cm = &vtkPolyData::SetPolys;
			} else {
				rm = &vtkShapefileReader::ReadPolyLine;
				cm = &vtkPolyData::SetLines;
			}
			is_z = 0;
			is_m = 0;
			typestr = "Polygon";
			break;
		}
	case 8:
		{
			rm = &vtkShapefileReader::ReadMultiPoint;
			cm = &vtkPolyData::SetVerts;
			is_z = 0;
			is_m = 0;
			typestr = "MultiPoint";
			break;
		}
	case 11:
		{
			rm = &vtkShapefileReader::ReadPoint;
			cm = &vtkPolyData::SetVerts;
			is_z = 1;
			is_m = 0;
			typestr = "PointZ";
			break;
		}
	case 13:
		{
			rm = &vtkShapefileReader::ReadPolyLine;
			cm = &vtkPolyData::SetLines;
			is_z = 1;
			is_m = 0;
			typestr = "PolyLineZ";
			break;
		}
	case 15:
		{
			if (this->FillPolygons) {
				rm = &vtkShapefileReader::ReadPolygon;
				cm = &vtkPolyData::SetPolys;
			} else {
				rm = &vtkShapefileReader::ReadPolyLine;
				cm = &vtkPolyData::SetLines;
			}
			is_z = 1;
			is_m = 0;
			typestr = "PolygonZ";
			break;
		}
	case 18:
		{
			rm = &vtkShapefileReader::ReadMultiPoint;
			cm = &vtkPolyData::SetVerts;
			is_z = 1;
			is_m = 0;
			typestr = "MultiPointZ";
			break;
		}
	case 21:
		{
			rm = &vtkShapefileReader::ReadPoint;
			cm = &vtkPolyData::SetVerts;
			is_z = 0;
			is_m = 1;
			typestr = "PointM";
			break;
		}
	case 23:
		{
			rm = &vtkShapefileReader::ReadPolyLine;
			cm = &vtkPolyData::SetLines;
			is_z = 0;
			is_m = 1;
			typestr = "PolyLineM";
			break;
		}
	case 25:
		{
			if (this->FillPolygons) {
				rm = &vtkShapefileReader::ReadPolygon;
				cm = &vtkPolyData::SetPolys;
			} else {
				rm = &vtkShapefileReader::ReadPolyLine;
				cm = &vtkPolyData::SetLines;
			}
			is_z = 0;
			is_m = 1;
			typestr = "PolygonM";
			break;
		}
	case 28:
		{
			rm = &vtkShapefileReader::ReadMultiPoint;
			cm = &vtkPolyData::SetVerts;
			is_z = 0;
			is_m = 1;
			typestr = "MultiPointM";
			break;
		}
	case 31:
		{
			rm = &vtkShapefileReader::ReadMultiPatch;
			cm = &vtkPolyData::SetPolys;
			is_z = 1;
			is_m = 1;
			typestr = "MultiPatch";
			break;
		}
	default:
		{
			rm = NULL;
			cm = NULL;
			typestr = "";
			OK = 0;
		}
	}
	return OK;
}

char* vtkShapefileReader::GetIndexFileName(char* sFileName)
{
	char *ext = "shx";
	int pos = strlen(sFileName)-strlen(ext);
	char *iFileName = new char[strlen(sFileName)];
	strcpy(iFileName,sFileName);
	for (int i=0;i<strlen(ext);i++)
	{
		iFileName[pos+i] = ext[i];
	}
	return iFileName;
}

int vtkShapefileReader::ParseIndexHeader(char *data,int &code,int &vers,int &type,int &nRecs,vtkByteSwap* swap)
{
	int *i;
	i = (int*)&data[0];
	swap->Swap4BE(i);
	code = *i;
	if (code != 9994)
	{
		return 0;
	}
	i = (int*)&data[24];
	swap->Swap4BE(i);
	nRecs = ((*i)-50)/4;
	i = (int*)&data[28];
	swap->Swap4LE(i);
	vers = *i;
	i = (int*)&data[32];
	swap->Swap4LE(i);
	type = *i;
	return 1;
}

void vtkShapefileReader::ReadPoint(int rNo,
									 char *data,
									 vtkPoints *points,
									 vtkCellArray *cells,
									 vtkUnsignedIntArray *ids,
									 int *count,
									 int *pts_count,
									 int is_z,
									 int is_m,
									 vtkByteSwap* swap)
{
	double* d;
	double z = 0.0;
	double m = 0.0;
	if (is_z)
	{
		d = (double*)&data[20];
		swap->Swap8LE(d);
		z = *d;
		d = (double*)&data[28];
		swap->Swap8LE(d);
		m = *d;
	}
	else
		if (is_m)
		{
			d = (double*)&data[28];
			swap->Swap8LE(d);
			m = *d;
		}
	// x&y
	d = (double*)&data[4];
	swap->Swap8LE(d);
	int x = *d;
	d = (double*)&data[12];
	swap->Swap8LE(d);
	int y = *d;
	//
	points->InsertNextPoint(x,y,z);
	cells->InsertNextCell(1);
	cells->InsertCellPoint(*pts_count);
	if (ids)
		ids->InsertNextValue(rNo);
	*pts_count += 1;
}

void vtkShapefileReader::ReadPolyLine(int rNo,
									 char *data,
									 vtkPoints *points,
									 vtkCellArray *cells,
									 vtkUnsignedIntArray *ids,
									 int *count,
									 int *pts_count,
									 int is_z,
									 int is_m,
									 vtkByteSwap* swap)
{
	// todo - m value to point data
    // skip bbox - 4 doubles (bytes 4-36)
	int *i;
	i = (int*)&data[36];
	swap->Swap4LE(i);
	int nParts = *i;
	i = (int*)&data[40];
	swap->Swap4LE(i);
	int nPoints = *i;
	// calculate offsets
	int off_xy = 44+(4*nParts);
	int off_z = off_xy+(16*nPoints)+16; // +16 skips range
	int off_m = is_z ? off_z+(8*nPoints) : off_z;
	//
	int *parts = new int[nParts+1];
	int k;
	for (k=0;k<nParts;k++)
	{
		i = (int*)&data[44+(k*4)];
		swap->Swap4LE(i);
		parts[k] = *i;
	}
	parts[k] = nPoints;
	for (k=0;k<nParts;k++)
	{
		int n = parts[k+1]-parts[k];
		cells->InsertNextCell(n);
		for (int m=0;m<n;m++)
			cells->InsertCellPoint((*pts_count)+parts[k]+m);
		if (ids)
			ids->InsertNextValue(rNo);
	}
	
	double *d; // temp holder
	// z
	double *z = new double[nPoints];
	if (is_z)
		for (k=0;k<nPoints;k++)
		{
			d = (double*)&data[off_z+(k*8)];
			swap->Swap8LE(d);
			z[k] = *d;
		}
	else
		for (k=0;k<nPoints;k++)
			z[k] = 0.0;
	// xy
	for (k=0;k<nPoints;k++)
	{
		d = (double*)&data[off_xy+k*16];
		swap->Swap8LE(d);
		double x = *d;
		d = (double*)&data[off_xy+k*16+8];
		swap->Swap8LE(d);
		double y = *d;
		points->InsertNextPoint(x,y,z[k]);
	}
	delete [] z;
	//printf("%s","\n");
	*pts_count += nPoints;

	//printf("%d %d\n",nParts,nPoints);

	/*
	if len(data) == offm:
            pass # no measure
        else:
            offm += 16 # skip range
            allm = unpack(fmt,data[offm:offm+(nPoints*8)]) 
	*/
}


void vtkShapefileReader::ReadPolygon (int rNo,
									 char *data,
									 vtkPoints *points,
									 vtkCellArray *cells,
									 vtkUnsignedIntArray *ids,
									 int *count,
									 int *pts_count,
									 int is_z,
									 int is_m,
									 vtkByteSwap* swap)
{
	// todo - m value to point data
    // skip bbox - 4 doubles (bytes 4-36)
	int *i;
	i = (int*)&data[36];
	swap->Swap4LE(i);
	int nParts = *i;
	i = (int*)&data[40];
	swap->Swap4LE(i);
	int nPoints = *i;
	// calculate offsets
	int off_xy = 44+(4*nParts);
	int off_z = off_xy+(16*nPoints)+16; // +16 skips range
	int off_m = is_z ? off_z+(8*nPoints) : off_z;
	//
	int *parts = new int[nParts+1];
	int k;
	for (k=0;k<nParts;k++)
	{
		i = (int*)&data[44+(k*4)];
		swap->Swap4LE(i);
		parts[k] = *i;
	}
	parts[k] = nPoints;
	for (k=0;k<nParts;k++)
	{
		int n = parts[k+1]-parts[k];
		cells->InsertNextCell(n);
		for (int m=0;m<n;m++)
			cells->InsertCellPoint((*pts_count)+parts[k]+m);
		if (ids)
			ids->InsertNextValue(rNo);
	}
	
	double *d; // temp holder
	// z
	double *z = new double[nPoints];
	if (is_z)
		for (k=0;k<nPoints;k++)
		{
			d = (double*)&data[off_z+(k*8)];
			swap->Swap8LE(d);
			z[k] = *d;
		}
	else
		for (k=0;k<nPoints;k++)
			z[k] = 0.0;
	// xy
	for (k=0;k<nPoints;k++)
	{
		d = (double*)&data[off_xy+k*16];
		swap->Swap8LE(d);
		double x = *d;
		d = (double*)&data[off_xy+k*16+8];
		swap->Swap8LE(d);
		double y = *d;
		points->InsertNextPoint(x,y,z[k]);
	}
	delete [] z;
	//printf("%s","\n");
	*pts_count += nPoints;

	//printf("%d %d\n",nParts,nPoints);

	/*
	if len(data) == offm:
            pass # no measure
        else:
            offm += 16 # skip range
            allm = unpack(fmt,data[offm:offm+(nPoints*8)]) 
	*/
}

void vtkShapefileReader::ReadMultiPoint(int rNo,
									 char *data,
									 vtkPoints *points,
									 vtkCellArray *cells,
									 vtkUnsignedIntArray *ids,
									 int *count,
									 int *pts_count,
									 int is_z,
									 int is_m,
									 vtkByteSwap* swap)
{
	// todo - m value to point data
    // skip bbox - 4 doubles (bytes 4-36)
	int *i;
	i = (int*)&data[36];
	swap->Swap4LE(i);
	int nPoints = *i;
	int off_xy = 40;
	// calculate offsets
	//int off_xy = 44+(4*nParts);
	int off_z = off_xy+(16*nPoints)+16; // +16 skips range
	int off_m = is_z ? off_z+(8*nPoints) : off_z;
	//
	double *d; // temp holder
	// z
	double *z = new double[nPoints];
	int k;
	if (is_z)
		for (k=0;k<nPoints;k++)
		{
			d = (double*)&data[off_z+(k*8)];
			swap->Swap8LE(d);
			z[k] = *d;
		}
	else
		for (k=0;k<nPoints;k++)
			z[k] = 0.0;
	// xy
	for (k=0;k<nPoints;k++)
	{
		d = (double*)&data[off_xy+k*16];
		swap->Swap8LE(d);
		double x = *d;
		d = (double*)&data[off_xy+k*16+8];
		swap->Swap8LE(d);
		double y = *d;
		points->InsertNextPoint(x,y,z[k]);
		// cell for every point - so do in this loop
		cells->InsertNextCell(1);
		cells->InsertCellPoint(*pts_count+k);
		if (ids)
			ids->InsertNextValue(rNo);
		// squeeze handling of m in here
	}
	delete [] z;

	//printf("%s","\n");
	*pts_count += nPoints;
}


void vtkShapefileReader::ReadMultiPatch(int rNo,
									 char *data,
									 vtkPoints *points,
									 vtkCellArray *cells,
									 vtkUnsignedIntArray *ids,
									 int *count,
									 int *pts_count,
									 int is_z,
									 int is_m,
									 vtkByteSwap* swap)
{
	// todo - m value to point data
    // skip bbox - 4 doubles (bytes 4-36)
	int *i;
	i = (int*)&data[36];
	swap->Swap4LE(i);
	int nParts = *i;
	i = (int*)&data[40];
	swap->Swap4LE(i);
	int nPoints = *i;
	// calculate offsets
	int off_t = 44+(4*nParts); // part types
	int off_xy = off_t+(4*nParts);
	int off_z = off_xy+(16*nPoints)+16; // +16 skips range
	int off_m = is_z ? off_z+(8*nPoints) : off_z;
	//
	int *parts = new int[nParts+1];
	int k;
	for (k=0;k<nParts;k++)
	{
		i = (int*)&data[44+(k*4)];
		swap->Swap4LE(i);
		parts[k] = *i;
	}
	parts[k] = nPoints;
	// first do points -> z then xy -> points
	double *d; // temp holder
	// z
	double *z = new double[nPoints];
	if (is_z)
		for (k=0;k<nPoints;k++)
		{
			d = (double*)&data[off_z+(k*8)];
			swap->Swap8LE(d);
			z[k] = *d;
		}
	else
		for (k=0;k<nPoints;k++)
			z[k] = 0.0;
	// xy
	for (k=0;k<nPoints;k++)
	{
		d = (double*)&data[off_xy+k*16];
		swap->Swap8LE(d);
		double x = *d;
		d = (double*)&data[off_xy+k*16+8];
		swap->Swap8LE(d);
		double y = *d;
		points->InsertNextPoint(x,y,z[k]);
	}
	delete [] z;
	//printf("%s","\n");
	// now cells
	for (k=0;k<nParts;k++)
	{
		// get number of points in this part
		int n = parts[k+1]-parts[k];
		// get part type
		i = (int*)&data[off_t+(k*4)];
		swap->Swap4LE(i);
		int ptype = *i;
		switch (ptype)
		{
		case 0:
			{
				for (int j=0;j<(n-2);j++)
				{
                    cells->InsertNextCell(3);
                    // todo - check winding
                    if (j%2) {
						cells->InsertCellPoint(*pts_count+parts[k]+j);
                        cells->InsertCellPoint(*pts_count+parts[k]+j+1);
                        cells->InsertCellPoint(*pts_count+parts[k]+j+2);
					} else {
                        cells->InsertCellPoint(*pts_count+parts[k]+j);
                        cells->InsertCellPoint(*pts_count+parts[k]+j+2);
                        cells->InsertCellPoint(*pts_count+parts[k]+j+1);
					}
                    if (ids)
                        ids->InsertNextValue(rNo);
				}
				break;
			}
		case 1:
			{
				for (int j=0;j<(n-2);j++)
				{
                    cells->InsertNextCell(3);
                    cells->InsertCellPoint(*pts_count+parts[k]+0);
                    cells->InsertCellPoint(*pts_count+parts[k]+j+1);
                    cells->InsertCellPoint(*pts_count+parts[k]+j+2);
                    if (ids)
                        ids->InsertNextValue(rNo);
				}
				break;
			}
		case 2:
			{
				printf("ptype %d\n",ptype);
				break;
			}
		case 3:
			{
				printf("ptype %d\n",ptype);
				break;
			}
		case 4:
			{
				printf("ptype %d\n",ptype);
				break;
			}
		case 5:
			{
				printf("ptype %d\n",ptype);
				break;
			}
		}
	}
	// finally update point count
	*pts_count += nPoints;
}

void vtkShapefileReader::Execute()
{
	if (! this->FileName)
	{
		return;
	}
	FILE *fSHP,*fSHX;
	fSHX = fopen(GetIndexFileName(this->FileName),"r+b");
	fSHP = fopen(this->FileName,"r+b");
	if (!(fSHX && fSHP))
	{
		printf("%s\n","shx file missing");
		return;
	}

	vtkByteSwap *swap = vtkByteSwap::New();

	int OK;
	int code,vers,type,nRecs;
	
	char *data = new char[100];
	fread(data,100,1,fSHX);
	OK = this->ParseIndexHeader(data,code,vers,type,nRecs,swap);
	delete [] data;
	if (!OK)
	{
		printf("%s","invalid header");
		swap->Delete();
		return;
	}
	this->NumberOfRecords = nRecs;
	this->Type = type;

	// now set methods/vars
	int is_z = 0,is_m = 0;
	ReadMethod rm;
	CellMethod cm;
	if (!this->SetParams(type,rm,cm,is_z,is_m,this->TypeAsString))
	{
		printf("No reader for type %d",type);
		swap->Delete();
		return;
	}

	int n,rNo;
	vtkIdList* idlist = this->IdList;
	if (! this->IdList)
	{
		idlist = vtkIdList::New();
		idlist->SetNumberOfIds(nRecs);
		for (n=0;n<nRecs;n++)
			idlist->SetId(n,n+1);
	}

	vtkPoints *points = vtkPoints::New();
	vtkCellArray *cells = vtkCellArray::New();
	vtkUnsignedIntArray *ids = NULL;
	if (this->GenerateIds)
	{
		ids = vtkUnsignedIntArray::New();
		ids->SetName("rec_id");
	}

	printf("%d\n",idlist->GetNumberOfIds());
	// read records
	char *ibuff = new char[8];
	int *i;
	int rpos,rlen;
	int count = 0,pts_count = 0;
	
	for (n=0;n<idlist->GetNumberOfIds();n++)
	{
		rNo = idlist->GetId(n);
		// get record length from index file
		fseek(fSHX,100+((rNo-1)*8),0);
		fread(ibuff,8,1,fSHX);
		//
		i = (int*)&ibuff[0];
		swap->Swap4BE(i);
		rpos = (*i)*2;
		i = (int*)&ibuff[4];
		swap->Swap4BE(i);
		rlen = (*i)*2;
		data = new char[rlen];
		fseek(fSHP,rpos+8,0); // +8 skips record header
		fread(data,rlen,1,fSHP);
		// peek at rectype, skip if 0 else abort if != type
		i = (int*)&data[0];
		swap->Swap4LE(i);
		if (*i == 0)
			continue;
		if (*i != type)
		{
			count = 0;
			break;
		}
		//
		(this->*rm)(rNo,data,points,cells,ids,&count,&pts_count,is_z,is_m,swap);
		delete [] data;
		count += 1;
		// todo - update progress indicator
		// todo - improve error/quit handling/messages
	}
	// set up output if valid
	if (count > 0)
	{
		vtkPolyData* pd = this->GetOutput();
		pd->SetPoints(points);
		(pd->*cm)(cells);
		if (ids)	
			pd->GetCellData()->SetScalars(ids);
	}
	// clean-up whether valid or not
	points->Delete();
	cells->Delete();
	if (ids)
		ids->Delete();
	swap->Delete();
	delete [] ibuff;
	if (! this->IdList)
		idlist->Delete();
}

void vtkShapefileReader::PrintSelf(ostream& os, vtkIndent indent)
{
	this->Superclass::PrintSelf(os,indent);

	os << indent << "File Name: " 
		<< (this->FileName ? this->FileName : "(none)") << "\n";
	os << indent << "Type: " 
		<< this->Type << "\n";
	os << indent << "TypeAsString: " 
		<< this->TypeAsString << "\n";
	os << indent << "GenerateIds: " 
		<< this->GenerateIds << "\n";
	os << indent << "NumberOfRecords: " 
		<< this->NumberOfRecords << "\n";
	os << indent << "FillPolygons: " 
		<< this->FillPolygons << "\n";
}
