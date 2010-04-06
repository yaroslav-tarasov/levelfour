#include "GraphPropertyMapper.h"

	int main(int,char*[])
	{
		// Create a GraphPropertyMapper
		GraphPropertyMapper<boost::directedS> graphMapper;

		// Add vertices to the GraphPropertyMapper
		for (int i = 0; i < 17; ++i) {
			graphMapper.addVertex(i);
		}

		// Add edges to the GraphPropertyMapper
		graphMapper.addEdge(0,1,1);
		graphMapper.addEdge(0,2,1);
		graphMapper.addEdge(0,3,1);
		graphMapper.addEdge(1,4,1);
		graphMapper.addEdge(1,5,1);
		graphMapper.addEdge(1,6,1);
		graphMapper.addEdge(1,7,1);
		graphMapper.addEdge(2,8,1);
		graphMapper.addEdge(2,9,1);
		graphMapper.addEdge(3,10,1);
		graphMapper.addEdge(3,11,1);
		graphMapper.addEdge(3,12,1);
		graphMapper.addEdge(3,13,1);
		graphMapper.addEdge(3,14,1);
		graphMapper.addEdge(3,15,1);
		graphMapper.addEdge(3,16,1);

		// Apply the layout
		graphMapper.setPositionTransform(graphdefs::cone3D);
		graphMapper.transformPosition();
		
		// Create an OSG scene and save it as an osg file
		graphMapper.plotScene();

		return 0;
	}