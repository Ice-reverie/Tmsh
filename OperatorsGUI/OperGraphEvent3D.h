/**
 * @brief  三维可视化事件操作器抽象类。
 * @author BaGuijun (baguijun@163.com)
 * @date   2025-03-12
 */
#ifndef __OPERGRAPHEVENT3D_H__
#define __OPERGRAPHEVENT3D_H__

#include "OperatorsInterface/GraphEventOperator.h"

// 前置声明
namespace Comp{
    class FITKGraph3DWindowVTK;
}

namespace Graph{
    class GraphObjectBase;
}

namespace GUIOper
{
    /**
     * @brief  可视化事件操作器
     * @author BaGuijun (baguijun@163.com)
     * @date   2025-03-12
     */
    class OperGraphEvent3D : public EventOper::GraphEventOperator
    {
        Q_OBJECT;
    protected:
        /**
         * @brief    构造函数
         * @author   BaGuijun (baguijun@163.com)
         * @date     2025-03-12
         */
        explicit OperGraphEvent3D() = default;
        /**
         * @brief   析构函数。[虚函数]
         * @author   BaGuijun (baguijun@163.com)
         * @date     2025-03-12
         */
        virtual ~OperGraphEvent3D() = default;
        /**
         * @brief    获取三维渲染窗口
         * @return   Comp::FITKGraph3DWindowVTK*               三维渲染窗口
         * @author   BaGuijun (baguijun@163.com)
         * @date     2025-03-05
         */
        Comp::FITKGraph3DWindowVTK* getPreGraphWidget();
        /**
         * @brief    添加可视化对象至三维窗口
         * @param[i] obj                       可视化对象
         * @param[i] graphWidget               三维窗口
         * @param[i] fitView                   是否自适应
         * @author   BaGuijun (baguijun@163.com)
         * @date     2025-03-12
         */
        void addGraphObjectToWidget(Graph::GraphObjectBase* obj, Comp::FITKGraph3DWindowVTK* graphWidget, bool fitView = false);
        /**
         * @brief    获取渲染对象
         * @param[i] dataObjId                             数据id
         * @param[i] forceUpdate                           是否强制刷新
         * @return   QList<Graph::GraphObjectBase*>        渲染对象列表
         * @author   BaGuijun (baguijun@163.com)
         * @date     2025-03-12
         */
        QList<Graph::GraphObjectBase*> getGraphObjectsByDataId(int dataObjId, bool forceUpdate);
        /**
         * @brief    获取所有几何渲染对象（会自动创建渲染对象）
         * @return   QList<Graph::GraphObjectBase*>        渲染对象列表
         * @author   BaGuijun (baguijun@163.com)
         * @date     2025-04-16
         */
        QList<Graph::GraphObjectBase*> getGeometryGraphs();
        /**
         * @brief    获取所有网格渲染对象（会自动创建渲染对象）
         * @return   QList<Graph::GraphObjectBase*>        渲染对象列表
         * @author   BaGuijun (baguijun@163.com)
         * @date     2025-04-16
         */
        QList<Graph::GraphObjectBase*> getMeshKernelGraphs();
        /**
         * @brief    根据提供的数据对象ID递归更新对应可视化对象。（没有则不创建）
         * @param[i] dataObjId                                 数据对象ID
         * @param[i] refLayer                                  更新引用数据的层级（-1则递归至根数据）[缺省]
         * @param[i] recDown                                   向下递归（被引用方向查询）[缺省]
         * @author   BaGuijun (baguijun@163.com)
         * @date     2025-04-02
         */
        void updateGraphRecursively(int dataObjId, int refLayer = 2, bool recDown = true) override;
        /**
         * @brief   刷新渲染窗口。[重写]
         * @author  ChengHaotian (yeguangbaozi@foxmail.com)
         * @date    2024-07-24
         */
        void reRender() override;
        /**
         * @brief    自适应
         * @author   BaGuijun (baguijun@163.com)
         * @date     2025-03-06
         */
        void fitView() override;
    };

}  // namespace GUIOper

#endif // __OPERGRAPHEVENT3D_H__
