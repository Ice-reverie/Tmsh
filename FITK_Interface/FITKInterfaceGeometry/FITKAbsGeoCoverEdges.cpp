#include "FITKAbsGeoCoverEdges.h"

namespace Interface {

    FITKGeoEnum::FITKGeometryComType FITKAbsGeoCoverEdges::getGeometryCommandType()
    {
        return FITKGeoEnum::FITKGeometryComType::FGTCoverEdges;
    }

}