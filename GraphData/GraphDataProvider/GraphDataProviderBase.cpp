#include "GraphDataProviderBase.h"

#include "GraphData/GraphDataAdaptor/GraphObjectBase.h"
#include "GraphData/GraphDataAdaptor/ViewAdaptorBase.h"

#include "FITK_Kernel/FITKCore/FITKAbstractDataObject.h"
#include "FITK_Kernel/FITKAdaptor/FITKViewAdaptorFactory.h"
#include "FITK_Interface/FITKInterfaceGeometry/FITKAbsGeoCommand.h"

namespace Graph
{
    GraphDataProviderBase::GraphDataProviderBase(Comp::FITKGraph3DWindowVTK * graphWidget) :
        _graphWidget(graphWidget)
    {

    }

    GraphDataProviderBase::~GraphDataProviderBase()
    {

    }

    Comp::FITKGraph3DWindowVTK * GraphDataProviderBase::getGraphWidget()
    {
        return _graphWidget;
    }

    GraphObjectBase * GraphDataProviderBase::getGraphObject(QString KeyName, QHash<int, GraphObjectBase*>& objDict, Core::FITKAbstractDataObject * dataObj)
    {
        //判断数据是否为空
        if (dataObj == nullptr) {
            return nullptr;
        }

        //判断管理链表中是否已经包含该对象
        if (objDict.keys().contains(dataObj->getDataObjectID())) {
            return  objDict.value(dataObj->getDataObjectID());
        }

        //生成可视化对象。
        ViewAdaptorBase* adaptor = FITKVIEWADAPTORFACTORY->createT<ViewAdaptorBase>(KeyName, dataObj);
        if (adaptor == nullptr){
            return nullptr;
        }

        //设置输入数据并更新获取三维可视化对象。
        adaptor->setDataObject(dataObj);
        adaptor->adapt();

        GraphObjectBase* obj = adaptor->getOutputData();
        delete adaptor;

        if (obj == nullptr){
            return nullptr;
        }

        objDict.insert(dataObj->getDataObjectID(), obj);

        //关联数据删除信号
        connect(dataObj, &Core::FITKAbstractDataObject::dataObjectDestoried, this, [&](Core::FITKAbstractDataObject* objDelete)
        {
            GraphObjectBase* gObj = objDict.take(objDelete->getDataObjectID());
            if (gObj){
                delete gObj;
            }
        });
        return obj;
    }

    GraphObjectBase * GraphDataProviderBase::getGeoGraphObject(QString keyName, QHash<int, GraphObjectBase*>& objDict, Interface::FITKAbsGeoCommand * geoCom)
    {
        //判断数据是否为空
        if (geoCom == nullptr) {
            return nullptr;
        }

        //判断管理链表中是否已经包含该对象
        if (objDict.keys().contains(geoCom->getDataObjectID())) {
            return  objDict.value(geoCom->getDataObjectID());
        }

        //获取形状代理器
        Interface::FITKAbsGeoShapeAgent* shapeAgent = geoCom->getShapeAgent();
        if (shapeAgent == nullptr) {
            return nullptr;
        }

        //生成可视化对象。
        ViewAdaptorBase* adaptor = FITKVIEWADAPTORFACTORY->createT<ViewAdaptorBase>(keyName, shapeAgent);
        if (adaptor == nullptr) {
            return nullptr;
        }

        //设置输入数据并更新获取三维可视化对象。
        adaptor->setDataObject(shapeAgent);
        adaptor->adapt();

        GraphObjectBase* obj = adaptor->getOutputData();
        delete adaptor;

        if (obj == nullptr) {
            return nullptr;
        }

        objDict.insert(geoCom->getDataObjectID(), obj);

        //关联数据删除信号
        connect(geoCom, &Core::FITKAbstractDataObject::dataObjectDestoried, this, [&](Core::FITKAbstractDataObject* objDelete)
        {
            GraphObjectBase* gObj = objDict.take(objDelete->getDataObjectID());
            if (gObj) {
                delete gObj;
            }
        });
        return obj;
    }
}