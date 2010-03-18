#include "CBoostGraphData.h"

CBoostGraphData::CBoostGraphData()
{
	m_graph=0;
}

CBoostGraphData::~CBoostGraphData()
{

}

void CBoostGraphData::setGraph(sandra::Graph* graph)
{
	m_graph = graph;
}

void* CBoostGraphData::dataPointer()
{
	return m_graph;
}

QString CBoostGraphData::dataTypeName()
{
	return "boostGraph";
}

GCF_BEGIN_QUERY_TABLE(CBoostGraphData)
	GCF_IMPLEMENTS(IBoostGraphIOData)
GCF_END_QUERY_TABLE(CBoostGraphData)

sandra::Graph* CBoostGraphData::getBoostGraph()
{
	return m_graph;
}