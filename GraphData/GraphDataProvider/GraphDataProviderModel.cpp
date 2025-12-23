#include "GraphDataProviderModel.h"

#include "GraphData/GraphDataAdaptor/GraphObjectCommons.h"
#include "GraphData/GraphDataAdaptor/GraphObjectBase.h"
#include "ModelData/MeshKernel.h"

#include "FITK_Kernel/FITKCore/FITKDataRepo.h"
#include "FITK_Interface/FITKInterfaceGeometry/FITKAbsGeoCommand.h"

namespace Graph
{
    GraphDataProviderModel::GraphDataProviderModel(Comp::FITKGraph3DWindowVTK * graphWidget) :
        GraphDataProviderBase(graphWidget)
    {

    }

    GraphDataProviderModel::~GraphDataProviderModel()
    {

    }

    GraphObjectBase * GraphDataProviderModel::getGeoModelGraph(int objId)
    {
        //判断数据有效性
        Interface::FITKAbsGeoCommand* geoCommand = FITKDATAREPO->getTDataByID<Interface::FITKAbsGeoCommand>(objId);
        if (geoCommand == nullptr){
            return nullptr;
        }

        //模型可视化对象。
        GraphObjectBase* obj = getGeoGraphObject("GeoModel", _geoModelObjs, geoCommand);
        return obj;
    }

    GraphObjectBase * GraphDataProviderModel::getMeshKernelGraph(int objId)
    {
        //判断数据有效性
        ModelData::MeshKernel* meshKernel = FITKDATAREPO->getTDataByID< ModelData::MeshKernel>(objId);
        if (meshKernel == nullptr) {
            return nullptr;
        }

        //模型可视化对象。
        GraphObjectBase* obj = getGraphObject("MeshKernel", _meshKernelObjs, meshKernel);
        return obj;
    }

    void GraphDataProviderModel::setGeoModelVertexPick(bool isPick, int objID)
    {
        QList<Graph::GraphObjectBase*> graphObjs = {};
        if (objID == -1) {
            graphObjs.append(_geoModelObjs.values());
        }
        else {
            graphObjs.append(_geoModelObjs.value(objID));
        }

        for (Graph::GraphObjectBase* graphObj : graphObjs) {
            if (graphObj == nullptr) {
                continue;
            }
            graphObj->setPickModel(isPick, Graph::ShapeType::ModelVertex);
        }
    }

    void GraphDataProviderModel::setGeoModelCurverPick(bool isPick, int objID)
    {
        QList<Graph::GraphObjectBase*> graphObjs = {};
        if (objID == -1) {
            graphObjs.append(_geoModelObjs.values());
        }
        else {
            graphObjs.append(_geoModelObjs.value(objID));
        }

        for (Graph::GraphObjectBase* graphObj : graphObjs) {
            if (graphObj == nullptr) {
                continue;
            }
            graphObj->setPickModel(isPick, Graph::ShapeType::ModelEdge);
        }
    }

    void GraphDataProviderModel::setGeoModelFacePick(bool isPick, int objID)
    {
        QList<Graph::GraphObjectBase*> graphObjs = {};
        if (objID == -1) {
            graphObjs.append(_geoModelObjs.values());
        }
        else {
            graphObjs.append(_geoModelObjs.value(objID));
        }

        for (Graph::GraphObjectBase* graphObj : graphObjs) {
            if (graphObj == nullptr) {
                continue;
            }
            graphObj->setPickModel(isPick, Graph::ShapeType::ModelFace);
        }
    }

    void GraphDataProviderModel::setGeoModelSolidPick(bool isPick, int objID)
    {
        QList<Graph::GraphObjectBase*> graphObjs = {};
        if (objID == -1) {
            graphObjs.append(_geoModelObjs.values());
        }
        else {
            graphObjs.append(_geoModelObjs.value(objID));
        }

        for (Graph::GraphObjectBase* graphObj : graphObjs) {
            if (graphObj == nullptr) {
                continue;
            }
            graphObj->setPickModel(isPick, Graph::ShapeType::ModelSolid);
        }
    }
}