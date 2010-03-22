#ifndef IBOOST_POSITIONMAP_IO_DATA_H
#define IBOOST_POSITIONMAP_IO_DATA_H

#include "IVisSystemNodeIOData.h"
#include "IBoostGraphIOData.h"

//class PositionMap;
class IBoostPositionMapIOData : virtual public IVisSystemNodeIOData
{
public:
	virtual graphdefs::PositionMap* getBoostPositionMap() = 0;
};

#endif