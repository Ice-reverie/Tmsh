/**********************************************************************
 * @file   FITKMeshGenGmshExecInterface.h
 * @brief  Gmsh exe网格生成
 * @author liuzhonghua (liuzhonghuaszch@163.com)
 * @date   2025-04-21
 *********************************************************************/
#ifndef _FITKMESHGENGMSHEXECINTERFACE_H_
#define _FITKMESHGENGMSHEXECINTERFACE_H_

#include "FITKGmshExeDriverAPI.h"
#include "FITK_Kernel/FITKAppFramework/FITKComponentInterface.h"

namespace Gmsh
{
    /**
     * @brief Gmsh网格生成
     * @author liuzhonghua (liuzhonghuaszch@163.com)
     * @date 2024-03-04
     */
    class FITKGmshExeDriverAPI FITKMeshGenGmshExecInterface :
        public AppFrame::FITKComponentInterface
    {
    public:
        /**
         * @brief Construct a new FITKMeshGenOFInterface object
         * @author liuzhonghua (liuzhonghuaszch@163.com)
         * @date 2024-07-17
         */
        explicit FITKMeshGenGmshExecInterface();
        /**
         * @brief Destroy the FITKMeshGenOFInterface object
         * @author liuzhonghua (liuzhonghuaszch@163.com)
         * @date 2024-07-17
         */
        virtual ~FITKMeshGenGmshExecInterface() = default;

        /**
        * @brief 获取部件名称，不能重复  return "FITKMeshGenOF"
        * @return QString
        * @author liuzhonghua (liuzhonghuaszch@163.com)
        * @date 2024-03-04
        */
        virtual QString getComponentName() override;

        /**
         * @brief 执行读取或写出
         * @author liuzhonghua (liuzhonghuaszch@163.com)
         * @date 2024-03-07
         */
        virtual bool exec(const int indexPort) override;

        /**
         * @brief       获取组件内不需要重置的数据对象ID。[虚函数][重写]
         * @return      ID列表
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2025-10-28
         */
        virtual QList<int> getUnResetDataObj() override;

    };
}


#endif
