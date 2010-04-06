#include "GraphPropertyMapper.h"

	int main(int,char*[])
	{
		// Create a GraphPropertyMapper
		GraphPropertyMapper<boost::directedS> graphMapper;

		// Add vertices to the GraphPropertyMapper
		for (int i = 0; i < 50; ++i) {
			graphMapper.addVertex(i);
		}

		// Add edges to the GraphPropertyMapper
		graphMapper.addEdge(0,1,1);
		graphMapper.addEdge(0,2,1);
		graphMapper.addEdge(0,3,1);
		graphMapper.addEdge(0,4,1);
		graphMapper.addEdge(0,5,1);
		graphMapper.addEdge(0,6,1);
		graphMapper.addEdge(0,7,1);
		graphMapper.addEdge(1,8,1);
		graphMapper.addEdge(1,9,1);
		graphMapper.addEdge(1,10,1);
		graphMapper.addEdge(1,11,1);
		graphMapper.addEdge(1,12,1);
		graphMapper.addEdge(1,13,1);
		graphMapper.addEdge(1,14,1);
		graphMapper.addEdge(2,15,1);
		graphMapper.addEdge(2,16,1);
		graphMapper.addEdge(2,17,1);
		graphMapper.addEdge(2,18,1);
		graphMapper.addEdge(3,19,1);
		graphMapper.addEdge(3,20,1);
		graphMapper.addEdge(3,21,1);
		graphMapper.addEdge(3,22,1);
		graphMapper.addEdge(3,23,1);
		graphMapper.addEdge(3,24,1);
		graphMapper.addEdge(3,25,1);
		graphMapper.addEdge(3,26,1);
		graphMapper.addEdge(3,27,1);
		graphMapper.addEdge(3,28,1);
		graphMapper.addEdge(3,29,1);
		graphMapper.addEdge(3,30,1);
		graphMapper.addEdge(3,31,1);
		graphMapper.addEdge(4,32,1);
		graphMapper.addEdge(4,33,1);
		graphMapper.addEdge(4,34,1);
		graphMapper.addEdge(5,35,1);
		graphMapper.addEdge(5,36,1);
		graphMapper.addEdge(5,37,1);
		graphMapper.addEdge(5,38,1);
		graphMapper.addEdge(6,39,1);
		graphMapper.addEdge(6,40,1);
		graphMapper.addEdge(6,41,1);
		graphMapper.addEdge(6,42,1);
		graphMapper.addEdge(6,43,1);
		graphMapper.addEdge(6,44,1);
		graphMapper.addEdge(6,45,1);
		graphMapper.addEdge(6,46,1);
		graphMapper.addEdge(7,47,1);
		graphMapper.addEdge(7,48,1);
		graphMapper.addEdge(7,49,1);

		// Apply the layout
		graphMapper.setPositionTransform(graphdefs::phyllo3D);
		graphMapper.transformPosition();
		
		// Create an OSG scene and save it as an osg file
		graphMapper.plotScene();

		return 0;
	}