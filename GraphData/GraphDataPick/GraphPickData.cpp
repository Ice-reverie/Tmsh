#include "GraphPickData.h"
#include "GraphDataPickInfo.h"

#include "GraphData/GraphDataAdaptor/GraphObjectCommons.h"
#include "GraphData/GraphDataAdaptor/GraphObjectBase.h"
#include "OperatorsInterface/GraphEventOperator.h"

#include "FITK_Kernel/FITKCore/FITKDataRepo.h"
#include "FITK_Kernel/FITKCore/FITKOperatorRepo.h"
#include "FITK_Kernel/FITKAppFramework/FITKAppFramework.h"
#include "FITK_Kernel/FITKAppFramework/FITKKeyMouseStates.h"
#include "FITK_Kernel/FITKAppFramework/FITKGlobalData.h"
#include "FITK_Component/FITKGeoCompOCC/FITKAbstractOCCModel.h"
#include "FITK_Interface/FITKVTKAlgorithm/FITKGraphActor.h"
#include "FITK_Interface/FITKVTKAlgorithm/FITKGraphActor2D.h"
#include "FITK_Interface/FITKVTKAlgorithm/FITKShellFeatureEdges.h"
#include "FITK_Interface/FITKVTKAlgorithm/FITKSurfaceFilter.h"

#include <vtkPlanes.h>
#include <vtkUnstructuredGrid.h>
#include <vtkDataSet.h>
#include <vtkActor.h>
#include <vtkActor2D.h>
#include <vtkMapper.h>
#include <vtkSelectionNode.h>
#include <vtkSelection.h>
#include <vtkExtractSelection.h>
#include <vtkIdTypeArray.h>
#include <QVector>

namespace Graph
{
    GraphPickData::GraphPickData(GraphDataPickInfoStru pickedInfo, vtkActor* pickedActor, int pickedIndex, double* pickedWorldPos, bool isPreview) :
        m_pickedInfo(pickedInfo), m_pickedActor(pickedActor), m_pickedIndex(pickedIndex), m_isPreview(isPreview)
    {
        // 初始化鼠标操作方式。（点击）
        m_mouseOper = PickedMouseType::PickedMouseClick;

        if (pickedWorldPos)
        {
            m_pickedWorldPos[0] = pickedWorldPos[0];
            m_pickedWorldPos[1] = pickedWorldPos[1];
            m_pickedWorldPos[2] = pickedWorldPos[2];
        }

        // 初始化。
        init();
    }

    GraphPickData::GraphPickData(GraphDataPickInfoStru pickedInfo, vtkActor* pickedActor, vtkPlanes* cutPlane) :
        m_pickedInfo(pickedInfo), m_pickedActor(pickedActor), m_cutPlane(cutPlane)
    {
        // 初始化鼠标操作方式。（框选）
        m_mouseOper = PickedMouseType::PickedMouseRubber;

        // 初始化。
        init();
    }

    GraphPickData::GraphPickData(GraphDataPickInfoStru pickedInfo, vtkActor2D* pickedActor, bool isPreview) :
        m_pickedInfo(pickedInfo), m_pickedActor2D(pickedActor), m_isPreview(isPreview)
    {
        // 初始化鼠标操作方式。（点击）
        m_mouseOper = PickedMouseType::PickedMouseClick;

        // 初始化。
        init2D();
    }

    GraphPickData::GraphPickData(Interface::FITKModelEnum::FITKModelSetType pType, int dataObjectId, QList<int> & indice)
    {
        // 初始化鼠标操作方式。（点击）
        m_mouseOper = PickedMouseType::PickedMouseClick;

        // 反向获取拾取类型。
        switch (pType)
        {
        case Interface::FITKModelEnum::FMSPoint:
            m_type = PickedDataType::ModelVertPick;
            break;
        case Interface::FITKModelEnum::FMSEdge:
            m_type = PickedDataType::ModelEdgePick;
            break;
        case Interface::FITKModelEnum::FMSSurface:
            m_type = PickedDataType::ModelFacePick;
            break;
        case Interface::FITKModelEnum::FMSSolid:
            m_type = PickedDataType::ModelSolidPick;
            break;
        case Interface::FITKModelEnum::FMSNone:
        case Interface::FITKModelEnum::FMSMIX:
        case Interface::FITKModelEnum::FMSComb:
        default:
            // 数据不可用。
            m_needToCal = false;
            m_isValid = false;
            return;
        }

        // 存储数据对象ID与数据索引。
        m_dataObjId = dataObjectId;
        m_ids = indice;

        // 初始化。
        initManual();
    }

