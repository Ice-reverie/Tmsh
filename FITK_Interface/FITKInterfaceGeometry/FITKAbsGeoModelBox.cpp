#include "FITKAbsGeoModelBox.h" 
#include "FITK_Interface/FITKInterfaceModel/FITKAbstractModel.h"

namespace Interface
{

    FITKAbsGeoModelBox::~FITKAbsGeoModelBox()
    {

    }

    void FITKAbsGeoModelBox::setPoint1(double* p1)
    {
        //第一点
        if(p1 == nullptr) return;
        FITKAbstractModel::CopyArray(p1, _point1, 3);
    }

    void FITKAbsGeoModelBox::setLength(double* p2)
    {
        if(p2 == nullptr) return;
        //第二点
        FITKAbstractModel::CopyArray(p2, _lengthParam, 3);
    }

     

    void FITKAbsGeoModelBox::getPoint1(double* p1)
    {
        //第一点
        if (p1 == nullptr) return;
        FITKAbstractModel::CopyArray( _point1, p1, 3);
    }

    void FITKAbsGeoModelBox::getLength(double* p2)
    {
        if (p2 == nullptr) return;
        //第二点
        FITKAbstractModel::CopyArray(_lengthParam, p2, 3);
    }

    FITKGeoEnum::FITKGeometryComType FITKAbsGeoModelBox::getGeometryCommandType()
    {
        return FITKGeoEnum::FGTBox;
    }

}
