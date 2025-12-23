#include "FITKAbsGeoModelCurve.h" 
#include "FITK_Interface/FITKInterfaceModel/FITKAbstractModel.h"
#include "FITKAbsGeoReferenceLine.h"

namespace Interface
{
    FITKGeoEnum::FITKDatumType FITKAbsGeoReferenceLine::getDatumType()
    {
        return FITKGeoEnum::FITKDatumType::FDTLine;
    }

    FITKGeoEnum::FITKDatumType FITKAbsGeoReferenceSegment::getDatumType()
    {
        return FITKGeoEnum::FITKDatumType::FDTSegment;
    }

    FITKGeoEnum::FITKDatumType FITKAbsGeoReferenceLineByIntersectPlanes::getDatumType()
    {
        return FITKGeoEnum::FITKDatumType::FDTLineByIntersectPlanes;
    }

    FITKGeoEnum::FITKDatumType FITKAbsGeoReferenceLineByRotateLine::getDatumType()
    {
        return FITKGeoEnum::FITKDatumType::FDTLineByRotateLine;
    }
}
