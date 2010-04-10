#include "GraphPropertyMapper.h"

	int main(int,char*[])
	{
		// Create a GraphPropertyMapper
		GraphPropertyMapper<boost::undirectedS> graphMapper;

		// Add vertices to the GraphPropertyMapper
		for (int i = 0; i < 80; ++i)
		{
			graphMapper.addVertex(i);
		}

		// Apply the layout
		graphMapper.setPositionTransform(graphdefs::sphere3D);
		graphMapper.transformPosition();
		
		// Create an OSG scene and save it as an osg file
		graphMapper.plotScene();

		return 0;
	}