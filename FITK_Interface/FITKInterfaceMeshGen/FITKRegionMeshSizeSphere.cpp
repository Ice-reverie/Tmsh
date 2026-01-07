#include "FITKRegionMeshSizeSphere.h"


namespace Interface
{
    FITKRegionMeshSizeSphere::FITKRegionMeshSizeSphere()
    {
        //只有一个面
        this->insertBoundary(0, FITKAbstractRegionMeshSize::BTPatch);
        this->insertFaceName(0, "boundaries");
    }


    FITKAbstractRegionMeshSize::RegionType FITKRegionMeshSizeSphere::getRegionType()
    {
        //球
        return FITKAbstractRegionMeshSize::RegionType::RegionSphere;
    }

    void FITKRegionMeshSizeSphere::setLocation(double* p1)
    {
        //球心位置
        if (p1 == nullptr) return;
        for (int i = 0; i < 3; ++i)
            _location[i] = p1[i];
    }

    void FITKRegionMeshSizeSphere::getLocation(double* p1)
    {
        //球心位置
        if (p1 == nullptr) return;
        for (int i = 0; i < 3; ++i)
            p1[i] = _location[i];
    }

    void FITKRegionMeshSizeSphere::setRadius(double r)
    {
        //r is the radius of the sphere
        _radius = r;
    }

    double FITKRegionMeshSizeSphere::getRadius() const
    {
        return _radius;
    }

}



