#include "CBoostPositionMapData.h"

CBoostPositionMapData::CBoostPositionMapData()
{
	m_positionMap=0;
}

CBoostPositionMapData::~CBoostPositionMapData()
{

}

void CBoostPositionMapData::setPositionMap(sandra::PositionMap* positionMap)
{
	m_positionMap = positionMap;
}

void* CBoostPositionMapData::dataPointer()
{
	return m_positionMap;
}

QString CBoostPositionMapData::dataTypeName()
{
	return "boostPositionMap";
}

GCF_BEGIN_QUERY_TABLE(CBoostPositionMapData)
	GCF_IMPLEMENTS(IBoostPositionMapIOData)
GCF_END_QUERY_TABLE(CBoostPositionMapData)

sandra::PositionMap* CBoostPositionMapData::getBoostPositionMap()
{
	return m_positionMap;
}