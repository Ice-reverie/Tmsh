#ifndef _FITK_GEOMODEL_IMPORT_H___
#define _FITK_GEOMODEL_IMPORT_H___
 
#include "FITKInterfaceGeometryAPI.h"
#include "FITKAbsGeoImportBase.h"
#include <QString>

namespace Interface
{
    class FITKInterfaceGeometryAPI FITKAbsGeoModelImport :
        public  FITKAbsGeoImportBase
    {
        FITKCLASS(Interface, FITKAbsGeoModelImport);
    public:

        explicit FITKAbsGeoModelImport() = default;

        virtual ~FITKAbsGeoModelImport() = default;;

        /**
         * @brief 获取几何命令类型
         * @return FITKGeoEnum::FITKGeometryComType
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2024-08-01
         */
        FITKGeoEnum::FITKGeometryComType getGeometryCommandType() override;


    };
}


#endif
