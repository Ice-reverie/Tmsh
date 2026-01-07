#include "FITKAbsGeoReplaceFaces.h"

namespace Interface {

    FITKGeoEnum::FITKGeometryComType FITKAbsGeoReplaceFaces::getGeometryCommandType()
    {
        return FITKGeoEnum::FITKGeometryComType::FGTReplaceFaces;
    }

}