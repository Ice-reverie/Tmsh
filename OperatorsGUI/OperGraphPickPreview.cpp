#include "OperGraphPickPreview.h"

#include "GUIFrame/MainWindow.h"
#include "GraphData/GraphDataPick/GraphDataPickInfo.h"
#include "GraphData/GraphDataPick/GraphPickProvider.h"
#include "GraphData/GraphDataAdaptor/GraphObjectBase.h"
//#include "GraphData/GraphDataAdaptor/GraphObjectGeoModel.h"
#include "GraphData/GraphDataAdaptor/GraphObjectPickPreview.h"

#include "FITK_Kernel/FITKAppFramework/FITKAppFramework.h"
#include "FITK_Kernel/FITKAppFramework/FITKGlobalData.h"
#include "FITK_Kernel/FITKCore/FITKDataRepo.h"
#include "FITK_Interface/FITKInterfaceModel/FITKUnstructuredMesh.h"
#include "FITK_Interface/FITKVTKAlgorithm/FITKGraphActor.h"
#include "FITK_Interface/FITKVTKAlgorithm/FITKGraphActor2D.h"
#include "FITK_Component/FITKWidget/FITKMdiArea.h"
#include "FITK_Component/FITKRenderWindowVTK/FITKGraph3DWindowVTK.h"
#include "FITK_Component/FITKRenderWindowVTK/FITKGraphRender.h"
#include "FITK_Component/FITKRenderWindowVTK/FITKGraphObjectVTK.h"

#include <vtkActor.h>
#include <vtkDataSet.h>
#include <vtkUnstructuredGrid.h>

namespace GUIOper
{
    OperGraphPickPreview::OperGraphPickPreview()
    {
        // 实例化预选高亮可视化对象。
        m_graphObjPreview = new Graph::GraphObjectPickPreview;

        // 初始化拾取数据管理器。
        m_pickedProvider = Graph::GraphPickProvider::getInstance();

        // 新版本：根据当前拾取对象类型与计算方法预选。
        //@{
        // 初始化可视化对象三维数据。
        m_graphObjPreview->setPickedData(m_pickedProvider->getPickedPreviewDataSet());
        //@}
    }

    OperGraphPickPreview::~OperGraphPickPreview()
    {
        // 析构。
        if (m_graphObjPreview)
        {
            m_graphObjPreview->removeFromGraphWidget();

            delete m_graphObjPreview;
            m_graphObjPreview = nullptr;
        }
    }

    void OperGraphPickPreview::picked(Comp::FITKGraph3DWindowVTK* graphWindow, vtkActor2D* actor)
    {
        if (!graphWindow || !actor)
        {
            return;
        }

        bool needUpdate = m_pickedProvider->addPreviewPicked(actor);
        if (!needUpdate)
        {
            return;
        }

        // 添加到渲染窗口。
        m_graphObjPreview->setVisible(true);
        addGraphObjectToWidget(m_graphObjPreview, graphWindow);

        // 刷新窗口。
        graphWindow->reRender();
    }

    void OperGraphPickPreview::picked(Comp::FITKGraph3DWindowVTK* graphWindow, vtkActor* actor, int index, double* pickedWorldPos)
    {
        if (!graphWindow || !actor || !m_graphObjPreview)
        {
            return;
        }

        Graph::PickObjType pickType = Graph::GraphDataPickInfo::GetPickInfo()._pickObjType;

        // 新版本：根据当前拾取对象类型与计算方法预选。
        //@{
        bool needUpdate = m_pickedProvider->addPreviewPicked(actor, index, pickedWorldPos);
        if (!needUpdate)
        {
            return;
        }

        // 设置演员样式。
        //@{
        switch (pickType){
        case Graph::PickObjType::PickGeoVertex:
            m_graphObjPreview->setPickedType(1);
            break;
        case Graph::PickObjType::PickGeoCurve:
            m_graphObjPreview->setPickedType(2);
            break;
        case Graph::PickObjType::PickGeoFace:
        case Graph::PickObjType::PickGeoSolid:
            m_graphObjPreview->setPickedType(3);
            break;
        default:
            m_graphObjPreview->setVisible(false);
            return;
        }
        //@}

        // 添加到渲染窗口。
        m_graphObjPreview->setVisible(true);
        addGraphObjectToWidget(m_graphObjPreview, graphWindow);

        // 刷新窗口。
        graphWindow->reRender();
    }

    void OperGraphPickPreview::clear(Comp::FITKGraph3DWindowVTK* graphWindow)
    {
        // 清空预选数据。
        Graph::GraphPickProvider::getInstance()->clearPickedDataPreview();

        // 取消预选高亮并隐藏可视化对象。
        if (m_graphObjPreview)
        {
            m_graphObjPreview->disHighlight();
            m_graphObjPreview->setVisible(false);

            if (!graphWindow)
            {
                graphWindow = dynamic_cast<Comp::FITKGraph3DWindowVTK*>(m_graphObjPreview->getGraphWidget());
            }
        }

        // 刷新窗口。
        if (graphWindow)
        {
            graphWindow->reRender();
        }
    }

    void OperGraphPickPreview::addGraphObjectToWidget(Graph::GraphObjectBase* obj, Comp::FITKGraph3DWindowVTK* graphWidget)
    {
        if (!obj || !graphWidget)
        {
            return;
        }

        // 添加可视化对象。
        //@{
        obj->removeFromGraphWidget();
        graphWidget->addObject(obj->getRenderLayer(), obj, true);
        //}

        // 刷新窗口。
        //@{
        graphWidget->reRender();
        //@}
    }
}  // namespace GUIOper