#include "OperGraphEvent3D.h"

#include "GUIFrame/MainWindow.h"
#include "GUIFrame/RenderWidget.h"
#include "ModelData/MeshKernel.h"
#include "ModelData/MeshData.h"
#include "ModelData/MeshManager.h"
#include "GraphData/GraphDataProvider/GraphDataProviderManager.h"
#include "GraphData/GraphDataProvider/GraphDataProviderModel.h"
#include "GraphData/GraphDataProvider/GraphDataProviderMark.h"
#include "GraphData/GraphDataAdaptor/GraphObjectBase.h"

#include "FITK_Kernel/FITKAppFramework/FITKAppFramework.h"
#include "FITK_Kernel/FITKAppFramework/FITKGlobalData.h"
#include "FITK_Kernel/FITKAppFramework/FITKAppSettings.h"
#include "FITK_Kernel/FITKAppFramework/FITKRunTimeSetting.h"
#include "FITK_Kernel/FITKCore/FITKDataRepo.h"
#include "FITK_Component/FITKRenderWindowVTK/FITKGraph3DWindowVTK.h"
#include "FITK_Component/FITKRenderWindowVTK/FITKGraphRender.h"
#include "FITK_Component/FITKRenderWindowVTK/FITKGraphObjectVTK.h"
#include "FITK_Component/FITKWidget/FITKMdiArea.h"
#include "FITK_Interface/FITKInterfaceGeometry/FITKAbsGeoCommand.h"
#include "FITK_Interface/FITKInterfaceGeometry/FITKGeoCommandList.h"

namespace GUIOper
{
    Comp::FITKGraph3DWindowVTK* OperGraphEvent3D::getPreGraphWidget()
    {
        // 获取主窗口。
        GUI::MainWindow* mainWindow = FITKAPP->getGlobalData()->getMainWindowT<GUI::MainWindow>();
        if (mainWindow == nullptr){
            return nullptr;
        }

        GUI::RenderWidget* renderWidget = mainWindow->getRenderWidget();
        if (renderWidget == nullptr){
            return nullptr;
        }

        // 获取3D区域。
        Comp::FITKGraph3DWindowVTK* widget = dynamic_cast<Comp::FITKGraph3DWindowVTK*>(renderWidget->getRenderWin());
        return widget;
    }

    void OperGraphEvent3D::addGraphObjectToWidget(Graph::GraphObjectBase* obj, Comp::FITKGraph3DWindowVTK* graphWidget, bool fitView)
    {
        if (!obj || !graphWidget){
            return;
        }

        obj->removeFromGraphWidget();
        graphWidget->addObject(obj->getRenderLayer(), obj, true);

        if (fitView){
            graphWidget->fitView();
        }
        else{
            graphWidget->reRender();
        }
    }

    QList<Graph::GraphObjectBase*> OperGraphEvent3D::getGraphObjectsByDataId(int dataObjId, bool forceUpdate)
    {
        Q_UNUSED(forceUpdate);

        //获取或创建可视化对象。
        Graph::GraphObjectBase* obj{ nullptr };
        QList<Graph::GraphObjectBase*> objs;
        bool isValid = false;

        //获取可视化窗口。
        Comp::FITKGraph3DWindowVTK* graphWidget = getPreGraphWidget();
        if (graphWidget == nullptr){
            return objs;
        }

        //获取数据对象。
        Core::FITKAbstractDataObject* dataObj = Core::FITKDataRepo::getInstance()->getDataByID(dataObjId);
        if (dataObj == nullptr){
            return objs;
        }

        //获取模型与符号可视化对象管理器。
        Graph::GraphDataProviderModel* modelProvider = Graph::GraphDataProviderManager::getInstance()->getModelProvider(graphWidget);
        Graph::GraphDataProviderMark* markProvider = Graph::GraphDataProviderManager::getInstance()->getMarkProvider(graphWidget);
        if (modelProvider == nullptr || markProvider == nullptr){
            return objs;
        }

        //检查数据ID是否为几何模型（命令）。
        Interface::FITKAbsGeoCommand* geoCmd = dynamic_cast<Interface::FITKAbsGeoCommand*>(dataObj);
        if (geoCmd && !isValid){
            obj = modelProvider->getGeoModelGraph(dataObjId);
            isValid = true;
        }

        //检查数据是否是网格
        ModelData::MeshKernel* meshKernel = dynamic_cast<ModelData::MeshKernel*>(dataObj);
        if (meshKernel && !isValid) {
            obj = modelProvider->getMeshKernelGraph(dataObjId);
            isValid = true;
        }

        if (obj){
            objs.push_back(obj);
        }

        return objs;
    }

