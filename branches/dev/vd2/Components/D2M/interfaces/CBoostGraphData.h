#ifndef CBOOST_GRAPH_DATA_H
#define CBOOST_GRAPH_DATA_H

#include "IBoostGraphIOData.h"

class CBoostGraphData : public IBoostGraphIOData
{
public:
	CBoostGraphData();
	~CBoostGraphData();

	void setGraph(graphdefs::Graph* graph);

	//IVisSystemNodeIOData implementation
	void* dataPointer();
	QString dataTypeName();

	//IQueryable implementation
	bool queryInterface(QString iface, void** ifacePtr);
	bool destroySelf();

	//IBoostGraphIOData implementation
	graphdefs::Graph* getBoostGraph();

private:
	graphdefs::Graph* m_graph;
};

#endif