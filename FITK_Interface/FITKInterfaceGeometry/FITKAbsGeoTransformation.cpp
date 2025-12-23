#include "FITKAbsGeoTransformation.h"

namespace Interface {

    FITKGeoEnum::FITKGeometryComType FITKAbsGeoModelTransform::getGeometryCommandType()
    {
        return FITKGeoEnum::FITKGeometryComType::FGTTransform;
    }

    FITKGeoEnum::FITKGeometryComType FITKAbsGeoModelTransformByTwoPoints::getGeometryCommandType()
    {
        return FITKGeoEnum::FITKGeometryComType::FGTTransformByTwoPoints;
    }

    FITKGeoEnum::FITKGeometryComType FITKAbsGeoModelTransformByDirAndDis::getGeometryCommandType()
    {
        return FITKGeoEnum::FITKGeometryComType::FGTTransformByDirAndDis;
    }

    FITKGeoEnum::FITKGeometryComType FITKAbsGeoModelRotate::getGeometryCommandType()
    {
        return FITKGeoEnum::FITKGeometryComType::FGTRotate;
    }

    FITKGeoEnum::FITKGeometryComType FITKAbsGeoModelScale::getGeometryCommandType()
    {
        return FITKGeoEnum::FITKGeometryComType::FGTScale;
    }
    FITKGeoEnum::FITKGeometryComType FITKAbsGeoModelMirror::getGeometryCommandType()
    {
        return FITKGeoEnum::FITKGeometryComType::FGTMirror;
    }
    FITKGeoEnum::FITKGeometryComType FITKAbsGeoModelPattern::getGeometryCommandType()
    {
        return FITKGeoEnum::FITKGeometryComType::FGTPattern;
    }
    FITKGeoEnum::FITKGeometryComType FITKAbsGeoModelRectangularPattern::getGeometryCommandType()
    {
        return FITKGeoEnum::FITKGeometryComType::FGTRectangularPattern;
    }
    FITKGeoEnum::FITKGeometryComType FITKAbsGeoModelCircularPattern::getGeometryCommandType()
    {
        return FITKGeoEnum::FITKGeometryComType::FGTCircularPattern;
    }
}
