#include "GraphPropertyMapper.h"

	int main(int,char*[])
	{
		// Create a GraphPropertyMapper
		GraphPropertyMapper<boost::undirectedS> graphMapper;

		// Add vertices to the GraphPropertyMapper
		for (int i = 0; i < 8; ++i) {
			graphMapper.addVertex(i);
		}

		// Add edges to the GraphPropertyMapper
		graphMapper.addEdge(0,1,1);
		graphMapper.addEdge(1,2,1);
		graphMapper.addEdge(2,3,5);
		graphMapper.addEdge(3,4,5);
		graphMapper.addEdge(4,5,1);
		graphMapper.addEdge(5,6,1);
		graphMapper.addEdge(6,7,1);
		graphMapper.addEdge(7,0,1);

		// Apply the layout
		graphMapper.setPositionTransform(graphdefs::kamada_kawai2D);
		graphMapper.transformPosition();
		
		// Create an OSG scene and save it as an osg file
		graphMapper.plotScene();

		return 0;

	}