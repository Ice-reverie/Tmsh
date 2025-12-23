#include "FITKAbsGeoModelSurface.h" 
#include "FITK_Interface/FITKInterfaceModel/FITKAbstractModel.h"

namespace Interface
{
    FITKGeoEnum::FITKGeometryComType FITKAbsGeoModelOffsetSurface::getGeometryCommandType()
    {
        return FITKGeoEnum::FITKGeometryComType::FGTOffsetSurface;
    }
    FITKGeoEnum::FITKGeometryComType FITKAbsGeoModelExtrudeSurface::getGeometryCommandType()
    {
        return FITKGeoEnum::FITKGeometryComType::FGTExtrudeSurface;
    }
    FITKGeoEnum::FITKGeometryComType FITKAbsGeoModelRevolSurface::getGeometryCommandType()
    {
        return FITKGeoEnum::FITKGeometryComType::FGTRevolSurface;
    }
    FITKGeoEnum::FITKGeometryComType FITKAbsGeoModelSweepSurface::getGeometryCommandType()
    {
        return FITKGeoEnum::FITKGeometryComType::FGTSweepSurface;
    }
    FITKGeoEnum::FITKGeometryComType FITKAbsGeoModelClosedWireSurface::getGeometryCommandType()
    {
        return FITKGeoEnum::FITKGeometryComType::FGTClosedWireSurface;
    }

    FITKGeoEnum::FITKGeometryComType FITKAbsGeoModelBridgeSurface::getGeometryCommandType()
    {
        return FITKGeoEnum::FITKGeometryComType::FGTBridgeSurface;
    }

    FITKGeoEnum::FITKGeometryComType FITKAbsGeoModelSolidSurface::getGeometryCommandType()
    {
        return FITKGeoEnum::FITKGeometryComType::FGTSurfaceFromSolid;
    }

    FITKGeoEnum::FITKGeometryComType FITKAbsGeoModelMultiSectionSurface::getGeometryCommandType()
    {
        return FITKGeoEnum::FITKGeometryComType::FGTMultiSectionSurface;
    }

}
