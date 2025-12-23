#include "GraphDataPickInfo.h"
#include "GraphPickData.h"
#include "GraphPickCalculator.h"
#include "GraphPickProvider.h"

#include "OperatorsInterface/GraphEventOperator.h"

#include "FITK_Kernel/FITKAppFramework/FITKAppFramework.h"
#include "FITK_Kernel/FITKAppFramework/FITKKeyMouseStates.h"
#include "FITK_Kernel/FITKAppFramework/FITKGlobalData.h"
#include "FITK_Kernel/FITKAppFramework/FITKSignalTransfer.h"
#include "FITK_Kernel/FITKCore/FITKAbstractDataObject.h"
#include "FITK_Kernel/FITKCore/FITKOperatorRepo.h"
#include "FITK_Interface/FITKVTKAlgorithm/FITKGraphActor.h"
#include "FITK_Component/FITKRenderWindowVTK/FITKGraphObjectVTK.h"

#include <vtkPlanes.h>
#include <vtkActor.h>
#include <vtkActor2D.h>
#include <vtkPoints.h>
#include <vtkUnstructuredGrid.h>
#include <vtkAppendFilter.h>

namespace Graph
{
    // 静态变量初始化。
    GraphPickProvider* GraphPickProvider::s_instance{ nullptr };

    GraphPickProvider* GraphPickProvider::getInstance()
    {
        // 获取实例。
        if (!s_instance)
        {
            s_instance = new GraphPickProvider;
        }

        return s_instance;
    }

    void GraphPickProvider::Delete()
    {
        if (s_instance)
        {
            delete s_instance;
        }

        s_instance = nullptr;
    }

    GraphPickProvider::GraphPickProvider()
    {
        // 初始化键盘监视器。
        m_settings = FITKAPP->getGlobalData()->getKeyMouseStates();

        // 创建拾取可视化数据数据。
        m_dataSetPicked = vtkUnstructuredGrid::New();

        // 创建预选可视化数据。
        m_dataSetPreview = vtkUnstructuredGrid::New();
    }

    GraphPickProvider::~GraphPickProvider()
    {
        clearPickedData();

        // 清除VTK数据。
        if (m_dataSetPicked)
        {
            m_dataSetPicked->Delete();
            m_dataSetPicked = nullptr;
        }

        if (m_dataSetPreview)
        {
            m_dataSetPreview->Delete();
            m_dataSetPreview = nullptr;
        }
    }

    void GraphPickProvider::addDataManually(Interface::FITKModelEnum::FITKModelSetType type, int dataObjId, QList<int> & indice)
    {
        // 创建拾取数据。
        GraphPickData* data = new GraphPickData(type, dataObjId, indice);

        // 判断是否有效。
        if (data->isValid())
        {
            m_pickedDataList.push_back(data);
        }
        else
        {
            delete data;
        }

        // 生成拾取数据。
        generatePickedDataSet();
    }

    void GraphPickProvider::clearPickedData()
    {
        // 清除并析构。
        for (GraphPickData* data : m_pickedDataList)
        {
            if (data)
            {
                data->clearHighlight();
                delete data;
            }
        }

        m_pickedDataList.clear();

        // 同时清空可视化数据。
        if (m_dataSetPicked)
        {
            m_dataSetPicked->Reset();
            m_dataSetPicked->Modified();
        }

        // 同时清空预选数据。
        clearPickedDataPreview();

        //刷新
        EventOper::GraphEventOperator* operGraph = FITKOPERREPO->getOperatorT<EventOper::GraphEventOperator>("GraphPreprocess");
        if (operGraph) {
            operGraph->reRender();
        }
    }

    void GraphPickProvider::clearPickedDataPreview()
    {
        if (m_dataSetPreview)
        {
            m_dataSetPreview->Reset();
            m_dataSetPreview->Modified();
        }

        if (m_pickedPreviewData)
        {
            delete m_pickedPreviewData;
            m_pickedPreviewData = nullptr;
        }
    }

    vtkDataSet* GraphPickProvider::getPickedPreviewDataSet()
    {
        return m_dataSetPreview;
    }

    vtkDataSet* GraphPickProvider::getPickedDataSet()
    {
        return m_dataSetPicked;
    }

