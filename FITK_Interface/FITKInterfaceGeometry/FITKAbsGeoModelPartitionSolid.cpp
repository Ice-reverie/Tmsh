#include "FITKAbsGeoModelPartitionSolid.h" 
#include "FITK_Interface/FITKInterfaceModel/FITKAbstractModel.h"

namespace Interface
{

    FITKGeoEnum::FITKGeometryComType FITKAbsGeoModelPartitionSolidWithPlane::getGeometryCommandType()
    {
        return FITKGeoEnum::FGTPartitionSolidWithPlane;
    }

    FITKGeoEnum::FITKGeometryComType FITKAbsGeoModelPartitionSolidWithExtendFace::getGeometryCommandType()
    {
        return FITKGeoEnum::FGTPartitionSolidWithExtendFace;
    }

    FITKGeoEnum::FITKGeometryComType FITKAbsGeoModelPartitionSolidWithSweepEdge::getGeometryCommandType()
    {
        return FITKGeoEnum::FGTPartitionSolidWithSweepEdge;
    }

    FITKGeoEnum::FITKGeometryComType FITKAbsGeoModelPartitionSolidWithNSidedPatch::getGeometryCommandType()
    {
        return FITKGeoEnum::FGTPartitionSolidWithNSidedPatch;
    }

    FITKGeoEnum::FITKGeometryComType FITKAbsGeoModelPartitionSolidWithSketchPlanar::getGeometryCommandType()
    {
        return FITKGeoEnum::FGTPartitionSolidWithSketchPlanar;
    }

}
