/**********************************************************************
 * @file   FITKMeshSizeGeneratorGmshExec.h
 * @brief  网格尺寸生成器
 * @author liuzhonghua (liuzhonghuaszch@163.com)
 * @date   2024-10-12
 *********************************************************************/
#ifndef _FITKMESHSIZEGENERATORGMSHEXEC_H___
#define _FITKMESHSIZEGENERATORGMSHEXEC_H___


#include "FITK_Interface/FITKInterfaceMeshGen/FITKAbstractMeshSizeInfoGenerator.h"

namespace Gmsh
{
    /**
     * @brief  网格尺寸生成器
     * @author liuzhonghua (liuzhonghuaszch@163.com)
     * @date   2024-10-12
     */
    class FITKMeshSizeGeneratorGmshExec :
        public Interface::FITKAbstractMeshSizeInfoGenerator
    {
    public:
        /**
         * @brief Construct a new FITKMeshSizeGeneratorGmshExec object
         * @author liuzhonghua (liuzhonghuaszch@163.com)
         * @date   2024-10-12
         */
        explicit FITKMeshSizeGeneratorGmshExec() = default;
        /**
         * @brief Destroy the FITKMeshSizeGeneratorGmshExec object
         * @author liuzhonghua (liuzhonghuaszch@163.com)
         * @date   2024-10-12
         */
        virtual ~FITKMeshSizeGeneratorGmshExec() = default;
        /**
        * @brief 生成全局尺寸信息
        * @return FITKGlobalMeshSizeInfo*
        * @author liuzhonghua (liuzhonghuaszch@163.com)
         * @date   2024-10-12
        */
        virtual Interface::FITKGlobalMeshSizeInfo* generateGlobalMeshSizeInfo() override;
        /**
         * @brief 生成局部区域网格尺寸
         * @param[i]  t              区域类型
         * @return FITKAbstractRegionMeshSize*
         * @author liuzhonghua (liuzhonghuaszch@163.com)
         * @date   2024-10-12
         */
        virtual Interface::FITKAbstractRegionMeshSize* createRegionMeshSize(Interface::FITKAbstractRegionMeshSize::RegionType t) override;
    };
}


#endif