    GraphPickData* GraphPickProvider::getPickedData(vtkActor* actor, int index, double* pickedWorldPos, bool isPreview)
    {
        // 获取拾取方式及附加信息。
        GraphDataPickInfoStru pickInfo = GraphDataPickInfo::GetPickInfo();

        // 创建拾取信息。
        GraphPickData* pickedData = new GraphPickData(pickInfo, actor, index, pickedWorldPos, isPreview);

        // 判断拾取是否有效。
        if (!pickedData->isValid())
        {
            delete pickedData;
            return nullptr;
        }

        // 创建临时计算器。
        GraphPickCalculator calculator(pickedData);
        calculator.calculate();

        return pickedData;
    }

    GraphPickData* GraphPickProvider::getPickedData(vtkActor2D* actor, bool isPreview)
    {
        // 获取拾取方式及附加信息。
        GraphDataPickInfoStru pickInfo = GraphDataPickInfo::GetPickInfo();

        // 创建拾取信息。
        GraphPickData* pickedData = new GraphPickData(pickInfo, actor, isPreview);

        // 判断拾取是否有效。
        if (!pickedData->isValid())
        {
            delete pickedData;
            return nullptr;
        }

        // 创建临时计算器。
        GraphPickCalculator calculator(pickedData);
        calculator.calculate();

        return pickedData;
    }

    void GraphPickProvider::emitPickSignals()
    {
        // 创建对象信息哈希表
        QHash<QString, void*> objInfo;
        bool pickStart = true;
        objInfo.insert("PickStart", &pickStart);
        // 如果没有拾取数据，直接返回
        if (m_pickedDataList.isEmpty())
        {
            bool pickError = true;
            objInfo.insert("PickError", &pickError);
            FITKAPP->getSignalTransfer()->graphObjectPickedSig(-1, -1, objInfo);
            return;

        }
        // 发送通用拾取信号
        emit sig_pickedData();

        // 获取拾取的数据
        QList<GraphPickData*> dataList = m_pickedDataList;
        for (GraphPickData* pData : dataList) {

            if (pData) {
                //获取拾取类型并转换
                int pickType = -1;
                switch (pData->getPickedInfo()._pickObjType)
                {
                case PickObjType::PickGeoVertex:
                    pickType = 3003; // 几何点
                    break;
                case PickObjType::PickGeoCurve:
                    pickType = 3004; // 几何线
                    break;
                case PickObjType::PickGeoFace:
                    pickType = 3005; // 几何面
                    break;
                case PickObjType::PickGeoSolid:
                    pickType = 3006; // 几何体
                    break;
                default:
                    pickType = -1;
                    break;
                }
                int cmdID = pData->getPickedDataObjId();
                objInfo.insert("CmdId", &cmdID);
                //遍历拾取ID和虚拓扑ID，逐个发送
                QHash<int, int> pickedOtherIDs = pData->getPickedOtherIDs();
                for (int shapeId : pData->getPickedIDs()) {

                    if (pickedOtherIDs.contains(shapeId)) {

                        FITKAPP->getSignalTransfer()->graphObjectPickedSig(pickType, shapeId, objInfo);
                        pickStart = false;
                        objInfo.insert("PickStart", &pickStart);
                    }
                }

                //拾取结束
                if (pData->isFinished()) {
                    emit sig_pickedOver();
                }
            }
        }

    }

    bool GraphPickProvider::addPreviewPicked(vtkActor* actor, int index, double* pickedWorldPos)
    {
        if (m_pickedPreviewData)
        {
            // 获取拾取方式及附加信息。
            GraphDataPickInfoStru pickInfo = GraphDataPickInfo::GetPickInfo();

            bool isTheSamePick = false;

            if (m_pickedPreviewData->getPickedInfo()._pickObjType != pickInfo._pickObjType ||
                m_pickedPreviewData->getPickedInfo()._pickMethod != pickInfo._pickMethod)
            {
                // 判断是否与上次预选相同，相同则跳出，节约性能。
                if (m_pickedPreviewData->contains(actor, index))
                {
                    isTheSamePick = true;
                }
            }

            if (isTheSamePick)
            {
                return false;
            }

            delete m_pickedPreviewData;
            m_pickedPreviewData = nullptr;
        }

        // 计算生成拾取数据。
        GraphPickData* pickedData = getPickedData(actor, index, pickedWorldPos, true);
        if (!pickedData)
        {
            return false;
        }

        m_pickedPreviewData = pickedData;

        // 生成预选数据。
        generatePickedPreviewDataSet();

        return true;
    }

