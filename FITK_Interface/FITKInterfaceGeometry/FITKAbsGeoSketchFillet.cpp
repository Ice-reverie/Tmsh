#include "FITKAbsGeoSketchFillet.h"

namespace Interface {
    FITKGeoEnum::FITKGeometryComType FITKAbsGeoSketchFillet::getGeometryCommandType()
    {
        return FITKGeoEnum::FITKGeometryComType::FGTSketchFillet;
    }

}