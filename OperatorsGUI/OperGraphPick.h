/*****************************************************************//**
 * @file    OperGraphPick.h
 * @brief   拾取高亮操作器。
 *
 * @author  ChengHaotian (yeguangbaozi@foxmail.com)
 * @date    2024-11-01
 *********************************************************************/

#ifndef __OPERGRAPHPICK_H__
#define __OPERGRAPHPICK_H__

#include "OperatorsInterface/GraphInteractionOperator.h"

#include "FITK_Kernel/FITKCore/FITKOperatorRepo.h"

namespace Graph
{
    class GraphObjectBase;
    class GraphPickProvider;
    class GraphObjectPick;
}

namespace GUIOper
{
    /**
     * @brief   高亮可视化事件处理。
     * @author  ChengHaotian (yeguangbaozi@foxmail.com)
     * @date    2024-11-01
     */
    class OperGraphPick : public EventOper::GraphInteractionOperator
    {
    public:
        /**
         * @brief   构造函数。
         * @author  ChengHaotian (yeguangbaozi@foxmail.com)
         * @date    2024-11-01
         */
        explicit OperGraphPick();

        /**
         * @brief   析构函数。
         * @author  ChengHaotian (yeguangbaozi@foxmail.com)
         * @date    2024-11-01
         */
        ~OperGraphPick();

        /**
         * @brief   拾取高亮单个节点或单元接口。[重写]
         * @param   graphWindow：可视化窗口
         * @param   actor：演员
         * @param   index：索引
         * @param   pickedWorldPos：拾取的世界坐标
         * @author  ChengHaotian (yeguangbaozi@foxmail.com)
         * @date    2024-11-01
         */
        void picked(Comp::FITKGraph3DWindowVTK* graphWindow, vtkActor* actor, int index, double* pickedWorldPos) override;
        /**
         * @brief   框选拾取多个节点或单元接口。[重写]
         * @param   graphWindow：可视化窗口
         * @param   actors：被框选到的演员列表
         * @param   pickPlanes：框选时的裁切平面
         * @author  ChengHaotian (yeguangbaozi@foxmail.com)
         * @date    2024-11-02
         */
        void picked(Comp::FITKGraph3DWindowVTK* graphWindow, QList<vtkActor*> actors, vtkPlanes* pickPlanes) override;

        /**
         * @brief   清除预选高亮对象渲染。[重写]
         * @param   graphWindow：可视化窗口[缺省]
         * @author  ChengHaotian (yeguangbaozi@foxmail.com)
         * @date    2024-11-01
         */
        void clear(Comp::FITKGraph3DWindowVTK* graphWindow = nullptr) override;

        /**
         * @brief   根据当前拾取对象类型隐藏或显示部分演员（特征边）。[重写]
         * @param   pickObjType：拾取对象类型
         * @param   pickMethod：拾取方法
         * @param   dataObjId：生效数据ID（-1则对所有数据生效）[缺省]
         * @author  ChengHaotian (yeguangbaozi@foxmail.com)
         * @date    2024-11-01
         */
        void setActorStateByPickInfo(int pickObjType, int pickMethod, int dataObjId = -1) override;

    private:
        /**
         * @brief   将三维对象添加至渲染窗口。
         * @param   obj：三维可视化对象
         * @param   graphWindow：渲染窗口
         * @author  ChengHaotian (yeguangbaozi@foxmail.com)
         * @date    2024-11-01
         */
        void addGraphObjToRenderWindow(Graph::GraphObjectBase* obj, Comp::FITKGraph3DWindowVTK* graphWindow);

        /**
         * @brief   获取当前激活三维窗口。
         * @return  三维窗口
         * @author  ChengHaotian (yeguangbaozi@foxmail.com)
         * @date    2024-11-01
         */
        Comp::FITKGraph3DWindowVTK* getCurrentGraphWindow();
    private:
        /**
         * @brief  拾取高亮可视化对象
         * @author BaGuijun (baguijun@163.com)
         * @date   2024-12-26
         */
        Graph::GraphObjectPick* _graphObjPick = nullptr;
        /**
         * @brief   拾取信息管理器。
         * @author  ChengHaotian (yeguangbaozi@foxmail.com)
         * @date    2024-11-01
         */
        Graph::GraphPickProvider* _pickedProvider{ nullptr };
    };

    Register2FITKOperatorRepo(GraphPick, OperGraphPick);

}  // namespace GUIOper

#endif // __OPERGRAPHPICK_H__
