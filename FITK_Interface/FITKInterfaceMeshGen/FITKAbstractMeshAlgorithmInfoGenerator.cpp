#include "FITKAbstractMeshAlgorithmInfoGenerator.h"
#include "FITKGlobalMeshGenerateAlgorithmInfo.h"


namespace Interface
{
    FITKAbstractMeshAlgorithmInfoGenerator::~FITKAbstractMeshAlgorithmInfoGenerator()
    {
        //纯虚函数
    }

    FITKGlobalMeshGenerateAlgorithmInfo* FITKAbstractMeshAlgorithmInfoGenerator::generateGlobalMeshAlgorithmInfo()
    {
        //默认值
        return new FITKGlobalMeshGenerateAlgorithmInfo;
    }

}


