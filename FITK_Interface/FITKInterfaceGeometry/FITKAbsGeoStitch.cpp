#include "FITKAbsGeoStitch.h"

namespace Interface {
    FITKGeoEnum::FITKGeometryComType Interface::FITKAbsGeoStitch::getGeometryCommandType()
    {
        return FITKGeoEnum::FITKGeometryComType::FGTStitch;
    }

}