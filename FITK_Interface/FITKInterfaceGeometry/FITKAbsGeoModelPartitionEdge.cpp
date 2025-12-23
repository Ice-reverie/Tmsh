#include "FITKAbsGeoModelPartitionEdge.h" 
#include "FITK_Interface/FITKInterfaceModel/FITKAbstractModel.h"

namespace Interface
{
    FITKGeoEnum::FITKGeometryComType FITKAbsGeoModelPartitionEdgeWithParameter::getGeometryCommandType()
    {
        return FITKGeoEnum::FITKGeometryComType::FGTPartitionEdgeWithParameter;
    }

    FITKGeoEnum::FITKGeometryComType FITKAbsGeoModelPartitionEdgeWithPoint::getGeometryCommandType()
    {
        return FITKGeoEnum::FITKGeometryComType::FGTPartitionEdgeWithPoint;
    }

    FITKGeoEnum::FITKGeometryComType FITKAbsGeoModelPartitionEdgeWithDatumPlane::getGeometryCommandType()
    {
        return FITKGeoEnum::FITKGeometryComType::FGTPartitionEdgeWithDatumPlane;
    }

}
