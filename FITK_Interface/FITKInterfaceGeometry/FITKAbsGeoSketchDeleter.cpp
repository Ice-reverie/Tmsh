#include "FITKAbsGeoSketchDeleter.h"

namespace Interface {
    FITKGeoEnum::FITKGeometryComType FITKAbsGeoSketchDeleter::getGeometryCommandType()
    {
        return FITKGeoEnum::FITKGeometryComType::FGTSketchDeleter;
    }

}