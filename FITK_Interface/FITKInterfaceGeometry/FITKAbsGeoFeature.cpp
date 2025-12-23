#include "FITKAbsGeoFeature.h"

namespace Interface {
    FITKGeoEnum::FITKGeometryComType FITKAbsGeoStiffener::getGeometryCommandType()
    {
        return FITKGeoEnum::FITKGeometryComType::FGTStiffener;
    }

}