    GraphPickData::GraphPickData()
    {

    }

    GraphPickData::~GraphPickData()
    {
        // 析构前恢复高亮（参考点、坐标轴）状态。
        clearHighlight();
    }

    GraphPickData* GraphPickData::getCopy()
    {
        if (m_needToCal || !m_isValid)
        {
            return nullptr;
        }

        GraphPickData* data = new GraphPickData;
        data->m_type = m_type;
        data->m_dataObjId = m_dataObjId;

        data->m_ids = m_ids;
        data->m_otherIds = m_otherIds;
        data->m_graphObject = m_graphObject;
        data->m_keyFlagsHash = m_keyFlagsHash;

        data->m_pickedActor = m_pickedActor;
        data->m_pickedActor2D = m_pickedActor2D;
        data->m_pickedIndex = m_pickedIndex;
        data->m_pickedInfo = m_pickedInfo;

        return data;
    }

    void GraphPickData::sortIds()
    {
        if (!m_ids.isEmpty())
        {
            std::sort(m_ids.begin(), m_ids.end());
        }
    }

    bool GraphPickData::isSameAs(GraphPickData* data)
    {
        if (!data)
        {
            return false;
        }

        if (data->m_type == m_type &&
            data->m_dataObjId == m_dataObjId)
        {
            return true;
        }
        else
        {
            return false;
        }
    }

    void GraphPickData::sort()
    {
        std::sort(m_ids.begin(), m_ids.end());
    }

    void GraphPickData::add(GraphPickData* data)
    {
        if (!data)
        {
            return;
        }

        // 首先进行排序，获取ID最大值。（最后一位ID）
        sort();
        data->sort();

        // 获取最大ID开数组存取存在标识。
        if (m_ids.count() && data->m_ids.count())
        {
            int nIdsMax = qMax(m_ids.last(), data->m_ids.last());
            QVector<int> idFlag(nIdsMax);
            idFlag.fill(-1);
            for (const int & id : m_ids)
            {
                idFlag[id - 1] = 1;
            }

            // 合并单元或节点ID。
            QHash<int, int> pickOther = data->getPickedOtherIDs();
            for (const int & id : data->m_ids)
            {
                if (idFlag[id - 1] == -1)
                {
                    m_ids.push_back(id);
                    m_otherIds.insert(id, pickOther.value(id));
                    idFlag[id - 1] = 1;
                }
            }
        }

        // 排序。
        sortIds();

        //额外拷贝后者键盘按下情况。
        m_keyFlagsHash = data->m_keyFlagsHash;

        // 合并后析构。
        delete data;
    }

    void GraphPickData::saveKeyFlags(Qt::Key qKey, bool flag)
    {
        m_keyFlagsHash[qKey] = flag;
    }

    void GraphPickData::subtract(GraphPickData* data)
    {
        if (!data)
        {
            return;
        }

        // 首先进行排序，获取ID最大值。（最后一位ID）
        sort();
        data->sort();

        // 获取最大ID开数组存取存在标识。
        if (m_ids.count() && data->m_ids.count())
        {
            int nIdsMax = qMax(m_ids.last(), data->m_ids.last());
            QVector<int> idFlags(nIdsMax);
            idFlags.fill(-1);
            for (const int & id : m_ids)
            {
                idFlags[id - 1] = 1;
            }

            // 移除单元或节点ID。
            for (const int & id : data->m_ids)
            {
                if (idFlags[id - 1] == 1)
                {
                    idFlags[id - 1] = -1;
                }
            }

            m_ids.clear();
            m_otherIds.clear();
            for (int i = 0; i < idFlags.count(); i++)
            {
                int& idFlag = idFlags[i];
                if (idFlag == 1)
                {
                    m_ids.push_back(i + 1);
                    m_otherIds.insert(i + 1, m_otherIds.value(i + 1));
                }
            }
        }

        // 合并后析构。
        delete data;
    }

