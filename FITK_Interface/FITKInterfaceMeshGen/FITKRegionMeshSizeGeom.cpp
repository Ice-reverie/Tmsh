#include "FITKRegionMeshSizeGeom.h"


namespace Interface
{
    FITKAbstractRegionMeshSize::RegionType FITKRegionMeshSizeGeom::getRegionType()
    {
        /** Geometry region type */
        return FITKAbstractRegionMeshSize::RigonGeom;
    }

    void FITKRegionMeshSizeGeom::setGeomID(const int id)
    {
        // Set geometry ID
        _geoID = id;
    }

    int FITKRegionMeshSizeGeom::getGeomID() const
    {
        /** Geometry ID */
        return _geoID;
    }

}