    QList<Graph::GraphObjectBase*> OperGraphEvent3D::getGeometryGraphs()
    {
        QList<Graph::GraphObjectBase*> objs = {};
        
        //获取可视化窗口。
        Comp::FITKGraph3DWindowVTK* graphWidget = getPreGraphWidget();
        if (graphWidget == nullptr) {
            return objs;
        }

        // 获取全局几何数据列表。
        Interface::FITKGeoCommandList* geoData = FITKGLODATA->getGeometryData<Interface::FITKGeoCommandList>();
        //获取渲染模型管理器
        Graph::GraphDataProviderModel* modelProvider = Graph::GraphDataProviderManager::getInstance()->getModelProvider(graphWidget);
        if (modelProvider == nullptr || geoData == nullptr) {
            return objs;
        }

        for (int i = 0; i < geoData->getDataCount(); i++) {
            Interface::FITKAbsGeoCommand* geoCommand = geoData->getDataByIndex(i);
            if (geoCommand == nullptr) {
                continue;
            }
            objs << modelProvider->getGeoModelGraph(geoData->getDataObjectID());
        }
        return objs;
    }

    QList<Graph::GraphObjectBase*> OperGraphEvent3D::getMeshKernelGraphs()
    {
        QList<Graph::GraphObjectBase*> objs = {};

        //获取可视化窗口。
        Comp::FITKGraph3DWindowVTK* graphWidget = getPreGraphWidget();
        //获取渲染模型管理器
        Graph::GraphDataProviderModel* modelProvider = Graph::GraphDataProviderManager::getInstance()->getModelProvider(graphWidget);
        if (graphWidget == nullptr || modelProvider == nullptr) {
            return objs;
        }

        //获取网格管理器
        ModelData::MeshManager* meshManager = FITKAPP->getGlobalData()->getMeshData<ModelData::MeshManager>();
        if (meshManager == nullptr) {
            return objs;
        }

        for (int mDIndex = 0; mDIndex < meshManager->getDataCount(); mDIndex++) {
            ModelData::MeshData* meshData = meshManager->getDataByIndex(mDIndex);
            if (meshData == nullptr) {
                continue;
            }

            for (int i = 0; i < meshData->getDataCount(); i++) {
                ModelData::MeshKernel* meshKernel = meshData->getDataByIndex(i);
                if (meshKernel == nullptr) {
                    continue;
                }
                objs << modelProvider->getMeshKernelGraph(meshKernel->getDataObjectID());
            }
        }

        return objs;
    }

    void OperGraphEvent3D::updateGraphRecursively(int dataObjId, int refLayer, bool recDown)
    {
        // 检查数据ID。
        Interface::FITKAbsGeoCommand* cmd = FITKDATAREPO->getTDataByID<Interface::FITKAbsGeoCommand>(dataObjId);
        if (!cmd) {
            return;
        }

        // 更新可视化对象。
        updateGraph(dataObjId);

        // 更新引用数据对象。
        if (refLayer != 0)
        {
            int subLayer = refLayer == -1 ? -1 : refLayer - 1;

            if (recDown)
            {
                int nRefedCmd = cmd->getReferencedCmdCount();
                for (int i = 0; i < nRefedCmd; i++)
                {
                    Interface::FITKAbsGeoCommand* cmdRef = cmd->getReferencedCmdByIndex(i);
                    if (!cmdRef)
                    {
                        continue;
                    }

                    // 递归更新。
                    updateGraphRecursively(cmdRef->getDataObjectID(), subLayer, recDown);
                }
            }
            else
            {
                int nRefCmd = cmd->getReferenceCmdCount();
                for (int i = 0; i < nRefCmd; i++)
                {
                    Interface::FITKAbsGeoCommand* cmdRef = cmd->getReferenceCmdByIndex(i);
                    if (!cmdRef)
                    {
                        continue;
                    }

                    // 递归更新。
                    updateGraphRecursively(cmdRef->getDataObjectID(), subLayer, recDown);
                }
            }
        }
    }

    void OperGraphEvent3D::reRender()
    {
        Comp::FITKGraph3DWindowVTK* graphWidget = getPreGraphWidget();
        if (graphWidget == nullptr) {
            return;
        }
        graphWidget->reRender();
    }

    void OperGraphEvent3D::fitView()
    {
        Comp::FITKGraph3DWindowVTK* graphWidget = getPreGraphWidget();
        if (graphWidget == nullptr) {
            return;
        }
        graphWidget->fitView();
    }
}  // namespace GUIOper