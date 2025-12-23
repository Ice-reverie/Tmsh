#include "FITKAbsGeoModelPartitionFace.h" 
#include "FITK_Interface/FITKInterfaceModel/FITKAbstractModel.h"

namespace Interface
{
    FITKGeoEnum::FITKGeometryComType FITKAbsGeoModelPartitionFaceWithSketch::getGeometryCommandType()
    {
        return FITKGeoEnum::FGTPartitionFaceWithSketch;
    }

    FITKGeoEnum::FITKGeometryComType FITKAbsGeoModelPartitionFaceWithTwoPoints::getGeometryCommandType()
    {
        return FITKGeoEnum::FGTPartitionFaceWithTwoPoints;
    }

    FITKGeoEnum::FITKGeometryComType FITKAbsGeoModelPartitionFaceWithDatumPlane::getGeometryCommandType()
    {
        return FITKGeoEnum::FGTPartitionFaceWithDatumPlane;
    }

    FITKGeoEnum::FITKGeometryComType FITKAbsGeoModelPartitionFaceWithCurvedPath::getGeometryCommandType()
    {
        return FITKGeoEnum::FGTPartitionFaceWithCurvedPath;
    }

    FITKGeoEnum::FITKGeometryComType FITKAbsGeoModelPartitionFaceWithExtendFace::getGeometryCommandType()
    {
        return FITKGeoEnum::FGTPartitionFaceWithExtendFace;
    }

    FITKGeoEnum::FITKGeometryComType FITKAbsGeoModelPartitionFaceWithIntersectFace::getGeometryCommandType()
    {
        return FITKGeoEnum::FGTPartitionFaceWithIntersectFace;
    }

    FITKGeoEnum::FITKGeometryComType FITKAbsGeoModelPartitionFaceWithProjectEdges::getGeometryCommandType()
    {
        return FITKGeoEnum::FGTPartitionFaceWithProjectEdges;
    }

}
