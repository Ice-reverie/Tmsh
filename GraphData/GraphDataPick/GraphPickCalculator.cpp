#include "GraphPickCalculator.h"
#include "GraphPickData.h"

#include "GraphData/GraphDataAdaptor/GraphObjectBase.h"
#include "GraphData/GraphDataAdaptor/GraphObjectGeoModel.h"

#include "FITK_Kernel/FITKAppFramework/FITKAppFramework.h"
#include "FITK_Kernel/FITKAppFramework/FITKKeyMouseStates.h"
#include "FITK_Kernel/FITKAppFramework/FITKGlobalData.h"
#include "FITK_Kernel/FITKCore/FITKDataRepo.h"
#include "FITK_Interface/FITKVTKAlgorithm/FITKShellFeatureEdges.h"
#include "FITK_Interface/FITKVTKAlgorithm/FITKSurfaceFilter.h"
#include "FITK_Interface/FITKVTKAlgorithm/FITKExtractGeometry.h"
#include "FITK_Interface/FITKVTKAlgorithm/FITKGraphActor.h"
#include "FITK_Interface/FITKInterfaceGeometry/FITKAbsGeoCommand.h"

#include <vtkActor.h>
#include <vtkMapper.h>
#include <vtkPlanes.h>

namespace Graph
{
    GraphPickCalculator::GraphPickCalculator(GraphPickData* pickedData) :
        m_pickedData(pickedData)
    {

    }

    void GraphPickCalculator::calculate()
    {
        if (m_pickedData == nullptr){
            return;
        }

        
        if (m_pickedData->getPickedMouseType() == PickedMouseType::PickedMouseClick){
            //点选
            GraphDataPickInfoStru pickInfo = m_pickedData->getPickedInfo();
            //判断拾取方式
            switch (pickInfo._pickMethod){
            case PickMethod::PMIndividually:
            case PickMethod::PMSingle:{
                bySingle();
                break;
            }
            default:
                return;
            }
        }
        else if (m_pickedData->getPickedMouseType() == PickedMouseType::PickedMouseRubber){
            //框选拾取方式
            byAreaPick();
        }

        // 不管计算完成都将是否需要计算标识置为false。
        m_pickedData->calculateFinsish();

        // 排序。
        m_pickedData->sortIds();
    }

    void GraphPickCalculator::bySingle()
    {
        //根据拾取数据类型进行不同数据获取。
        switch (m_pickedData->getPickedDataType())
        {
        case PickedDataType::ModelVertPick:
        case PickedDataType::ModelEdgePick:
        case PickedDataType::ModelFacePick:
        case PickedDataType::ModelSolidPick:
            bySingleGeometry();
            break;
        }
    }

    void GraphPickCalculator::byAreaPick()
    {
        //根据拾取数据类型进行不同数据获取。
        switch (m_pickedData->getPickedDataType()){
        case PickedDataType::ModelVertPick:
        case PickedDataType::ModelEdgePick:
        case PickedDataType::ModelFacePick:
        case PickedDataType::ModelSolidPick:
            byAreaGeometry();
        default:
            return;
        }
    }

    void GraphPickCalculator::bySingleGeometry()
    {
        if (m_pickedData == nullptr) {
            return;
        }

        Graph::GraphObjectGeoModel* geoGraph = dynamic_cast<Graph::GraphObjectGeoModel*>(m_pickedData->getPickedGraphObejct());
        int index = m_pickedData->getPickedIndex();
        if (geoGraph == nullptr || index < 0){
            return;
        }

        // 获取虚拓扑ID。
        int vTopoId = -1, vTopoIndex = -1;

        // 根据拾取数据类型进行不同数据获取。
        switch (m_pickedData->getPickedDataType())
        {
        case PickedDataType::ModelVertPick:
            vTopoId = geoGraph->getShapeIdByVTKCellId(index, Graph::ShapeAbsEnum::STA_VERTEX);
            vTopoIndex = geoGraph->getVirTopoIndexByShapeId(vTopoId, Graph::ShapeAbsEnum::STA_VERTEX);
            break;
        case PickedDataType::ModelEdgePick:
            vTopoId = geoGraph->getShapeIdByVTKCellId(index, Graph::ShapeAbsEnum::STA_EDGE);
            vTopoIndex = geoGraph->getVirTopoIndexByShapeId(vTopoId, Graph::ShapeAbsEnum::STA_EDGE);
            break;
        case PickedDataType::ModelFacePick:
            vTopoId = geoGraph->getShapeIdByVTKCellId(index, Graph::ShapeAbsEnum::STA_FACE);
            vTopoIndex = geoGraph->getVirTopoIndexByShapeId(vTopoId, Graph::ShapeAbsEnum::STA_FACE);
            break;
        case PickedDataType::ModelSolidPick:
            vTopoId = geoGraph->getShapeIdByVTKCellId(index, Graph::ShapeAbsEnum::STA_SOLID);
            vTopoIndex = geoGraph->getVirTopoIndexByShapeId(vTopoId, Graph::ShapeAbsEnum::STA_SOLID);
            break;
        default:
            return;
        }

        // 追加几何虚拓扑ID。
        if (vTopoId != -1)
        {
            m_pickedData->appendPickID(vTopoId);
            //添加虚拓扑索引
            m_pickedData->appendPickedOtherId(vTopoId, vTopoIndex);
        }
    }

