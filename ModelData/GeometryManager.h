/**********************************************************************
 * @file   GeometryManager.h
 * @brief  几何数据管理器声明
 * @author liuzhonghua (liuzhonghuaszch@163.com)
 * @date   2025-04-16
 *********************************************************************/
#ifndef _GEOMETRY_DATA_MGR_H___
#define _GEOMETRY_DATA_MGR_H___

#include "ModelDataAPI.h"
#include "FITK_Kernel/FITKCore/FITKAbstractDataObject.h"
#include "FITK_Interface/FITKInterfaceGeometry/FITKGeoCommandList.h"

namespace Interface
{
    class FITKGeoCommandList;
}

namespace ModelData
{
    /**
     * @brief  几何数据管理器类
     * @author liuzhonghua (liuzhonghuaszch@163.com)
     * @date   2025-04-16
     */
    class ModelDataAPI GeometryManager : public Interface::FITKGeoCommandList
    {
    public:
        explicit GeometryManager();
        virtual ~GeometryManager();

    };
}


#endif
