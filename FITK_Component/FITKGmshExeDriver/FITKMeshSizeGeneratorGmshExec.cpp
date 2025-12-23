#include "FITKMeshSizeGeneratorGmshExec.h"

namespace Gmsh
{
    Interface::FITKGlobalMeshSizeInfo* FITKMeshSizeGeneratorGmshExec::generateGlobalMeshSizeInfo()
    {
        //默认使用父类函数
        return Interface::FITKAbstractMeshSizeInfoGenerator::generateGlobalMeshSizeInfo();
    }
    Interface::FITKAbstractRegionMeshSize* FITKMeshSizeGeneratorGmshExec::createRegionMeshSize(Interface::FITKAbstractRegionMeshSize::RegionType t)
    {
        //默认使用父类函数
        return Interface::FITKAbstractMeshSizeInfoGenerator::createRegionMeshSize(t);
    }
}




