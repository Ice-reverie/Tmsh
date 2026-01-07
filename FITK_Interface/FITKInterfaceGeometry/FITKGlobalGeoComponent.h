/*****************************************************************//**
 * @file        FITKGlobalGeoComponent.h
 * @brief       全局几何分组数据。

 * @author      ChengHaotian (yeguangbaozi@foxmail.com)
 * @date        2024-10-28
 *********************************************************************/

#ifndef __FITKGLOBALGEOCOMPONENT_H__
#define __FITKGLOBALGEOCOMPONENT_H__ 

#include "FITK_Kernel/FITKCore/FITKAbstractDataManager.hpp"
#include "FITK_Interface/FITKInterfaceModel/FITKModelEnum.h"
#include "FITK_Interface/FITKInterfaceModel/FITKAbstractGeoModel.h"

#include "FITKInterfaceGeometryAPI.h"

namespace Interface
{
    /**
     * @brief       全局几何分组数据类。（可存储不同模型内的同种类型形状）
     * @author      ChengHaotian (yeguangbaozi@foxmail.com)
     * @date        2024-10-28
     */
    class FITKInterfaceGeometryAPI FITKGlobalGeoComponent : 
        public Core::FITKAbstractDataManager<FITKGeoComponent>,  
        public Core::FITKAbstractNDataObject
    {
        FITKCLASS(Interface, FITKGlobalGeoComponent);
    public:
        /**
         * @brief       构造函数。
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2024-10-28
         */
        FITKGlobalGeoComponent(const FITKModelEnum::FITKModelSetType type);

        /**
         * @brief       析构函数。[虚函数]
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2024-10-28
         */
        virtual ~FITKGlobalGeoComponent() = default;
        /**
         * @brief  获取分组类型.
         * @author YanZhiHui(chanyuantiandao@126.com)
         * @date   2024-10-30
         */
        FITKModelEnum::FITKModelSetType getGeoType() const { return m_geoType; }
    protected:
        /**
         * @brief       几何分组形状类型。
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2024-10-28
         */
        FITKModelEnum::FITKModelSetType m_geoType = FITKModelEnum::FITKModelSetType::FMSNone;

    };

    /**
     * @brief       全局几何分组数据管理器类。
     * @author      ChengHaotian (yeguangbaozi@foxmail.com)
     * @date        2024-10-28
     */
    class FITKInterfaceGeometryAPI FITKGlobalGeoComponentManager : 
        public Core::FITKAbstractDataManager<FITKGlobalGeoComponent>
    {
    public:
        /**
         * @brief       构造函数。
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2024-10-28
         */
        FITKGlobalGeoComponentManager() = default;

        /**
         * @brief       析构函数。[虚函数]
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2024-10-28
         */
        virtual ~FITKGlobalGeoComponentManager() = default;

    };
}

#endif //!__FITKGLOBALGEOCOMPONENT_H__
