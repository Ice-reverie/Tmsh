#include "OperGraphPick.h"

#include "GUIFrame/MainWindow.h"
#include "GUIFrame/RenderWidget.h"

#include "GraphData/GraphDataProvider/GraphDataProviderManager.h"
#include "GraphData/GraphDataProvider/GraphDataProviderModel.h"
#include "GraphData/GraphDataProvider/GraphDataProviderMark.h"
#include "GraphData/GraphDataAdaptor/GraphObjectBase.h"
#include "GraphData/GraphDataAdaptor/GraphObjectPick.h"
#include "GraphData/GraphDataPick/GraphDataPickInfo.h"
#include "GraphData/GraphDataPick/GraphPickProvider.h"

#include "FITK_Kernel/FITKAppFramework/FITKAppFramework.h"
#include "FITK_Kernel/FITKAppFramework/FITKGlobalData.h"
#include "FITK_Kernel/FITKAppFramework/FITKSignalTransfer.h"
#include "FITK_Kernel/FITKCore/FITKActionOperator.h"
#include "FITK_Interface/FITKVTKAlgorithm/FITKGraphActor.h"
#include "FITK_Interface/FITKVTKAlgorithm/FITKGraphActor2D.h"
#include "FITK_Interface/FITKVTKAlgorithm/FITKGraphActorImage.h"
#include "FITK_Component/FITKRenderWindowVTK/FITKGraph3DWindowVTK.h"
#include "FITK_Component/FITKRenderWindowVTK/FITKGraphRender.h"
#include "FITK_Component/FITKRenderWindowVTK/FITKGraphObjectVTK.h"
#include "FITK_Component/FITKWidget/FITKMdiArea.h"

namespace GUIOper
{
    OperGraphPick::OperGraphPick()
    {
        // 初始化拾取数据管理器。
        _pickedProvider = Graph::GraphPickProvider::getInstance();

        // 实例化高亮可视化对象。
        _graphObjPick = new Graph::GraphObjectPick();
        _graphObjPick->setVisible(false);

        // 初始化可视化对象三维数据。
        _graphObjPick->setPickedData(_pickedProvider->getPickedDataSet());
    }

    OperGraphPick::~OperGraphPick()
    {

    }

    void OperGraphPick::picked(Comp::FITKGraph3DWindowVTK* graphWindow, vtkActor* actor, int index, double* pickedWorldPos)
    {
        if (!graphWindow || !actor)
        {
            return;
        }

        // 处理拾取数据。
        Graph::PickObjType pickType = Graph::GraphDataPickInfo::GetPickInfo()._pickObjType;
        _pickedProvider->addPicked(actor, index, pickedWorldPos);

        switch (pickType)
        {
        case Graph::PickObjType::PickGeoVertex:
            _graphObjPick->setPickedType(1);
            break;
        case Graph::PickObjType::PickGeoCurve:
            _graphObjPick->setPickedType(2);
            break;
        case Graph::PickObjType::PickGeoFace:
        case Graph::PickObjType::PickGeoSolid:
            _graphObjPick->setPickedType(3);
            break;
        default:
            _graphObjPick->setVisible(false);
            return;
        }
        //@}

        // 添加到渲染窗口。
        _graphObjPick->setVisible(true);
        addGraphObjToRenderWindow(_graphObjPick, graphWindow);

        // 刷新窗口。
        graphWindow->reRender();
    }

    void OperGraphPick::picked(Comp::FITKGraph3DWindowVTK* graphWindow, QList<vtkActor*> actors, vtkPlanes* pickPlanes)
    {
        if (!graphWindow || !pickPlanes || actors.count() == 0){
            return;
        }

        //处理拾取数据
        _pickedProvider->addPicked(actors, pickPlanes);

        //刷新窗口
        graphWindow->reRender();
    }

    void OperGraphPick::clear(Comp::FITKGraph3DWindowVTK* graphWindow)
    {
        // 清空拾取数据。
        _pickedProvider->clearPickedData();

        QHash<QString, void*> objInfo;
        bool pickError = true;
        objInfo.insert("PickError", &pickError);
        objInfo.insert("PickStart", &pickError);
        int cmdID = -1;
        objInfo.insert("CmdId", &cmdID);
        FITKAPP->getSignalTransfer()->graphObjectPickedSig(-1, -1, objInfo);
        emit _pickedProvider->sig_pickedOver();
        // 刷新窗口。
        if (graphWindow)
        {
            graphWindow->reRender();
        }
    }

    void OperGraphPick::setActorStateByPickInfo(int pickObjType, int pickMethod, int dataObjId)
    {
        Graph::PickObjType pObjType = Graph::PickObjType(pickObjType);
        Graph::PickMethod pMethod = Graph::PickMethod(pickMethod);
        Q_UNUSED(pMethod);

        // 获取模型与符号可视化对象管理器。
        Comp::FITKGraph3DWindowVTK* graphWindow = getCurrentGraphWindow();
        Graph::GraphDataProviderModel* modelProvider = Graph::GraphDataProviderManager::getInstance()->getModelProvider(graphWindow);
        Graph::GraphDataProviderMark* markProvider = Graph::GraphDataProviderManager::getInstance()->getMarkProvider(graphWindow);
        if (modelProvider == nullptr || markProvider == nullptr)return;

        //关闭全部拾取
        modelProvider->setGeoModelVertexPick(false);
        modelProvider->setGeoModelCurverPick(false);
        modelProvider->setGeoModelFacePick(false);
        modelProvider->setGeoModelSolidPick(false);

        //
        switch (pObjType){
        case Graph::PickObjType::PickGeoVertex:
            modelProvider->setGeoModelVertexPick(true, dataObjId);
            break;
        case Graph::PickObjType::PickGeoCurve:
            modelProvider->setGeoModelCurverPick(true, dataObjId);
            break;
        case Graph::PickObjType::PickGeoFace:
            modelProvider->setGeoModelFacePick(true, dataObjId);
            break;
        case Graph::PickObjType::PickGeoSolid:
            modelProvider->setGeoModelSolidPick(true, dataObjId);
            break;
        }
    }

    void OperGraphPick::addGraphObjToRenderWindow(Graph::GraphObjectBase* obj, Comp::FITKGraph3DWindowVTK* graphWindow)
    {
        if (!obj || !graphWindow) {
            return;
        }

        // 添加可视化对象。
        obj->removeFromGraphWidget();
        graphWindow->addObject(obj->getRenderLayer(), obj, true);

        // 刷新窗口。
        graphWindow->reRender();
    }

    Comp::FITKGraph3DWindowVTK* OperGraphPick::getCurrentGraphWindow()
    {
        // 获取主窗口。
        GUI::MainWindow* mainWindow = FITKAPP->getGlobalData()->getMainWindowT<GUI::MainWindow>();
        if (mainWindow == nullptr) {
            return nullptr;
        }

        GUI::RenderWidget* renderWidget = mainWindow->getRenderWidget();
        if (renderWidget == nullptr) {
            return nullptr;
        }

        // 获取3D区域。
        Comp::FITKGraph3DWindowVTK* widget = dynamic_cast<Comp::FITKGraph3DWindowVTK*>(renderWidget->getRenderWin());
        return widget;
    }
}  // namespace GUIOper