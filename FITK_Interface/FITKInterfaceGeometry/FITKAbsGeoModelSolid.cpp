#include "FITKAbsGeoModelSolid.h" 
#include "FITK_Interface/FITKInterfaceModel/FITKAbstractModel.h"

namespace Interface
{
    FITKGeoEnum::FITKGeometryComType FITKAbsGeoModelExtrudeSolid::getGeometryCommandType()
    {
        return FITKGeoEnum::FITKGeometryComType::FGTExtrudeSolid;
    }
    FITKGeoEnum::FITKGeometryComType FITKAbsGeoModelRevolSolid::getGeometryCommandType()
    {
        return FITKGeoEnum::FITKGeometryComType::FGTRevolSolid;
    }
    FITKGeoEnum::FITKGeometryComType FITKAbsGeoModelSweepSolid::getGeometryCommandType()
    {
        return FITKGeoEnum::FITKGeometryComType::FGTSweepSolid;
    }
    FITKGeoEnum::FITKGeometryComType FITKAbsGeoModelClosedSurfaceSolid::getGeometryCommandType()
    {
        return FITKGeoEnum::FITKGeometryComType::FGTClosedSurfaceSolid;
    }
    FITKGeoEnum::FITKGeometryComType FITKAbsGeoModelMultiSectionSolid::getGeometryCommandType()
    {
        return FITKGeoEnum::FITKGeometryComType::FGTMultiSectionSolid;
    }
}
