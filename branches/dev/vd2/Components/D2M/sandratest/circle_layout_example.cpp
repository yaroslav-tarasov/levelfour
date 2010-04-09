#include "GraphPropertyMapper.h"

	int main(int,char*[])
	{
		// Create a GraphPropertyMapper
		GraphPropertyMapper<boost::undirectedS> graphMapper;

		// Add vertices to the GraphPropertyMapper
		for (int i = 0; i < 7; ++i) {
			graphMapper.addVertex(i);
		}

		// Add edges to the GraphPropertyMapper
		graphMapper.addEdge(1,0,1);
		graphMapper.addEdge(2,6,1);
		graphMapper.addEdge(2,4,1);
		graphMapper.addEdge(4,1,1);

		// Apply the layout
		graphMapper.setPositionTransform(graphdefs::circle2D);
		graphMapper.transformPosition();
		
		// Create an OSG scene and save it as an osg file
		graphMapper.plotScene();

		return 0;

	}