#include "FITKGeoModelManager.h"

namespace Interface
{
    FITKGeoModelManager::FITKGeoModelManager()
    {
       //构造函数
    }
    FITKGeoModelManager::~FITKGeoModelManager()
    {
        //析构函数
    }

    void FITKGeoModelManager::getBoundaryBox(double* minPt, double* maxPt)
    {
        const int n = this->getDataCount();
        for (int i=0;i<n; ++i)
        {
            FITKAbstractGeoModel* m = this->getDataByIndex(i);
            if(m == nullptr) continue;
            double min[3] = { 9e66,9e66,9e66 }, max[3] = { -9e66, -9e66, -9e66 };
            bool hasBds = m->getBoundaryBox(min, max);
            if (!hasBds)
            {
                continue;
            }

            for (int j =0; j < 3; ++j)
            {
                minPt[j] = minPt[j] < min[j] ? minPt[j] : min[j];
                maxPt[j] = maxPt[j] < max[j] ? maxPt[j] : max[j];
            }
        }
    }

}

