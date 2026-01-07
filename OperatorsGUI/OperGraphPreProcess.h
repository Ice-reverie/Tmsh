/**********************************************************************
 * @file   OperGraphPreProcess.h
 * @brief  前处理可视化事件操作器。
 * @author BaGuijun (baguijun@163.com)
 * @date   2025-03-12
 *********************************************************************/
#ifndef _OPERGRAPHPREPROCESS_H__
#define _OPERGRAPHPREPROCESS_H__

#include "OperGraphEvent3D.h"

#include "FITK_Kernel/FITKCore/FITKOperatorRepo.h"

namespace GUIOper
{
    /**
     * @brief  前处理可视化事件操作器。
     * @author BaGuijun (baguijun@163.com)
     * @date   2025-03-12
     */
    class OperGraphPreprocess : public OperGraphEvent3D
    {
        Q_OBJECT;
    public:
        /**
         * @brief    构造函数
         * @author   BaGuijun (baguijun@163.com)
         * @date     2025-03-12
         */
        explicit OperGraphPreprocess() = default;
        /**
         * @brief    构造函数
         * @author   BaGuijun (baguijun@163.com)
         * @date     2025-03-12
         */
        ~OperGraphPreprocess() = default;
        /**
         * @brief    更新全部渲染
         * @author   BaGuijun (baguijun@163.com)
         * @date     2025-04-16
         */
        void updateAllGraph() override;
        /**
         * @brief    刷新渲染数据
         * @param[i] dataObjId                数据id
         * @param[i] forceUpdate              是否强制刷新（默认否）
         * @author   BaGuijun (baguijun@163.com)
         * @date     2025-03-12
         */
        void updateGraph(int dataObjId, bool forceUpdate = false) override;
        /**
         * @brief    更新部分颜色
         * @param[i] dataObjId                对象id
         * @param[i] color                    颜色
         * @param[i] otherPar                 其他参数
         * @author   BaGuijun (baguijun@163.com)
         * @date     2025-03-28
         */
        void updateColor(int dataObjId, QColor color, QVariant otherPar = QVariant()) override;
        /**
         * @brief    更新整体颜色
         * @param[i] dataObjId                对象id
         * @param[i] color                    颜色
         * @param[i] forceUpdate              是否强制刷新（默认否）
         * @author   ZhouZhaoYi (zhouzhaoyi@diso.cn)
         * @date     2025-03-28
         */
        void updateColor(int dataObjId, QColor color = QColor(), bool forceUpdate = false) override;
    };

    Register2FITKOperatorRepo(GraphPreprocess, OperGraphPreprocess);

}  // namespace GUIOper

#endif
