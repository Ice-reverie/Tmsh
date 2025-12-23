#include "FITKAbsGeoModelSphere.h"
#include "FITK_Interface/FITKInterfaceModel/FITKAbstractModel.h"

namespace Interface
{

    FITKAbsGeoModelSphere::~FITKAbsGeoModelSphere()
    {

    }

    void FITKAbsGeoModelSphere::setLocation(double* p1)
    {
        //圆心坐标
        if (p1 == nullptr) return;
        FITKAbstractModel::CopyArray(p1, _location, 3);
    }

    void FITKAbsGeoModelSphere::getLocation(double* p1)
    {
        //圆心坐标
        if (p1 == nullptr) return;
        FITKAbstractModel::CopyArray( _location, p1, 3);
    }

    void FITKAbsGeoModelSphere::setRadius(double r)
    {
        _radius = r;
    }

    double FITKAbsGeoModelSphere::getRadius() const
    {
        return _radius;
    }

    FITKGeoEnum::FITKGeometryComType FITKAbsGeoModelSphere::getGeometryCommandType()
    {
        //创建球体
        return FITKGeoEnum::FGTSphere;
    }

}