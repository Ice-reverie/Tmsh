#include "FITKAbsGeoModelCut.h" 
#include "FITK_Interface/FITKInterfaceModel/FITKAbstractModel.h"

namespace Interface
{
    FITKGeoEnum::FITKGeometryComType FITKAbsGeoModelExtrudeCut::getGeometryCommandType()
    {
        return FITKGeoEnum::FITKGeometryComType::FGTExtrudeCut;
    }
    FITKGeoEnum::FITKGeometryComType FITKAbsGeoModelRevolCut::getGeometryCommandType()
    {
        return FITKGeoEnum::FITKGeometryComType::FGTRevolCut;
    }
    FITKGeoEnum::FITKGeometryComType FITKAbsGeoModelSweepCut::getGeometryCommandType()
    {
        return FITKGeoEnum::FITKGeometryComType::FGTSweepCut;
    }

    FITKGeoEnum::FITKGeometryComType FITKAbsGeoModelMultiSectionCut::getGeometryCommandType()
    {
        return FITKGeoEnum::FITKGeometryComType::FGTMultiSectionCut;
    }
}
