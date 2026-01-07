/**********************************************************************
 * @file   ViewAdaptorMesh.h
 * @brief  网格渲染适配器
 * @author BaGuijun (baguijun@163.com)
 * @date   2025-04-10
 *********************************************************************/
#ifndef ViewAdaptorGeoModel_H
#define ViewAdaptorGeoModel_H

#include "GraphDataAdaptorAPI.h"
#include "ViewAdaptorBase.h"

#include "FITK_Kernel/FITKAdaptor/FITKViewAdaptorFactory.h"

namespace Graph
{
    /**
     * @brief  网格渲染适配器
     * @author BaGuijun (baguijun@163.com)
     * @date   2025-03-12
     */
    class GraphDataAdaptorAPI ViewAdaptorMesh : public ViewAdaptorBase
    {
    public:
        /**
         * @brief    构造函数
         * @author   BaGuijun (baguijun@163.com)
         * @date     2025-03-12
         */
        ViewAdaptorMesh() = default;
        /**
         * @brief    析构函数
         * @author   BaGuijun (baguijun@163.com)
         * @date     2025-03-12
         */
        virtual ~ViewAdaptorMesh() = default;
        /**
         * @brief    适配
         * @return   bool                是否成功
         * @author   BaGuijun (baguijun@163.com)
         * @date     2025-03-12
         */
        bool adapt() override;
    };

    //注册适配器
    Register2FITKViewAdaptorFactory(MeshKernel, ModelData::MeshKernel, ViewAdaptorMesh);
}
#endif