    void GraphPickCalculator::byAreaGeometry()
    {
        //获取拾取平面。
        vtkPlanes* pickPlanes = m_pickedData->getCutPlane();
        vtkActor* actor = m_pickedData->getPickedActor();
        Graph::GraphObjectGeoModel* graphGeo = dynamic_cast<Graph::GraphObjectGeoModel*>(m_pickedData->getPickedGraphObejct());
        if (!graphGeo || !pickPlanes || !actor)
        {
            return;
        }

        // 获取拾取数据集。
        vtkDataSet* dataSet = actor->GetMapper()->GetInputAsDataSet();
        if (!dataSet){
            return;
        }

        //根据拾取数据类型进行不同数据获取。
        Graph::ShapeAbsEnum sType;
        switch (m_pickedData->getPickedDataType()) {
        case PickedDataType::ModelVertPick:sType = Graph::ShapeAbsEnum::STA_VERTEX; break;
        case PickedDataType::ModelEdgePick:sType = Graph::ShapeAbsEnum::STA_EDGE; break;
        case PickedDataType::ModelFacePick:sType = Graph::ShapeAbsEnum::STA_FACE; break;
        case PickedDataType::ModelSolidPick:sType = Graph::ShapeAbsEnum::STA_SOLID; break;
        default:return;
        }

        //提取数据。
        vtkSmartPointer<FITKExtractGeometry> extractor = vtkSmartPointer<FITKExtractGeometry>::New();
        extractor->SetImplicitFunction(pickPlanes);
        extractor->SetInputData(dataSet);
        extractor->Update();

        // 追加几何面ID。
        const QList<int> cellsIndice = extractor->getSelectOriginalCells();

        // 被拾取形状编号。
        QVector<int> shapeIds;

        //预处理拾取单元数据。（加速判断拾取子Id包含关系）
        int nCells = dataSet->GetNumberOfCells();
        QVector<int> cellPickedFlags;
        cellPickedFlags.resize(nCells);
        cellPickedFlags.fill(0);
        //存储形状数据id与形状虚拓扑索引拓扑关系
        QHash<int, int> shapeIdAndVirTopoIndex = {};

        //获取虚拓扑ID与索引
        for (const int & index : cellsIndice){
            int id = graphGeo->getShapeIdByVTKCellId(index, sType);

            if (!shapeIds.contains(id)) {
                shapeIds.push_back(id);
            }

            if (index < cellPickedFlags.size()) {
                cellPickedFlags[index] = 1;
            }

            //存储虚拓扑与虚拓扑索引关系
            int virTopoIndex = graphGeo->getVirTopoIndexByShapeId(id, sType);
            shapeIdAndVirTopoIndex.insert(id, virTopoIndex);
        }

        // 保存拾取数据。
        for (int i = 0; i < shapeIds.count(); i++){
            int shapeId = shapeIds[i];

            //检测当前几何数据是否完全被选中。
            QVector<int> subIds = graphGeo->getVTKCellIdsByShapeId(shapeId, sType);
            bool isFullPicked = true;
            for (const int & id : subIds){
                isFullPicked &= (cellPickedFlags[id] == 1);
            }

            //完全选中且未被拾取过
            if (isFullPicked && !m_pickedData->getPickedIDs().contains(shapeId)){
                //存储拾取形状id
                m_pickedData->appendPickID(shapeId);
                //存储拾取形状id与形状虚拓扑索引对应关系
                m_pickedData->appendPickedOtherId(shapeId, shapeIdAndVirTopoIndex.value(shapeId));
            }
        }

        //清除缓存。
        shapeIds.clear();
    }
}