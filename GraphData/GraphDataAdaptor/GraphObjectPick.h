/**********************************************************************
 * @file   GraphObjectPick.h
 * @brief  拾取模型渲染对象
 * @author BaGuijun (baguijun@163.com)
 * @date   2024-12-26
 *********************************************************************/
#ifndef __GraphObjectPick_H__
#define __GraphObjectPick_H__

#include "GraphObjectBase.h"
#include "GraphDataAdaptorAPI.h"

// Forward declaration
class QVariant;

class vtkDataSet;
class vtkDataSetSurfaceFilter;

class FITKGraphActor;
class FITKHighOrderCellFilter;

namespace Core
{
    class FITKAbstractDataObject;
}

namespace Graph
{
    /**
     * @brief  拾取模型渲染对象
     * @author BaGuijun (baguijun@163.com)
     * @date   2024-12-26
     */
    class GraphDataAdaptorAPI GraphObjectPick : public GraphObjectBase
    {
        // Regist
        FITKGraphObjectRegist(GraphObjectPick, GraphObjectBase);
        FITKCLASS(Graph, GraphObjectPick);
    public:
        /**
         * @brief    构造函数
         * @author   BaGuijun (baguijun@163.com)
         * @date     2024-12-26
         */
        GraphObjectPick();
        /**
         * @brief    析构函数
         * @return   void
         * @author   BaGuijun (baguijun@163.com)
         * @date     2024-12-26
         */
        ~GraphObjectPick();
        /**
         * @brief    设置拾取数据对象
         * @param[i] grid           数据对象
         * @author   BaGuijun (baguijun@163.com)
         * @date     2024-12-26
         */
        void setPickedData(vtkDataSet* grid);
        /**
         * @brief    设置拾取类型
         * @param[i] type          类型( 1: Points, 2: Cells, 3: Mixed)
         * @author   BaGuijun (baguijun@163.com)
         * @date     2024-12-26
         */
        void setPickedType(int type);
    private:
        /**
         * @brief    初始化
         * @author   BaGuijun (baguijun@163.com)
         * @date     2024-12-26
         */
        void init();
        /**
         * @brief    初始化演员属性
         * @param[i] actor              演员
         * @param[i] details            参数
         * @author   BaGuijun (baguijun@163.com)
         * @date     2024-12-26
         */
        void initActorProperties(vtkProp* actor, QVariant details = QVariant());
    private:
        /**
         * @brief    The actor.
         * @author   BaGuijun (baguijun@163.com)
         * @date     2024-12-26
         */
        FITKGraphActor* m_fActor{ nullptr };
        /**
         * @brief    The surface filter for high-order element.
         * @author   BaGuijun (baguijun@163.com)
         * @date     2024-12-26
         */
        vtkDataSetSurfaceFilter* m_surfaceFilter{ nullptr };
        /**
         * @brief    The high-order element dealer.
         * @author   BaGuijun (baguijun@163.com)
         * @date     2024-12-26
         */
        FITKHighOrderCellFilter* m_highOrderFilter{ nullptr };
    };
}   // namespace Exchange

#endif // __GraphObjectPick_H__
