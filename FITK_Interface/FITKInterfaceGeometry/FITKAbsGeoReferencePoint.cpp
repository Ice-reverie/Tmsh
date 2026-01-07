#include "FITKAbsGeoModelPoint.h" 
#include "FITK_Interface/FITKInterfaceModel/FITKAbstractModel.h"
#include "FITKAbsGeoReferencePoint.h"

namespace Interface
{
    Interface::FITKGeoEnum::FITKDatumType FITKAbsGeoReferencePoint::getDatumType()
    {
        return Interface::FITKGeoEnum::FITKDatumType::FDTPoint;
    }


    Interface::FITKGeoEnum::FITKDatumType FITKAbsGeoReferenceCurveRadio::getDatumType()
    {
        return Interface::FITKGeoEnum::FITKDatumType::FDTCurveRadio;
    }

}
