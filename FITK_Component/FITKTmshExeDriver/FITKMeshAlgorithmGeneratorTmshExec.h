/**********************************************************************
 * @file   FITKMeshAlgorithmGeneratorTmshExec.h
 * @brief  网格算法生成器
 * @date   2025-12-22
 *********************************************************************/
#ifndef _FITKMESHALGORITHMGENERATORTMSHEXEC_H___
#define _FITKMESHALGORITHMGENERATORTMSHEXEC_H___

#include "FITK_Interface/FITKInterfaceMeshGen/FITKAbstractMeshAlgorithmInfoGenerator.h"

namespace Tmsh
{
    class FITKMeshAlgorithmGeneratorTmshExec :
        public Interface::FITKAbstractMeshAlgorithmInfoGenerator
    {
    public:
        explicit FITKMeshAlgorithmGeneratorTmshExec() = default;
        virtual ~FITKMeshAlgorithmGeneratorTmshExec() = default;

        virtual Interface::FITKGlobalMeshGenerateAlgorithmInfo* generateGlobalMeshAlgorithmInfo() override;
    };
}

#endif

