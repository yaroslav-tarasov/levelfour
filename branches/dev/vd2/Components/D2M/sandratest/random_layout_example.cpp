#include "GraphPropertyMapper.h"
#include "TestPlotter.h"

	int main(int,char*[])
	{
		// Create a GraphPropertyMapper
		GraphPropertyMapper graphMapper;

		// Add vertices to the GraphPropertyMapper
		for (int i = 0; i < 7; ++i) {
			graphMapper.addVertex(i);
		}

		// Add edges to the GraphPropertyMapper
/*		graphMapper.addEdge(0,1);
		graphMapper.addEdge(1,2);
		graphMapper.addEdge(2,3);
		graphMapper.addEdge(3,4);
		graphMapper.addEdge(4,5);
		graphMapper.addEdge(5,6);
		graphMapper.addEdge(6,0);
*/
		// Apply the layout
		graphMapper.setPositionTransform(graphdefs::random2D);
		graphMapper.transformPosition();
		
		// Create an OSG scene and save it as an osg file
		TestPlotter tp;
		tp.constructScene(graphMapper.g, graphMapper.positionMap);
		return 0;

	}