    bool GraphPickProvider::addPreviewPicked(vtkActor2D* actor)
    {
        if (m_pickedPreviewData)
        {
            bool isTheSamePick = false;

            // 判断是否与上次预选相同，相同则跳出，节约性能。
            if (m_pickedPreviewData->contains(actor))
            {
                isTheSamePick = true;
            }

            if (isTheSamePick)
            {
                return false;
            }

            delete m_pickedPreviewData;
            m_pickedPreviewData = nullptr;
        }

        // 计算生成拾取数据。
        GraphPickData* pickedData = getPickedData(actor, true);
        if (!pickedData)
        {
            return false;
        }

        m_pickedPreviewData = pickedData;

        // 生成预选数据。
        generatePickedPreviewDataSet();

        return true;
    }

    void GraphPickProvider::addPicked(vtkActor* actor, int index, double* pickedWorldPos)
    {
        GraphPickData* pickedData{ nullptr };

        // 如果有预选数据则使用预选数据。
        if (m_pickedPreviewData)
        {
            pickedData = m_pickedPreviewData->getCopy();

            // 拷贝后析构，防止快速进行多次拾取导致无法预选。
            delete m_pickedPreviewData;
            m_pickedPreviewData = nullptr;
        }
        else
        {
            // 计算生成拾取数据。
            pickedData = getPickedData(actor, index, pickedWorldPos);
        }

        if (!pickedData)
        {
            return;
        }

        // 处理拾取数据。
        dealPickedData(pickedData);

        // 生成拾取数据。
        generatePickedDataSet();

        emitPickSignals();

    }

    void GraphPickProvider::addPicked(vtkActor2D* actor)
    {
        GraphPickData* pickedData{ nullptr };

        // 如果有预选数据则使用预选数据。
        if (m_pickedPreviewData)
        {
            pickedData = m_pickedPreviewData->getCopy();

            // 拷贝后析构，防止快速进行多次拾取导致无法预选。
            delete m_pickedPreviewData;
            m_pickedPreviewData = nullptr;
        }
        else
        {
            // 计算生成拾取数据。
            pickedData = getPickedData(actor);
        }

        if (!pickedData)
        {
            return;
        }

        // 处理拾取数据。
        dealPickedData(pickedData);

        // 生成拾取数据。
        generatePickedDataSet();

        // 如果有数据被拾取，并且最后一次拾取数据为结束状态则发送信号。
        if (m_pickedDataList.count() != 0)
        {
            //有对象拾取
            emit sig_pickedData();
            GraphPickData* pData = m_pickedDataList.last();
            if (pData) {
                //拾取结束
                if (pData->isFinished()) {
                    emit sig_pickedOver();
                }
            }
        }
    }

    void GraphPickProvider::addPicked(QList<vtkActor*> actors, vtkPlanes* cutPlane)
    {
        // 获取拾取方式及附加信息。
        GraphDataPickInfoStru pickInfo = GraphDataPickInfo::GetPickInfo();

        // Single拾取模式禁止框选。
        if (pickInfo._pickMethod == PickMethod::PMSingle)
        {
            return;
        }

        // 是否需要合并或移除数据。
        bool needAddOrSubData = (m_settings->keyPressed(Qt::Key_Shift) && !m_settings->keyPressed(Qt::Key_Control)) ||
            (!m_settings->keyPressed(Qt::Key_Shift) && m_settings->keyPressed(Qt::Key_Control));

        // 不需要操作数据说明是普通框选，需要清空历史拾取信息。
        if (!needAddOrSubData)
        {
            clearPickedData();
        }

        for (vtkActor* actor : actors)
        {
            // 创建拾取信息。
            GraphPickData* pickedData = new GraphPickData(pickInfo, actor, cutPlane);

            // 判断拾取是否有效。
            if (!pickedData->isValid())
            {
                delete pickedData;
                continue;
            }

            // 创建临时计算器。
            GraphPickCalculator calculator(pickedData);
            calculator.calculate();

            // 处理拾取数据。
            dealPickedData(pickedData, true);
        }

        // 生成拾取数据。
        generatePickedDataSet();

        // 框选拾取也需要将“最终拾取到的拓扑ID”通知到业务层（如 GUI 统计/回填）。
        // emitPickSignals 内部会处理空拾取（发送 PickError）以及拾取结束判断。
        emitPickSignals();
    }

    void GraphPickProvider::generatePickedPreviewDataSet()
    {
        if (!m_dataSetPreview)
        {
            return;
        }

        m_dataSetPreview->Reset();

        if (!m_pickedPreviewData)
        {
            m_dataSetPreview->Modified();
            return;
        }

        // 为空则不进行计算。
        if (m_pickedPreviewData->isEmpty())
        {
            m_dataSetPreview->Modified();

            delete m_pickedPreviewData;
            m_pickedPreviewData = nullptr;
            return;
        }

        // 获取预选三维数据。
        vtkUnstructuredGrid* ugrid = vtkUnstructuredGrid::New();
        m_pickedPreviewData->getDataSet(ugrid);
        m_dataSetPreview->DeepCopy(ugrid);
        ugrid->Delete();
        m_dataSetPreview->Modified();
    }

