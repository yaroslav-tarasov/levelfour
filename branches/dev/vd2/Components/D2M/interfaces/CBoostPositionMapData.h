#ifndef CBOOST_POSTIONMAP_DATA_H
#define CBOOST_POSTIONMAP_DATA_H

#include "IBoostPositionMapIOData.h"

class CBoostPositionMapData : public IBoostPositionMapIOData
{
public:
	CBoostPositionMapData();
	~CBoostPositionMapData();

	void setPositionMap(sandra::PositionMap* positionMap);

	//IVisSystemNodeIOData implementation
	void* dataPointer();
	QString dataTypeName();

	//IQueryable implementation
	bool queryInterface(QString iface, void** ifacePtr);
	bool destroySelf();

	//IBoostGraphIOData implementation
	sandra::PositionMap* getBoostPositionMap();

private:
	sandra::PositionMap* m_positionMap;
};

#endif