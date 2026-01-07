#include "FITKAbsGeoRepairSmallFaces.h"

namespace Interface {

    FITKGeoEnum::FITKGeometryComType FITKAbsGeoRepairSmallFaces::getGeometryCommandType()
    {
        return FITKGeoEnum::FITKGeometryComType::FGTRepairSmallFaces;
    }

}