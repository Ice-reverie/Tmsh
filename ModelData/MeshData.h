#ifndef MESHDATA_H__
#define MESHDATA_H__

#include "FITK_Kernel/FITKCore/FITKAbstractDataManager.hpp"
#include "FITK_Kernel/FITKCore/FITKAbstractNDataObject.h" 
#include "Meshkernel.h"
#include "ModelDataAPI.h"

namespace ModelData
{

    /**
     * @file MeshData.h
     * @brief 网格数据管理类
     * @details 用于管理 FITKAbstractMesh 对象的类，继承自 FITKAbstractDataManager 模板类
     * @date 2024-03-31
     */
    class ModelDataAPI MeshData : public Core::FITKAbstractNDataObject,
        public Core::FITKAbstractDataManager<ModelData::MeshKernel>
         
    {
        Q_OBJECT

    public:
        /**
         * @brief 构造函数
         * @date 2024-03-31
         */
        explicit MeshData();

        /**
         * @brief 析构函数
         * @date 2024-03-31
         */
        virtual ~MeshData();

    private:
        // 在这里添加私有成员
    };

} // namespace ModelData

#endif // MESHDATA_H 
