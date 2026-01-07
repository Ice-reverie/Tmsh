/**********************************************************************
 * @file   GraphObjectMesh.h
 * @brief  网格渲染对象
 * @author BaGuijun (baguijun@163.com)
 * @date   2025-03-12
 *********************************************************************/
#ifndef GraphObjectMeshKernel_H
#define GraphObjectMeshKernel_H

#include "GraphDataAdaptorAPI.h"
#include "GraphObjectBase.h"

namespace ModelData {
    class MeshKernel;
}

namespace Interface {
    class FITKAbstractModel;
    class FITKAbstractModelComponent;
}

class FITKGraphActor;
class vtkDataSet;
class vtkUnstructuredGrid;
class vtkStructuredGrid;

namespace Graph
{
    /**
     * @brief  几何渲染对象
     * @author BaGuijun (baguijun@163.com)
     * @date   2025-03-12
     */
    class GraphDataAdaptorAPI GraphObjectMesh : public GraphObjectBase
    {
        FITKGraphObjectRegist(GraphObjectMesh, GraphObjectBase);
        FITKCLASS(Graph, GraphObjectMesh);
    public:
        /**
         * @brief    构造函数
         * @param[i] meshKernel                数据对象
         * @author   BaGuijun (baguijun@163.com)
         * @date     2025-04-10
         */
        GraphObjectMesh(ModelData::MeshKernel* meshKernel);
        /**
         * @brief    析构函数
         * @author   BaGuijun (baguijun@163.com)
         * @date     2025-04-10
         */
        virtual ~GraphObjectMesh();
        /**
         * @brief    更新
         * @param[i] forceUpdate               是否强制刷新
         * @author   BaGuijun (baguijun@163.com)
         * @date     2025-04-11
         */
        virtual void update(bool forceUpdate) override;
        /**
         * @brief    更新可见性
         * @author   BaGuijun (baguijun@163.com)
         * @date     2025-04-11
         */
        virtual void updateVisibility() override;
        /**
         * @brief    更新子模型颜色
         * @param[i] color                    颜色
         * @param[i] otherPar                 其他参数[缺省]
         * @author   BaGuijun (baguijun@163.com)
         * @date     2025-03-28
         */
        virtual void updateSubColor(QColor color, QVariant otherPar = QVariant()) override;
        /**
         * @brief    更新模型颜色
         * @param[i] color                    颜色
         * @author   ZhouZhaoYi (zhouzhaoyi)
         * @date     2025-03-31
         */
        virtual void updateModelColor(QColor color = QColor()) override;
        /**
         * @brief    通过分组数据id获取对应的VTK单元id链表
         * @param[i] comID                         分组对象id
         * @return   QList<int>                    单元id链表
         * @author   BaGuijun (baguijun@163.com)
         * @date     2025-04-11
         */
        QList<int> getVTKCellIDsByComID(int comID);
    protected:
        /**
         * @brief    初始化
         * @author   BaGuijun (baguijun@163.com)
         * @date     2025-04-11
         */
        void init();
        /**
         * @brief    更新渲染数据
         * @return   bool                 是否成功
         * @author   BaGuijun (baguijun@163.com)
         * @date     2025-04-11
         */
        bool updateGraphData();
        /**
         * @brief    初始化演员
         * @author   BaGuijun (baguijun@163.com)
         * @date     2025-04-11
         */
        void initActor();
    private:
        /**
         * @brief    获取网格
         * @param[o] dataSet            网格数据
         * @author   BaGuijun (baguijun@163.com)
         * @date     2025-04-11
         */
        void getMesh(Interface::FITKAbstractModel* absModel, vtkDataSet* dataSet);
        /**
         * @brief    获取非结构化分区网格
         * @param[i] compoment           组件
         * @param[o] dataSet             网格数据
         * @author   BaGuijun (baguijun@163.com)
         * @date     2025-04-11
         */
        void getUnstrComponent(Interface::FITKAbstractModelComponent* compoment, vtkUnstructuredGrid* dataSet);
        /**
         * @brief    获取结构化分区网格
         * @param[i] compoment           组件
         * @param[o] dataSet             网格数据
         * @author   BaGuijun (baguijun@163.com)
         * @date     2025-04-11
         */
        void getStrComponent(Interface::FITKAbstractModelComponent* compoment, vtkStructuredGrid* dataSet);
        /**
         * @brief    创建VTK非结构化网格
         * @param[i] absModel                  FITK网格数据
         * @param[i] dataSet                   VTK网格数据
         * @author   BaGuijun (baguijun@163.com)
         * @date     2025-04-14
         */
        void createUnStrGridVKT(Interface::FITKAbstractModel* absModel, vtkDataSet* dataSet);
        /**
         * @brief    创建非结构化网格
         * @param[i] absModel                  FITK网格数据
         * @param[i] dataSet                   VTK网格数据
         * @author   BaGuijun (baguijun@163.com)
         * @date     2025-04-14
         */
        void createUnStrGrid(Interface::FITKAbstractModel* absModel, vtkDataSet* dataSet);
        /**
         * @brief    创建结构化网格
         * @param[i] absModel                  FITK网格数据
         * @param[i] dataSet                   VTK网格数据
         * @author   BaGuijun (baguijun@163.com)
         * @date     2025-04-14
         */
        void createStrGrid(Interface::FITKAbstractModel* absModel, vtkDataSet* dataSet);
    protected:
        /**
         * @brief  网格数据
         * @author BaGuijun (baguijun@163.com)
         * @date   2025-04-11
         */
        vtkDataSet* _meshData = nullptr;
        /**
         * @brief  网格分区数据
         * @author BaGuijun (baguijun@163.com)
         * @date   2025-04-11
         */
        vtkUnstructuredGrid* _comData = nullptr;
        /**
         * @brief  网格演员
         * @author BaGuijun (baguijun@163.com)
         * @date   2025-04-11
         */
        FITKGraphActor* _meshActor = nullptr;
        /**
         * @brief  网格分区演员
         * @author BaGuijun (baguijun@163.com)
         * @date   2025-04-11
         */
        FITKGraphActor* _comActor = nullptr;
    };
}

#endif
