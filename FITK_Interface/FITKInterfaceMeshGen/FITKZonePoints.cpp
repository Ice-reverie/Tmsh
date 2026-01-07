
#include "FITKZonePoints.h"

Interface::FITKZonePointManager::FITKZonePointManager()
{
    //默认有一个成员，位置是0，0，0
    FITKZonePoint* p = new FITKZonePoint(0, 0, 0);
    this->appendDataObj(p);
}