    void GraphPickData::init()
    {
        // 尝试转换为自定义Actor。
        FITKGraphActor* fActor = FITKGraphActor::SafeDownCast(m_pickedActor);
        if (!fActor)
        {
            return;
        }

        // 获取演员存储的可视化对象。
        Graph::GraphObjectBase* obj = fActor->getGraphObjectAs<Graph::GraphObjectBase>();
        if (!obj)
        {
            return;
        }

        // 保存可视化对象。
        m_graphObject = obj;
        m_dataObjId = obj->getDataId();

        switch (m_pickedInfo._pickObjType) {
        case PickObjType::PickGeoVertex:
            m_type = ModelVertPick;
            break;
        case PickObjType::PickGeoCurve:
            m_type = ModelEdgePick;
            break;
        case PickObjType::PickGeoFace:
            m_type = ModelFacePick;
            break;
        case PickObjType::PickGeoSolid:
            m_type = ModelSolidPick;
            break;
        default:
            m_type = OtherPick;
            break;
        }

        if (m_type == OtherPick)
        {
            return;
        }

        m_needToCal = true;
        m_isValid = true;
    }

    void GraphPickData::init2D()
    {
        // 尝试转换为自定义Actor。
        FITKGraphActor2D* fActor2D = FITKGraphActor2D::SafeDownCast(m_pickedActor2D);
        if (!fActor2D)
        {
            return;
        }

        // 获取演员存储的可视化对象。
        Graph::GraphObjectBase* obj = fActor2D->getGraphObjectAs<Graph::GraphObjectBase>();
        if (!obj)
        {
            return;
        }

        // 保存可视化对象。
        m_graphObject = obj;
        m_dataObjId = obj->getDataId();


        switch (m_pickedInfo._pickObjType) {
        case PickObjType::PickGeoVertex:
            m_type = ModelVertPick;
            break;
        case PickObjType::PickGeoCurve:
            m_type = ModelEdgePick;
            break;
        case PickObjType::PickGeoFace:
            m_type = ModelFacePick;
            break;
        case PickObjType::PickGeoSolid:
            m_type = ModelSolidPick;
            break;
        default:
            m_type = OtherPick;
            break;
        }

        if (m_type == OtherPick)
        {
            return;
        }

        m_needToCal = false;
        m_isValid = true;
    }

    void GraphPickData::initManual()
    {
        // 通过操作器获取可视化对象。
        Core::FITKOperatorRepo* operatorRepo = Core::FITKOperatorRepo::getInstance();
        EventOper::GraphEventOperator* operPre = operatorRepo->getOperatorT<EventOper::GraphEventOperator>("GraphPreprocess");
        if (!operPre)
        {
            return;
        }

        m_graphObject = operPre->getModelGraphObjectByDataId(m_dataObjId);

        // 判断数据是否可用。
        m_needToCal = false;
        m_isValid = m_graphObject != nullptr;

        // 排序。
        sortIds();
    }

    void GraphPickData::setPickedGraphObject(Graph::GraphObjectBase* obj)
    {
        if (!obj)
        {
            return;
        }

        // 断开旧数据信号。
        if (m_graphObject)
        {
            disconnect(m_graphObject, &Graph::GraphObjectBase::destroyed, this, &GraphPickData::slot_resetGraphObject);
        }

        // 保存数据，连接信号。
        m_graphObject = obj;
        connect(m_graphObject, &Graph::GraphObjectBase::destroyed, this, &GraphPickData::slot_resetGraphObject, Qt::UniqueConnection);
    }

    void GraphPickData::slot_resetGraphObject()
    {
        m_graphObject = nullptr;
    }

    void GraphPickData::highlight()
    {
        if (!m_graphObject)
        {
            return;
        }

        // 如果是预选并且没有在高亮则预选。
        if (m_isPreview)
        {
            //if (!m_graphObject->isHighlighting())
            //{
            //    m_graphObject->preHighlight();
            //}
        }
        else
        {
            //m_graphObject->highlight();
        }
    }

