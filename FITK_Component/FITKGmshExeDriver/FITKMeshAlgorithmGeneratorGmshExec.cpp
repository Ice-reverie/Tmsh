#include "FITKMeshAlgorithmGeneratorGmshExec.h"

namespace Gmsh
{
    Interface::FITKGlobalMeshGenerateAlgorithmInfo* FITKMeshAlgorithmGeneratorGmshExec::generateGlobalMeshAlgorithmInfo()
    {
        //默认使用父类函数
        return Interface::FITKAbstractMeshAlgorithmInfoGenerator::generateGlobalMeshAlgorithmInfo();
    }
}




