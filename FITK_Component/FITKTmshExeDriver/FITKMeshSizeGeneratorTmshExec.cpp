#include "FITKMeshSizeGeneratorTmshExec.h"

namespace Tmsh
{
    Interface::FITKGlobalMeshSizeInfo* FITKMeshSizeGeneratorTmshExec::generateGlobalMeshSizeInfo()
    {
        return Interface::FITKAbstractMeshSizeInfoGenerator::generateGlobalMeshSizeInfo();
    }

    Interface::FITKAbstractRegionMeshSize* FITKMeshSizeGeneratorTmshExec::createRegionMeshSize(Interface::FITKAbstractRegionMeshSize::RegionType t)
    {
        return Interface::FITKAbstractMeshSizeInfoGenerator::createRegionMeshSize(t);
    }
}

