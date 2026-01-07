/**********************************************************************
 * @file   ViewAdaptorBase.h
 * @brief  渲染适配器基类
 * @author BaGuijun (baguijun@163.com)
 * @date   2025-03-12
 *********************************************************************/
#ifndef _ViewAdaptorBase_H
#define _ViewAdaptorBase_H

#include "GraphDataAdaptorAPI.h"
#include "FITK_Kernel/FITKAdaptor/FITKAbstractViewAdaptor.h"
#include "FITK_Kernel/FITKCore/FITKVarientParams.h"

namespace Graph
{
    class GraphObjectBase;
    /**
     * @brief  渲染适配器基类
     * @author BaGuijun (baguijun@163.com)
     * @date   2025-03-12
     */
    class GraphDataAdaptorAPI ViewAdaptorBase :public Adaptor::FITKAbstractViewAdaptor, public Core::FITKVarientParams
    {
    public:
        /**
         * @brief    构造函数
         * @author   BaGuijun (baguijun@163.com)
         * @date     2025-03-12
         */
        ViewAdaptorBase() = default;
        /**
         * @brief    析构函数
         * @author   BaGuijun (baguijun@163.com)
         * @date     2025-03-12
         */
        virtual ~ViewAdaptorBase() = default;
        /**
         * @brief    获取渲染对象
         * @return   GraphObjectBase*          渲染对象
         * @author   BaGuijun (baguijun@163.com)
         * @date     2025-03-12
         */
        GraphObjectBase* getOutputData();
    protected:
        /**
         * @brief  渲染数据
         * @author BaGuijun (baguijun@163.com)
         * @date   2025-03-12
         */
        GraphObjectBase* _outputData = nullptr;
    };
}

#endif