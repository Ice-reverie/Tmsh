/*****************************************************************//**
 * @file    GraphInteractionOperator.h
 * @brief   三维拾取操作器接口抽象类。
 *
 * @author  ChengHaotian (yeguangbaozi@foxmail.com)
 * @date    2024-10-31
 *********************************************************************/

#ifndef __GRAPHINTERACTIONOPERATOR_H__
#define __GRAPHINTERACTIONOPERATOR_H__

#include "FITK_Kernel/FITKCore/FITKAbstractOperator.h"

#include "OperatorsInterfaceAPI.h"

#include <QColor>

 // 前置声明
class vtkActor2D;
class vtkActor;
class vtkImageActor;
class vtkPlanes;

namespace Comp
{
    class FITKGraph3DWindowVTK;
}

namespace EventOper
{
    /**
     * @brief   三维拾取操作器接口抽象类。
     * @author  ChengHaotian (yeguangbaozi@foxmail.com)
     * @date    2024-10-31
     */
    class OperatorsInterfaceAPI GraphInteractionOperator : public Core::FITKAbstractOperator
    {
        Q_OBJECT

    public:
        /**
         * @brief   构造函数。
         * @author  ChengHaotian (yeguangbaozi@foxmail.com)
         * @date    2024-10-31
         */
        explicit GraphInteractionOperator() = default;

        /**
         * @brief   析构函数。[虚函数]
         * @author  ChengHaotian (yeguangbaozi@foxmail.com)
         * @date    2024-10-31
         */
        virtual ~GraphInteractionOperator() = default;

        /**
         * @brief   拾取单个文本标签对象接口。[虚函数]
         * @param   graphWindow：可视化窗口
         * @param   actor：演员
         * @author  ChengHaotian (yeguangbaozi@foxmail.com)
         * @date    2024-10-31
         */
        virtual void picked(Comp::FITKGraph3DWindowVTK* graphWindow, vtkActor2D* actor);

        /**
         * @brief   拾取单个图片标签接口。[虚函数]
         * @param   graphWindow：可视化窗口
         * @param   actor：演员
         * @author  ChengHaotian (yeguangbaozi@foxmail.com)
         * @date    2024-11-07
         */
        virtual void picked(Comp::FITKGraph3DWindowVTK* graphWindow, vtkImageActor* actor);

        /**
         * @brief   拾取单个节点或单元接口。[虚函数]
         * @param   graphWindow：可视化窗口
         * @param   actor：演员
         * @param   index：索引
         * @param   pickedWorldPos：拾取的世界坐标
         * @author  ChengHaotian (yeguangbaozi@foxmail.com)
         * @date    2024-10-31
         */
        virtual void picked(Comp::FITKGraph3DWindowVTK* graphWindow, vtkActor* actor, int index, double* pickedWorldPos);

        /**
         * @brief   框选拾取多个节点或单元接口。[虚函数]
         * @param   graphWindow：可视化窗口
         * @param   actors：被框选到的演员列表
         * @param   pickPlane：框选时的裁切平面
         * @author  ChengHaotian (yeguangbaozi@foxmail.com)
         * @date    2024-11-02
         */
        virtual void picked(Comp::FITKGraph3DWindowVTK* graphWindow, QList<vtkActor*> actors, vtkPlanes* pickPlane);

        /**
         * @brief   双击左键拾取对象接口。[虚函数]
         * @param   actor：演员
         * @param   index：索引
         * @author  ChengHaotian (yeguangbaozi@foxmail.com)
         * @date    2024-11-04
         */
        virtual void doublePicked(vtkActor* actor, int index);

        /**
         * @brief   双击左键拾取2D图像接口。[虚函数]
         * @param   actor：演员
         * @author  ChengHaotian (yeguangbaozi@foxmail.com)
         * @date    2024-11-07
         */
        virtual void doublePicked(vtkActor2D* actor);

        /**
         * @brief   双击左键拾取图片对象接口。[虚函数]
         * @param   actor：演员
         * @author  ChengHaotian (yeguangbaozi@foxmail.com)
         * @date    2024-11-07
         */
        virtual void doublePicked(vtkImageActor* actor);

        /**
         * @brief   清除拾取高亮渲染。[虚函数]
         * @param   graphWindow：可视化窗口[缺省]
         * @author  ChengHaotian (yeguangbaozi@foxmail.com)
         * @date    2024-10-31
         */
        virtual void clear(Comp::FITKGraph3DWindowVTK* graphWindow = nullptr);

        /**
         * @brief   设置拾取颜色。[虚函数]
         * @param   color：颜色
         * @author  ChengHaotian (yeguangbaozi@foxmail.com)
         * @date    2024-10-31
         */
        virtual void setPickedColor(QColor color);

        /**
         * @brief   根据当前拾取对象类型与拾取方法隐藏或显示部分演员，并设置其可拾取状态（SEA节点、网格单元、几何面等）。[虚函数]
         * @param   pickObjType：拾取对象类型
         * @param   pickMethod：拾取方法
         * @param   dataObjId：生效数据ID（-1则对所有数据生效）[缺省]
         * @author  ChengHaotian (yeguangbaozi@foxmail.com)
         * @date    2024-10-31
         */
        virtual void setActorStateByPickInfo(int pickObjType, int pickMethod, int dataObjId = -1);

    };
}   // namespace EventOper

#endif // __GRAPHINTERACTIONOPERATOR_H__