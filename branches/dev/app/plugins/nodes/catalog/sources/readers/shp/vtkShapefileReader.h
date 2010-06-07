/*=========================================================================

  Program:   Visualization Toolkit
  Module:    $RCSfile: vtkShapefileReader.h,v $
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
// .NAME vtkShapefileReader - read ESRI binary shapefiles
// .SECTION Description
// vtkShapefileReader is a source object that reads .shp and shx files.
// The output of this source object is polygonal data.


#ifndef vtkShapefileReader_h
#define vtkShapefileReader_h

#include "vtkPolyDataSource.h"
#include "vtkIdList.h"
#include "vtkCellArray.h"
#include "vtkUnsignedIntArray.h"
#include "vtkPoints.h"
#include "vtkDoubleArray.h"
#include "vtkByteSwap.h"

// declare typedefs here to get past wrapper parsers
class vtkShapefileReader;
typedef void (vtkShapefileReader::*ReadMethod)(int rNo,char *data,vtkPoints *points,vtkCellArray *cells,
						vtkUnsignedIntArray *values,int *count,int *pts_count,
						int is_z,int is_m,vtkByteSwap* swap);
typedef void (vtkPolyData::*CellMethod)(vtkCellArray *cells);


class vtkShapefileReader : public vtkPolyDataSource 
{
public:
  static vtkShapefileReader *New();
  vtkTypeRevisionMacro(vtkShapefileReader,vtkPolyDataSource);
  void PrintSelf(ostream& os, vtkIndent indent);

  // Description:
  // Specify file name of .shp file.
  vtkSetStringMacro(FileName);
  vtkGetStringMacro(FileName);

  vtkSetMacro(FillPolygons,int);
  vtkGetMacro(FillPolygons,int);

  vtkSetMacro(GenerateIds,int);
  vtkGetMacro(GenerateIds,int);

  vtkGetMacro(Type,int);

  vtkGetMacro(NumberOfRecords,int);

  void SetIdList(vtkIdList* ids);
  vtkIdList* GetIdList();

  vtkGetStringMacro(TypeAsString);

protected:
  vtkShapefileReader();

  ~vtkShapefileReader();
  
  void Execute();
  
  char* GetIndexFileName(char* sFileName);
  
  int SetParams(int type,ReadMethod &rm,CellMethod &cm,int &is_z,int &is_m,char* &typestr);
  
  int ParseIndexHeader(char *data,int &code,int &vers,int &type,int &nRecs,vtkByteSwap* swap);

  void ReadPoint		(int rNo,char *data,vtkPoints *points,vtkCellArray *cells,
						vtkUnsignedIntArray *values,int *count,int *pts_count,
						int is_z,int is_m,vtkByteSwap* swap);

  void ReadPolygon		(int rNo,char *data,vtkPoints *points,vtkCellArray *cells,
						vtkUnsignedIntArray *values,int *count,int *pts_count,
						int is_z,int is_m,vtkByteSwap* swap);
  
  void ReadPolyLine		(int rNo,char *data,vtkPoints *points,vtkCellArray *cells,
						vtkUnsignedIntArray *values,int *count,int *pts_count,
						int is_z,int is_m,vtkByteSwap* swap);

  void ReadMultiPoint	(int rNo,char *data,vtkPoints *points,vtkCellArray *cells,
						vtkUnsignedIntArray *values,int *count,int *pts_count,
						int is_z,int is_m,vtkByteSwap* swap);

  
  void ReadMultiPatch	(int rNo,char *data,vtkPoints *points,vtkCellArray *cells,
						vtkUnsignedIntArray *values,int *count,int *pts_count,
						int is_z,int is_m,vtkByteSwap* swap);


  char *FileName;
  char *TypeAsString;
  int FillPolygons;
  int GenerateIds;
  int Type;
  int NumberOfRecords;
  vtkIdList *IdList;
private:
  vtkShapefileReader(const vtkShapefileReader&);  // Not implemented.
  void operator=(const vtkShapefileReader&);  // Not implemented.
};

#endif


