#include "FITKAbsGeoRepairSmallEdges.h"

namespace Interface {

    FITKGeoEnum::FITKGeometryComType FITKAbsGeoRepairSmallEdges::getGeometryCommandType()
    {
        return FITKGeoEnum::FITKGeometryComType::FGTRepairSmallEdges;
    }

}