    void GraphPickData::clearHighlight()
    {
        // 取消高亮。
        if (!m_graphObject)
        {
            return;
        }

        //// 如果是预选高亮参考点需要额外判断是否为高亮状态。
        //if (m_isPreview)
        //{
        //    // 如果为高亮状态则还原为高亮。
        //    if (m_graphObject->isHighlighting())
        //    {
        //        m_graphObject->highlight();
        //    }
        //    // 否则取消预选高亮。
        //    else
        //    {
        //        m_graphObject->disHighlight();
        //    }
        //}
        //// 高亮则直接取消高亮。
        //else
        //{
        //    m_graphObject->disHighlight();
        //}
    }

    void GraphPickData::calculateFinsish()
    {
        m_needToCal = false;

        m_mouseOper = PickedMouseType::PickedNoneType;
        m_pickedIndex = -1;
        m_cutPlane = nullptr;

        m_pickedWorldPos[0] = 0;
        m_pickedWorldPos[1] = 0;
        m_pickedWorldPos[2] = 0;
    }

    bool GraphPickData::needToCalculate()
    {
        return m_needToCal;
    }

    bool GraphPickData::isValid()
    {
        return m_isValid;
    }

    bool GraphPickData::isEmpty()
    {
        // 如果数据对象ID为空则一定为空拾取数据。
        if (m_dataObjId < 0)
        {
            return true;
        }

        bool isEmpty = m_ids.isEmpty();
        return isEmpty;
    }

    bool GraphPickData::isFinished()
    {
        // 单选情况下数据不为空则直接视为拾取结束。
        if (m_pickedInfo._pickMethod == PickMethod::PMSingle)
        {
            return true;
        }
        // 多选模式均通过键盘按下事件进行判断拾取结束。
        else
        {
            if (m_keyFlagsHash[Qt::Key::Key_Shift] && m_keyFlagsHash[Qt::Key::Key_Control])
            {
                return true;
            }
            else
            {
                return false;
            }
        }

        return true;
    }

    bool GraphPickData::contains(vtkActor* actor, int index)
    {
        if (!actor || index < 0 || !m_graphObject)
        {
            return false;
        }

        // 首先判断是否为同一演员，参考点直接返回。
        if (actor != m_pickedActor)
        {
            return false;
        }

        int id = -1;

        switch (m_pickedInfo._pickObjType) {
        case PickObjType::PickGeoVertex:
            id = m_graphObject->getShapeIdByVTKCellId(index, ShapeAbsEnum::STA_VERTEX);
            break;
        case PickObjType::PickGeoCurve:
            id = m_graphObject->getShapeIdByVTKCellId(index, ShapeAbsEnum::STA_EDGE);
            break;
        case PickObjType::PickGeoFace:
            id = m_graphObject->getShapeIdByVTKCellId(index, ShapeAbsEnum::STA_FACE);
            break;
        case PickObjType::PickGeoSolid:
            id = m_graphObject->getShapeIdByVTKCellId(index, ShapeAbsEnum::STA_SOLID);
            break;
        default:
            return false;
        }

        if (id == -1)
        {
            return false;
        }

        return m_ids.contains(id);
    }

    bool GraphPickData::contains(vtkActor2D* actor)
    {
        if (!actor || !m_graphObject)
        {
            return false;
        }

        // 新版本
        //@{
        return m_graphObject->contains(actor);
        //@}
    }

    GraphDataPickInfoStru GraphPickData::getPickedInfo()
    {
        return m_pickedInfo;
    }

    vtkActor* GraphPickData::getPickedActor()
    {
        return m_pickedActor;
    }

    int GraphPickData::getPickedIndex()
    {
        return m_pickedIndex;
    }

    PickedMouseType GraphPickData::getPickedMouseType()
    {
        return m_mouseOper;
    }

    vtkPlanes* GraphPickData::getCutPlane()
    {
        return m_cutPlane;
    }

    PickedDataType GraphPickData::getPickedDataType()
    {
        return m_type;
    }

    void GraphPickData::appendPickID(int pickId)
    {
        m_ids.append(pickId);
    }