    void GraphPickProvider::generatePickedDataSet()
    {
        // 合并拾取可视化数据。
        vtkAppendFilter* appendFilter = vtkAppendFilter::New();

        // 析构数据列表。
        QList<vtkUnstructuredGrid*> dataSets;

        // 移除已经被清空的数据。
        for (int i = m_pickedDataList.count() - 1; i >= 0; i--)
        {
            GraphPickData* data = m_pickedDataList[i];
            if (data->isEmpty())
            {
                m_pickedDataList.removeAt(i);
                delete data;
            }
        }

        for (GraphPickData* data : m_pickedDataList)
        {
            if (!data)
            {
                continue;
            }

            // 外部创建网格数据，内部进行数据填充。
            vtkUnstructuredGrid* ugrid = vtkUnstructuredGrid::New();
            data->getDataSet(ugrid);
            appendFilter->AddInputData(ugrid);

            dataSets.push_back(ugrid);
        }

        // 合并数据并拷贝。
        appendFilter->Update();

        m_dataSetPicked->DeepCopy(appendFilter->GetOutput());
        m_dataSetPicked->Modified();

        appendFilter->Delete();

        // 移除并析构子数据集。
        for (vtkUnstructuredGrid* dataSet : dataSets)
        {
            if (dataSet)
            {
                dataSet->Delete();
            }
        }

        dataSets.clear();
    }

    void GraphPickProvider::dealPickedData(GraphPickData* data, bool isAreaPick)
    {
        // 获取拾取方式及附加信息。
        GraphDataPickInfoStru pickInfo = GraphDataPickInfo::GetPickInfo();

        // 保存拾取时按键状态。
        bool ctrlPressed = m_settings->keyPressed(Qt::Key::Key_Control);
        bool shiftPressed = m_settings->keyPressed(Qt::Key::Key_Shift);
        data->saveKeyFlags(Qt::Key::Key_Control, ctrlPressed);
        data->saveKeyFlags(Qt::Key::Key_Shift, shiftPressed);

        //追加拾取
        if (m_settings->keyPressed(Qt::Key_Shift) && !m_settings->keyPressed(Qt::Key_Control) && pickInfo._pickMethod != PickMethod::PMSingle){
            // 获取相同模型拾取信息进行合并，否则直接添加。
            GraphPickData* brotherData = getSameModelPickData(data);
            if (brotherData){
                brotherData->add(data);
            }
            else{
                m_pickedDataList.push_back(data);
            }
        }
        //拾取擦除
        else if (!m_settings->keyPressed(Qt::Key_Shift) && m_settings->keyPressed(Qt::Key_Control)) {
            // 获取相同模型拾取信息进行做差，否则直接跳出。
            GraphPickData* brotherData = getSameModelPickData(data);
            if (brotherData) {
                brotherData->subtract(data);
            }
            else {
                return;
            }
        }
        //结束拾取
        else if (m_settings->keyPressed(Qt::Key_Shift) && m_settings->keyPressed(Qt::Key_Control)) {
            // 获取相同模型拾取信息进行合并，否则直接添加。
            GraphPickData* brotherData = getSameModelPickData(data);
            if (brotherData) {
                brotherData->add(data);
            }
            else {
                m_pickedDataList.push_back(data);
            }
        }
        //取消重新拾取
        else {
            if (!isAreaPick) {
                clearPickedData();
            }
            m_pickedDataList.push_back(data);
        }


        // 移除已经被清空的数据。
        for (int i = m_pickedDataList.count() - 1; i >= 0; i--){
            GraphPickData* data = m_pickedDataList[i];
            if (data->isEmpty()){
                m_pickedDataList.removeAt(i);
                delete data;
            }
        }
    }

    GraphPickData* GraphPickProvider::getSameModelPickData(GraphPickData* data)
    {
        if (!data || m_pickedDataList.isEmpty())
        {
            return nullptr;
        }

        // 寻找拾取到的同模型数据。
        for (GraphPickData* pData : m_pickedDataList)
        {
            if (data->isSameAs(pData))
            {
                return pData;
            }
        }

        return nullptr;
    }

    QList<GraphPickData*> GraphPickProvider::getPickedList()
    {
        return m_pickedDataList;
    }
}
