#ifndef DATA_TABLE_CREATOR_H
#define DATA_TABLE_CREATOR_H

#include "GraphPropertyMapper.h"
#include "vtkTable.h"
#include "vtkStringArray.h"
#include "vtkFloatArray.h"

template<typename VertexListGraph, typename PositionMap, typename VertexIdMap>
class DataTableCreator
{

public:

	vtkTable *createTable(VertexListGraph g, PositionMap positionMap, VertexIdMap vertexIdMap)
	{
		vtkTable *table = vtkTable::New();
		//Create a column named "NodeId"
		vtkStringArray *colNodeId = vtkStringArray::New();
		colNodeId->SetName("NodeId");
		table->AddColumn(colNodeId);
		//Create columns named "X", "Y" and "Z"
		vtkFloatArray *colX = vtkFloatArray::New();
		colX->SetName("X");
		table->AddColumn(colX);
		vtkFloatArray *colY = vtkFloatArray::New();
		colY->SetName("Y");
		table->AddColumn(colY);
		vtkFloatArray *colZ = vtkFloatArray::New();
		colZ->SetName("Z");
		table->AddColumn(colZ);

		boost::graph_traits<VertexListGraph>::vertex_iterator i, end;
		for (boost::tie(i, end) = boost::vertices(g); i != end; ++i) {
			//Add the vertex ID to the "NodeId" column
			char vertexId[10];
			sprintf(vertexId, "%d",vertexIdMap[*i]); //Convert the vertex ID to a string
			colNodeId->InsertNextValue( vertexId );

			//Add the position values to columns "X", "Y" and "Z"
			colX->InsertNextValue( positionMap[*i][0] );
			colY->InsertNextValue( positionMap[*i][1] );
			colZ->InsertNextValue( positionMap[*i][2] );
		}
		return table;
	} //end function createTable

};

#endif //end DATA_TABLE_CREATOR_H