    QList<int> GraphPickData::getPickedIDs()
    {
        return m_ids;
    }

    void GraphPickData::appendPickedOtherId(int pickId, int pickOtherId)
    {
        m_otherIds.insert(pickId, pickOtherId);
    }

    QHash<int, int> GraphPickData::getPickedOtherIDs()
    {
        return m_otherIds;
    }

    int GraphPickData::getPickedDataObjId()
    {
        return m_dataObjId;
    }

    Graph::GraphObjectBase* GraphPickData::getPickedGraphObejct()
    {
        return m_graphObject;
    }

    void GraphPickData::getDataSet(vtkUnstructuredGrid* ugrid)
    {
        // 未计算过的数据直接跳出。
        if (m_needToCal || !ugrid || !m_graphObject)
        {
            return;
        }

        vtkDataSet* dataSet{ nullptr };
        ShapeAbsEnum shapeEnum;

        // 根据拾取模型数据类型获取数据集。
        switch (m_type)
        {
        case PickedDataType::ModelVertPick:
        {
            dataSet = m_graphObject->getMesh(ShapeType::ModelVertex);
            shapeEnum = ShapeAbsEnum::STA_VERTEX;
            break;
        }
        case PickedDataType::ModelEdgePick:
        {
            dataSet = m_graphObject->getMesh(ShapeType::ModelEdge);
            shapeEnum = ShapeAbsEnum::STA_EDGE;
            break;
        }
        case PickedDataType::ModelFacePick:
        {
            dataSet = m_graphObject->getMesh(ShapeType::ModelFace);
            shapeEnum = ShapeAbsEnum::STA_FACE;
            break;
        }
        case PickedDataType::ModelSolidPick:
        {
            dataSet = m_graphObject->getMesh(ShapeType::ModelSolid);
            shapeEnum = ShapeAbsEnum::STA_SOLID;
            break;
        }
        default:
            return;
        }

        if (!dataSet)
        {
            return;
        }

        int nCell = dataSet->GetNumberOfCells();

        // 创建ID数组，加速合并数据。
        vtkSmartPointer<vtkIntArray> idArray = vtkSmartPointer<vtkIntArray>::New();
        idArray->SetNumberOfComponents(1);
        idArray->SetNumberOfValues(nCell);
        idArray->FillComponent(0, 0);

        // 根据OCC形状ID获取所有VTK数据。
        for (const int & id : m_ids)
        {
            const QVector<int> subCellIds = m_graphObject->getVTKCellIdsByShapeId(id, shapeEnum);
            for (const int & cId : subCellIds)
            {
                idArray->SetValue(cId, 1);
            }
        }

        // 通过ID获取实际VTK索引。
        vtkIdTypeArray* selectIdArray = vtkIdTypeArray::New();
        vtkSelectionNode* selectNode = vtkSelectionNode::New();
        vtkExtractSelection* extractSelection = vtkExtractSelection::New();

        for (int i = 0; i < nCell; i++)
        {
            if (idArray->GetValue(i) == 1)
            {
                selectIdArray->InsertNextValue(i);
            }
        }

        // 从原始网格数据提取。
        selectNode->SetFieldType(vtkSelectionNode::SelectionField::CELL);
        extractSelection->SetInputData(dataSet);

        // 提取VTK数据。
        //@{
        vtkSelection* section = vtkSelection::New();
        selectNode->SetContentType(vtkSelectionNode::INDICES);
        section->AddNode(selectNode);
        extractSelection->SetInputData(1, section);
        selectNode->SetSelectionList(selectIdArray);
        extractSelection->Update();
        //@}

        // 数据拷贝。
        ugrid->DeepCopy(extractSelection->GetOutput());

        // 析构。
        //@{
        selectNode->Delete();
        section->Delete();
        selectIdArray->Delete();
        extractSelection->Delete();
        //@}
    }

    void GraphPickData::getPickedWorldPosition(double* pos)
    {
        if (!pos)
        {
            return;
        }

        for (int i = 0; i < 3; i++)
        {
            pos[i] = m_pickedWorldPos[i];
        }
    }
}