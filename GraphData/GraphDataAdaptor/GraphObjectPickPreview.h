/**********************************************************************
 * @file   GraphObjectPickPreview.h
 * @brief  拾取模型预选渲染对象
 * @author BaGuijun (baguijun@163.com)
 * @date   2024-12-26
 *********************************************************************/
#ifndef _GraphObjectPickPreview_H__
#define _GraphObjectPickPreview_H__

#include "GraphObjectBase.h"
#include "GraphDataAdaptorAPI.h"

// Forward declaration
class QVariant;

class vtkDataSet;
class vtkSelectionNode;
class vtkSelection;
class vtkExtractSelection;
class vtkIdTypeArray;
class vtkDataSetSurfaceFilter;
class vtkActor;

class FITKGraphActor;
class FITKHighOrderCellFilter;

namespace Core{
    class FITKAbstractDataObject;
}

namespace Graph {
    /**
     * @brief  拾取模型预选渲染对象
     * @author BaGuijun (baguijun@163.com)
     * @date   2024-12-26
     */
    class GraphDataAdaptorAPI GraphObjectPickPreview : public GraphObjectBase
    {
        // Regist
        FITKGraphObjectRegist(GraphObjectPickPreview, GraphObjectBase);
        FITKCLASS(Graph, GraphObjectPickPreview);
    public:
        /**
         * @brief    Constructor.
         * @author   BaGuijun (baguijun@163.com)
         * @date     2024-12-26
         */
        GraphObjectPickPreview();

        /**
         * @brief    Destructor.
         * @author   BaGuijun (baguijun@163.com)
         * @date     2024-12-26
         */
        ~GraphObjectPickPreview();
        /**
         * @brief    Set the picked type( Point or Cell ) and the picked index.
         * @param    actor: The picked actor.
         * @param    type: The picked type( 1: Point, 2: Cell, 3: Mixed )
         * @param    index: The picked index
         * @author   BaGuijun (baguijun@163.com)
         * @date     2024-12-26
         */
        void setPickedData(vtkActor* actor, int type, int index);

        /**
         * @brief    Set the dataset for visualization.
         * @param    grid: The picked data
         * @author   BaGuijun (baguijun@163.com)
         * @date     2024-12-26
         */
        void setPickedData(vtkDataSet* grid);

        /**
         * @brief    Set the picked type( Point or Cell ) and the picked index.
         * @param    dataSet: The picked dataSet.
         * @param    type: The picked type( 1: Point, 2: Cell, 3: Mixed )
         * @param    index: The picked index
         * @author   BaGuijun (baguijun@163.com)
         * @date     2024-12-26
         */
        void setPickedData(vtkDataSet* dataSet, int type, int index);

        /**
         * @brief    Set the dataset for visualization.
         * @param    type: The type of picked data( 1: Points, 2: Cells, 3: Mixed )
         * @author   BaGuijun (baguijun@163.com)
         * @date     2024-12-26
         */
        void setPickedType(int type);

    private:
        /**
         * @brief   Initialize the pointers.
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
         * @brief   The actor.
         * @author   BaGuijun (baguijun@163.com)
         * @date     2024-12-26
         */
        FITKGraphActor* m_fActor{ nullptr };

        /**
         * @brief   The selection node for extract mesh grid.
         * @author   BaGuijun (baguijun@163.com)
         * @date     2024-12-26
         */
        vtkSelectionNode* m_selectNode{ nullptr };

        /**
         * @brief   The selection for vtkSelectionNode.
         * @author   BaGuijun (baguijun@163.com)
         * @date     2024-12-26
         */
        vtkSelection* m_section{ nullptr };

        /**
         * @brief   The mesh grid extractor.
         * @author   BaGuijun (baguijun@163.com)
         * @date     2024-12-26
         */
        vtkExtractSelection* m_extractSelection{ nullptr };

        /**
         * @brief   The surface filter for high-order element.
         * @author   BaGuijun (baguijun@163.com)
         * @date     2024-12-26
         */
        vtkDataSetSurfaceFilter* m_surfaceFilter{ nullptr };

        /**
         * @brief   The high-order element dealer.
         * @author   BaGuijun (baguijun@163.com)
         * @date     2024-12-26
         */
        FITKHighOrderCellFilter* m_highOrderFilter{ nullptr };

        /**
         * @brief   The indice array of extract id.
         * @author   BaGuijun (baguijun@163.com)
         * @date     2024-12-26
         */
        vtkIdTypeArray* m_idArray{ nullptr };

    };
}   // namespace Exchange

#endif // __GraphObjectPickPreview_H__
