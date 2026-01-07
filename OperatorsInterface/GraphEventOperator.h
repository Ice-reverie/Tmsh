/**********************************************************************
 * @file   GraphEventOperator.h
 * @brief  三维可视对象数据操作器接口类
 * @author BaGuijun (baguijun@163.com)
 * @date   2025-03-12
 *********************************************************************/
#ifndef __GRAPHEVENTOPERATOR_H__
#define __GRAPHEVENTOPERATOR_H__

#include "FITK_Kernel/FITKCore/FITKAbstractOperator.h"
#include "OperatorsInterfaceAPI.h"
#include <QColor>
#include <QVariant>

namespace Graph {
    class GraphObjectBase;
}

namespace EventOper
{
    /**
     * @brief  三维可视对象数据操作器接口类。
     * @author BaGuijun (baguijun@163.com)
     * @date   2025-03-12
     */
    class OperatorsInterfaceAPI GraphEventOperator : public Core::FITKAbstractOperator
    {
        Q_OBJECT;
    public:
        /**
         * @brief    构造函数
         * @author   BaGuijun (baguijun@163.com)
         * @date     2025-03-12
         */
        explicit GraphEventOperator() = default;
        /**
         * @brief    析构函数
         * @author   BaGuijun (baguijun@163.com)
         * @date     2025-03-12
         */
        virtual ~GraphEventOperator() = default;
        /**
         * @brief    更新所有渲染
         * @author   BaGuijun (baguijun@163.com)
         * @date     2025-04-16
         */
        virtual void updateAllGraph();
        /**
         * @brief    刷新渲染数据
         * @param[i] dataObjId                数据id
         * @param[i] forceUpdate              是否强制刷新（默认否）
         * @author   BaGuijun (baguijun@163.com)
         * @date     2025-03-12
         */
        virtual void updateGraph(int dataObjId, bool forceUpdate = false);
        /**
         * @brief    更新部分颜色
         * @param[i] dataObjId                对象id
         * @param[i] color                    颜色
         * @param[i] otherPar                 其他参数
         * @author   BaGuijun (baguijun@163.com)
         * @date     2025-03-28
         */
        virtual void updateColor(int dataObjId, QColor color, QVariant otherPar = QVariant());
        /**
         * @brief    更新全部颜色
         * @param[i] dataObjId                对象id
         * @param[i] color                    颜色
         * @param[i] forceUpdate              是否强制刷新（默认否）
         * @author   ZhouZhaoYi (zhouzhaoyi@diso.cn)
         * @date     2025-03-28
         */
        virtual void updateColor(int dataObjId, QColor color = QColor(), bool forceUpdate = false);
        /**
         * @brief    根据数据对象ID获取模型可视化对象。（没有不会创建）[虚函数]
         * @param[i] dataObjId                            数据id
         * @return   Exchange::GraphVTKObject3D*          渲染对象
         * @author   BaGuijun (baguijun@163.com)
         * @date     2024-12-26
         */
        virtual Graph::GraphObjectBase* getModelGraphObjectByDataId(int dataObjId);
        /**
         * @brief    根据提供的数据对象ID递归更新对应可视化对象。（没有则不创建）
         * @param[i] dataObjId                                 数据对象ID
         * @param[i] refLayer                                  更新引用数据的层级（-1则递归至根数据）[缺省]
         * @param[i] recDown                                   向下递归（被引用方向查询）[缺省]
         * @author   BaGuijun (baguijun@163.com)
         * @date     2025-04-02
         */
        virtual void updateGraphRecursively(int dataObjId, int refLayer = 2, bool recDown = true);
        /**
         * @brief   刷新渲染窗口。[重写]
         * @author  ChengHaotian (yeguangbaozi@foxmail.com)
         * @date    2024-07-24
         */
        virtual void reRender();
        /**
         * @brief    自适应
         * @author   BaGuijun (baguijun@163.com)
         * @date     2025-03-06
         */
        virtual void fitView();
    };
}
#endif