#include "FITKAbsGeoSplitter.h"

namespace Interface {
    FITKGeoEnum::FITKGeometryComType Interface::FITKAbsGeoCurveSplitter::getGeometryCommandType()
    {
        return FITKGeoEnum::FITKGeometryComType::FGTCurveSplitter;
    }

    FITKGeoEnum::FITKGeometryComType Interface::FITKAbsGeoSurfaceSplitter::getGeometryCommandType()
    {
        return FITKGeoEnum::FITKGeometryComType::FGTSurfaceSplitter;
    }

    FITKGeoEnum::FITKGeometryComType Interface::FITKAbsGeoSolidSplitter::getGeometryCommandType()
    {
        return FITKGeoEnum::FITKGeometryComType::FGTSolidSplitter;
    }

}