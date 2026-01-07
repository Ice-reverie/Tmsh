/**********************************************************************
 * @file   GraphObjectGeoModel.h
 * @brief  几何渲染对象
 * @author BaGuijun (baguijun@163.com)
 * @date   2025-03-12
 *********************************************************************/
#ifndef GraphObjectGeoModel_H
#define GraphObjectGeoModel_H

#include "GraphDataAdaptorAPI.h"
#include "GraphObjectBase.h"

namespace Interface {
    class FITKAbsGeoCommand;
    class FITKVirtualTopoManager;
}

class FITKGraphActor;
class vtkUnstructuredGrid;
class vtkPolyData;
class vtkUnsignedCharArray;

namespace Graph
{
    /**
     * @brief  几何渲染对象
     * @author BaGuijun (baguijun@163.com)
     * @date   2025-03-12
     */
    class GraphDataAdaptorAPI GraphObjectGeoModel : public GraphObjectBase
    {
        FITKGraphObjectRegist(GraphObjectGeoModel, GraphObjectBase);
        FITKCLASS(Graph, GraphObjectGeoModel);
    public:
        /**
         * @brief    构造函数
         * @param[i] geoCommand                几何对象
         * @author   BaGuijun (baguijun@163.com)
         * @date     2025-03-12
         */
        GraphObjectGeoModel(Interface::FITKAbsGeoCommand* geoCommand);
        /**
         * @brief    析构函数
         * @return   void
         * @author   BaGuijun (baguijun@163.com)
         * @date     2025-03-12
         */
        virtual ~GraphObjectGeoModel();
        /**
         * @brief    更新
         * @param[i] forceUpdate          是否强制刷新
         * @author   BaGuijun (baguijun@163.com)
         * @date     2025-03-12
         */
        void update(bool forceUpdate) override;
        /**
         * @brief    获取可见性
         * @return   bool                  是否可见
         * @author   BaGuijun (baguijun@163.com)
         * @date     2025-03-12
         */
        bool getVisibility() override;
        /**
         * @brief    更新可见性
         * @author   BaGuijun (baguijun@163.com)
         * @date     2025-03-12
         */
        void updateVisibility() override;
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
         * @brief    根据类型获取网格数据
         * @param[i] forceUpdate                 类型
         * @return   vtkDataSet*                 数据
         * @author   BaGuijun (baguijun@163.com)
         * @date     2025-03-12
         */
        vtkDataSet* getMesh(ShapeType type) override;
        /**
         * @brief    根据给定的形状类型与VTK单元id获取形状ID
         * @param[i] vtkCellId                      VTK单元ID
         * @param[i] topAbsShapeType                形状类型
         * @return   int                            形状ID
         * @author   BaGuijun (baguijun@163.com)
         * @date     2024-12-26
         */
        int getShapeIdByVTKCellId(int vtkCellId, ShapeAbsEnum topAbsShapeType) override;
        /**
         * @brief    根据给定的虚拓扑id获取对应的索引
         * @param[i] vTopoId                        虚拓扑id
         * @param[i] topAbsShapeType                类型
         * @return   int                            索引
         * @author   BaGuijun (baguijun@163.com)
         * @date     2025-03-31
         */
        int getVirTopoIndexByShapeId(int vTopoId, ShapeAbsEnum topAbsShapeType);
        /**
         * @brief    更具给定的形状类型与形状id，获取VTK单元id
         * @param[i] shapeId                        形状id
         * @param[i] topAbsShapeType                形状类型
         * @return   const QVector<int>             单元id
         * @author   BaGuijun (baguijun@163.com)
         * @date     2024-12-26
         */
        const QVector<int> getVTKCellIdsByShapeId(int shapeId, ShapeAbsEnum topAbsShapeType) override;
        /**
         * @brief    设置拾取模式
         * @param[i] isPick            是否拾取
         * @param[i] type              类型
         * @author   BaGuijun (baguijun@163.com)
         * @date     2025-03-13
         */
        void setPickModel(bool isPick, ShapeType type) override;
        /**
         * @brief    重新应用缓存的子拓扑颜色（强制重建后恢复自定义色）
         */
        void applyStoredTopoColors();
    private:
        /**
         * @brief    初始化
         * @author   BaGuijun (baguijun@163.com)
         * @date     2025-03-12
         */
        void init();
        /**
         * @brief    更新渲染数据
         * @return   bool                      是否成功
         * @author   BaGuijun (baguijun@163.com)
         * @date     2025-03-12
         */
        bool updateGraphData();
        /**
         * @brief    创建演员
         * @author   BaGuijun (baguijun@163.com)
         * @date     2025-03-12
         */
        void createActor();
        /**
         * @brief    刷新点网格数据
         * @author   BaGuijun (baguijun@163.com)
         * @date     2025-03-12
         */
        void updateVertexGrid();
        /**
         * @brief    刷新线网格数据
         * @author   BaGuijun (baguijun@163.com)
         * @date     2025-03-12
         */
        void updateCurveGrid();
        /**
         * @brief    刷新面网格数据
         * @author   BaGuijun (baguijun@163.com)
         * @date     2025-03-12
         */
        void updateFaceGrid();
        /**
         * @brief    刷新体网格数据
         * @author   BaGuijun (baguijun@163.com)
         * @date     2025-03-12
         */
        void updateSolidGrid();
        /**
         * @brief    获取虚拓扑管理器
         * @return   Interface::FITKVirtualTopoManager*   虚拓扑管理器
         * @author   BaGuijun (baguijun@163.com)
         * @date     2025-03-12
         */
        Interface::FITKVirtualTopoManager* getVirTopoManager();
        /**
         * @brief    获取模型颜色
         * @return   QColor                颜色
         * @author   BaGuijun (baguijun@163.com)
         * @date     2025-04-09
         */
        QColor getGeoModelColor();
    private:
        /**
         * @brief  点演员
         * @author BaGuijun (baguijun@163.com)
         * @date   2025-03-12
         */
        FITKGraphActor* _vertexActor = nullptr;
        /**
         * @brief  线演员
         * @author BaGuijun (baguijun@163.com)
         * @date   2025-03-12
         */
        FITKGraphActor* _curveActor = nullptr;
        /**
         * @brief  面演员
         * @author BaGuijun (baguijun@163.com)
         * @date   2025-03-12
         */
        FITKGraphActor* _faceActor = nullptr;
        /**
         * @brief  体演员
         * @author BaGuijun (baguijun@163.com)
         * @date   2025-03-12
         */
        FITKGraphActor* _solidActor = nullptr;
        /**
         * @brief  点网格
         * @author BaGuijun (baguijun@163.com)
         * @date   2025-03-12
         */
        vtkUnstructuredGrid* _vertexGrid = nullptr;
        /**
         * @brief  线网格
         * @author BaGuijun (baguijun@163.com)
         * @date   2025-03-12
         */
        vtkUnstructuredGrid* _curveGrid = nullptr;
        /**
         * @brief  面网格
         * @author BaGuijun (baguijun@163.com)
         * @date   2025-03-12
         */
        vtkPolyData* _faceGrid = nullptr;
        /**
         * @brief  体网格
         * @author BaGuijun (baguijun@163.com)
         * @date   2025-03-12
         */
        vtkPolyData* _solidGrid = nullptr;
        /**
         * @brief  点网格颜色数组
         * @author ZhouZhaoYi (zhouzhaoyi@diso.cn)
         * @date   2025-03-28
         */
        vtkUnsignedCharArray* _vertexColorArray = nullptr;
        /**
         * @brief  线网格颜色数组
         * @author ZhouZhaoYi (zhouzhaoyi@diso.cn)
         * @date   2025-03-28
         */
        vtkUnsignedCharArray* _curveColorArray = nullptr;
        /**
         * @brief  面网格颜色数组
         * @author BaGuijun (baguijun@163.com)
         * @date   2025-03-28
         */
        vtkUnsignedCharArray* _faceColorArray = nullptr;
        /**
         * @brief  体网格颜色数组
         * @author ZhouZhaoYi (zhouzhaoyi@diso.cn)
         * @date   2025-03-28
         */
        vtkUnsignedCharArray* _solidColorArray = nullptr;
        /**
         * @brief  点VTK单元与虚拓扑id映射（vtk单元id----虚拓扑id）
         * @author BaGuijun (baguijun@163.com)
         * @date   2025-03-12
         */
        QHash<int, int> _vertexInfoHash = {};
        /**
         * @brief  线VTK单元与虚拓扑id映射（vtk单元id----虚拓扑id）
         * @author BaGuijun (baguijun@163.com)
         * @date   2025-03-12
         */
        QHash<int, int> _curveInfoHash = {};
        /**
         * @brief  面VTK单元与虚拓扑id映射（vtk单元id----虚拓扑id）
         * @author BaGuijun (baguijun@163.com)
         * @date   2025-03-12
         */
        QHash<int, int> _faceInfoHash = {};
        /**
         * @brief  体VTK单元与虚拓扑id映射（vtk单元id----虚拓扑id）
         * @author BaGuijun (baguijun@163.com)
         * @date   2025-03-12
         */
        QHash<int, int> _solidInfoHash = {};
        /**
         * @brief  点虚拓扑id与虚拓扑索引映射（虚拓扑id----虚拓扑索引）
         * @author BaGuijun (baguijun@163.com)
         * @date   2025-03-12
         */
        QHash<int, int> _vertexVirTopIdVirIndex = {};
        /**
         * @brief  线虚拓扑id与虚拓扑索引映射（虚拓扑id----虚拓扑索引）
         * @author BaGuijun (baguijun@163.com)
         * @date   2025-03-12
         */
        QHash<int, int> _curveVirTopIdVirIndex = {};
        /**
         * @brief  面虚拓扑id与虚拓扑索引映射（虚拓扑id----虚拓扑索引）
         * @author BaGuijun (baguijun@163.com)
         * @date   2025-03-12
         */
        QHash<int, int> _faceVirTopIdVirIndex = {};
        /**
         * @brief  体虚拓扑id与虚拓扑索引映射（虚拓扑id----虚拓扑索引）
         * @author BaGuijun (baguijun@163.com)
         * @date   2025-03-12
         */
        QHash<int, int> _solidVirTopIdVirIndex = {};
        /**
         * @brief  缓存根节点基础颜色，用于在更新根色时识别需要继承基础色的单元，避免覆盖子节点自定义颜色
         */
        QColor _baseColorCache;
        /**
         * @brief  子拓扑（壳/面/边/点等）自定义颜色缓存，键为虚拓扑id
         */
        QHash<int, QColor> _vTopoColorOverrides;
    };
}

#endif
