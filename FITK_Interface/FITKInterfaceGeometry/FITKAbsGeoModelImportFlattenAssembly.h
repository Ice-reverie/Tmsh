/*****************************************************************//**
 * @file        FITKAbsGeoModelImportFlattenAssembly.h
 * @brief       拆分平铺装配模式读取几何。
 *
 * @author      ChengHaotian (yeguangbaozi@foxmail.com)
 * @date        2025-03-27
 *********************************************************************/

#ifndef __FITKABSGEOMODELIMPORTFLATTENASSEMBLY_H__
#define __FITKABSGEOMODELIMPORTFLATTENASSEMBLY_H__
 
#include "FITKAbsGeoImportBase.h"
#include "FITKInterfaceGeometryAPI.h"

namespace Interface
{
    /**
     * @brief       拆分平铺装配模式读取几何。
     * @author      ChengHaotian (yeguangbaozi@foxmail.com)
     * @date        2025-03-27
     */
    class FITKInterfaceGeometryAPI FITKAbsGeoModelImportFlattenAssembly : public FITKAbsGeoImportBase
    {
        FITKCLASS(Interface, FITKAbsGeoModelImportFlattenAssembly);
    public:
        /**
         * @brief       构造函数。
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2025-03-27
         */
        explicit FITKAbsGeoModelImportFlattenAssembly() = default;

        /**
         * @brief       析构函数。
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2025-03-27
         */
        virtual ~FITKAbsGeoModelImportFlattenAssembly() = default;

        /**
         * @brief       获取几何命令类型。[重写]
         * @return      命令类型
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2025-03-27
         */
        FITKGeoEnum::FITKGeometryComType getGeometryCommandType() override;

    };
}

#endif //!__FITKABSGEOMODELIMPORTFLATTENASSEMBLY_H__
