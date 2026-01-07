#include "OperGraphPreprocess.h"

#include "GraphData/GraphDataAdaptor/GraphObjectBase.h"

#include "FITK_Kernel/FITKAppFramework/FITKAppFramework.h"
#include "FITK_Kernel/FITKAppFramework/FITKGlobalData.h"
#include "FITK_Kernel/FITKAppFramework/FITKAppSettings.h"
#include "FITK_Kernel/FITKAppFramework/FITKRunTimeSetting.h"
#include "FITK_Kernel/FITKCore/FITKDataRepo.h"
#include "FITK_Component/FITKRenderWindowVTK/FITKGraph3DWindowVTK.h"
#include "FITK_Interface/FITKInterfaceGeometry/FITKAbsGeoCommand.h"

namespace GUIOper
{
    void OperGraphPreprocess::updateAllGraph()
    {
        Comp::FITKGraph3DWindowVTK* graphWidget = getPreGraphWidget();
        if (graphWidget == nullptr) {
            return;
        }

        QList<Graph::GraphObjectBase*> objs = {};
        //获取全部几何渲染对象
        objs << getGeometryGraphs();
        //获取全部网格渲染对象
        objs << getMeshKernelGraphs();

        //添加到渲染窗口
        for (Graph::GraphObjectBase* obj : objs) {
            if (obj == nullptr) {
                continue;
            }

            obj->setRender(graphWidget->getRenderer(obj->getRenderLayer()));
            obj->update(true);

            addGraphObjectToWidget(obj, graphWidget, false);
        }

        //刷新渲染。
        reRender();

        //自适应
        fitView();
    }

    void OperGraphPreprocess::updateGraph(int dataObjId, bool forceUpdate)
    {
        Comp::FITKGraph3DWindowVTK* graphWidget = getPreGraphWidget();
        if (graphWidget == nullptr) {
            return;
        }

        //获取或创建可视化对象。
        QList<Graph::GraphObjectBase*> objs = getGraphObjectsByDataId(dataObjId, forceUpdate);

        //添加至三维窗口。
        for (Graph::GraphObjectBase* obj : objs) {
            if (obj == nullptr) {
                continue;
            }
            
            obj->setRender(graphWidget->getRenderer(obj->getRenderLayer()));
            obj->update(forceUpdate);

            addGraphObjectToWidget(obj, graphWidget, false);
        }

        //刷新渲染。
        reRender();
    }

    void OperGraphPreprocess::updateColor(int dataObjId, QColor color, QVariant otherPar)
    {
        Comp::FITKGraph3DWindowVTK* graphWidget = getPreGraphWidget();
        if (graphWidget == nullptr) {
            return;
        }

        //获取可视化对象。
        QList<Graph::GraphObjectBase*> objs = getGraphObjectsByDataId(dataObjId, false);

        //刷新颜色
        for (Graph::GraphObjectBase* obj : objs) {
            if (obj == nullptr) {
                continue;
            }
            obj->updateSubColor(color, otherPar);
        }

        // 刷新渲染（重新挂接 renderer 并挂入窗口）。
        updateGraph(dataObjId, false);
    }
    void OperGraphPreprocess::updateColor(int dataObjId, QColor color, bool forceUpdate)
    {
        Comp::FITKGraph3DWindowVTK* graphWidget = getPreGraphWidget();
        if (graphWidget == nullptr) {
            return;
        }

        //获取可视化对象。
        QList<Graph::GraphObjectBase*> objs = getGraphObjectsByDataId(dataObjId, forceUpdate);

        //刷新颜色
        for (Graph::GraphObjectBase* obj : objs) {
            if (obj == nullptr) {
                continue;
            }
            obj->updateModelColor(color);
        }

        // 刷新渲染（重新挂接 renderer 并挂入窗口）。
        // 注意：这里不使用 forceUpdate，避免强制重建导致颜色被重置。
        updateGraph(dataObjId, false);
    }
}
