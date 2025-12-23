/**********************************************************************
 * @file   FITKMeshSizeGeneratorTmshExec.h
 * @brief  网格尺寸生成器
 * @date   2025-12-22
 *********************************************************************/
#ifndef _FITKMESHSIZEGENERATORTMSHEXEC_H___
#define _FITKMESHSIZEGENERATORTMSHEXEC_H___

#include "FITK_Interface/FITKInterfaceMeshGen/FITKAbstractMeshSizeInfoGenerator.h"

namespace Tmsh
{
    class FITKMeshSizeGeneratorTmshExec :
        public Interface::FITKAbstractMeshSizeInfoGenerator
    {
    public:
        explicit FITKMeshSizeGeneratorTmshExec() = default;
        virtual ~FITKMeshSizeGeneratorTmshExec() = default;

        virtual Interface::FITKGlobalMeshSizeInfo* generateGlobalMeshSizeInfo() override;
        virtual Interface::FITKAbstractRegionMeshSize* createRegionMeshSize(Interface::FITKAbstractRegionMeshSize::RegionType t) override;
    };
}

#endif

