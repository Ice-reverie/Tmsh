#ifndef _MESH_DATA_MGR_H___
#define _MESH_DATA_MGR_H___

#include "ModelDataAPI.h"
#include "FITK_Kernel/FITKCore/FITKAbstractDataManager.hpp"
#include "FITK_Kernel/FITKCore/FITKAbstractDataObject.h"
#include "MeshData.h"
#include <functional>
#include <QHash>

namespace ModelData
{
    /**
     * @file MeshManager.h
     * @brief 网格数据管理器类
     * @details 用于管理网格数据的创建、存储和访问
     * @author libaojun
     * @date 2024-03-31
     */
    class ModelDataAPI MeshManager : 
        public Core::FITKAbstractDataManager<MeshData>, public Core::FITKAbstractDataObject
    {
    public:
        /**
         * @brief 构造函数
         * @author libaojun
         * @date 2024-03-31
         */
        explicit MeshManager();
        
        /**
         * @brief 析构函数
         * @author libaojun
         * @date 2024-03-31
         */
        virtual ~MeshManager();

        /**
         * @brief 根据名称获取网格数据对象
         * @param name 网格数据名称
         * @return 网格数据对象指针，如果不存在则创建新的对象
         * @author libaojun
         * @date 2024-03-31
         */
        MeshData* getMeshDataObjectByName(const QString& name);
        /**
         * @brief 根据名称获取网格数据对象 
         * @param name 网格数据名称
         * @return 网格数据对象指针，如果不存在则创建新的对象
         * @author libaojun
         * @date 2024-04-01
         */
        template<typename T>
        T* getMeshDataObjectByNameT(const QString& name)
        {
            return dynamic_cast<T*>(getMeshDataObjectByName(name));
        }   
        /**
         * @brief 注册网格数据创建函数
         * @param type 网格数据类型
         * @param creator 创建函数指针
         * @return 是否注册成功
         * @author libaojun
         * @date 2024-03-31
         */
        bool registerMeshDataCreator(const QString& type, std::function<MeshData*()> creator);

        /**
         * @brief 取消注册网格数据创建函数
         * @param type 网格数据类型
         * @return 是否取消注册成功
         * @author libaojun
         * @date 2024-03-31
         */
        void  unregisterMeshDataCreator(const QString& type);

        /**
         * @brief 根据类型创建网格数据对象
         * @param type 网格数据类型
         * @return 网格数据对象指针，如果类型未注册返回nullptr
         * @author libaojun
         * @date 2024-03-31
         */
        MeshData* createMeshDataByType(const QString& type);
       
    private:
        QHash<QString, std::function<MeshData*()>> _meshDataCreators;  ///< 网格数据创建函数映射表
    };
}

#endif
