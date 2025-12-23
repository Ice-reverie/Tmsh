#include "FITKAbsGeoModelSurface.h" 
#include "FITK_Interface/FITKInterfaceModel/FITKAbstractModel.h"
#include "FITKAbsGeoReferencePlane.h"

namespace Interface
{
    FITKGeoEnum::FITKDatumType FITKAbsGeoReferenceOffsetPlane::getDatumType()
    {
        return FITKGeoEnum::FITKDatumType::FDTOffsetPlane;
    }
    FITKGeoEnum::FITKDatumType FITKAbsGeoReferenceThreePointsPlane::getDatumType()
    {
        return FITKGeoEnum::FITKDatumType::FDTThreePointsPlane;
    }
    FITKGeoEnum::FITKDatumType FITKAbsGeoReferenceEquationPlane::getDatumType()
    {
        return FITKGeoEnum::FITKDatumType::FDTEquationPlane;
    }

    FITKGeoEnum::FITKDatumType FITKAbsGeoReferencePointAndDirectionPlane::getDatumType()
    {
        return FITKGeoEnum::FITKDatumType::FDTPointAndDirectionPlane;
    }

    FITKGeoEnum::FITKDatumType FITKAbsGeoReferenceRotateFromPlanePlane::getDatumType()
    {
        return FITKGeoEnum::FITKDatumType::FDTRotateFromPlanePlane;
    }


}

