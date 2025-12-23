#include "FITKAbstractMeshSizeInfoGenerator.h"
#include "FITKGlobalMeshSizeInfo.h"
#include "FITKRegionMeshSizeBox.h"
#include "FITKRegionMeshSizeCylinder.h"
#include "FITKRegionMeshSizeSphere.h"
#include "FITKRegionMeshSizeGeom.h"


namespace Interface
{
    FITKAbstractMeshSizeInfoGenerator::~FITKAbstractMeshSizeInfoGenerator()
    {
        //纯虚函数
    }

    FITKGlobalMeshSizeInfo* FITKAbstractMeshSizeInfoGenerator::generateGlobalMeshSizeInfo()
    {
        //默认值
        return new FITKGlobalMeshSizeInfo;
    }

    FITKAbstractRegionMeshSize* FITKAbstractMeshSizeInfoGenerator::createRegionMeshSize(FITKAbstractRegionMeshSize::RegionType t)
    {
        //生成默认值
        switch (t)
        {
        case Interface::FITKAbstractRegionMeshSize::RegionBox:
            return new FITKRegionMeshSizeBox;
        case Interface::FITKAbstractRegionMeshSize::RegionCylinder:
            return new FITKRegionMeshSizeCylinder;
        case Interface::FITKAbstractRegionMeshSize::RegionSphere:
            return new FITKRegionMeshSizeSphere;
        case  FITKAbstractRegionMeshSize::RigonGeom:
            return new FITKRegionMeshSizeGeom;
        default: return nullptr;
        }
        return nullptr;
    }

}


