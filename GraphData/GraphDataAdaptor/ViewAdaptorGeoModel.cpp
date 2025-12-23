#include "ViewAdaptorGeoModel.h"
#include "GraphObjectGeoModel.h"

#include "FITK_Interface/FITKInterfaceGeometry/FITKAbsGeoCommand.h"

namespace Graph
{
    bool ViewAdaptorGeoModel::adapt()
    {
        //获取形状代理器
        Interface::FITKAbsGeoShapeAgent* geoShapeAgent = dynamic_cast<Interface::FITKAbsGeoShapeAgent*>(_dataObj);
        if (geoShapeAgent == nullptr) {
            return false;
        }

        //获取几何命令
        Interface::FITKAbsGeoCommand* geoCommand = geoShapeAgent->getGeoCommand();
        if (geoCommand == nullptr) {
            return false;
        }

        //获取可视化对象
        GraphObjectGeoModel* graphObj = new GraphObjectGeoModel(geoCommand);

        //判断可视化对象的有效性
        if (graphObj->getActorCount() == 0) {
            delete graphObj;
            graphObj = nullptr;
            return false;
        }

        _outputData = graphObj;
        return true;
    }
}