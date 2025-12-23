#include "MeshManager.h"

namespace ModelData
{
    /**
     * @brief 构造函数
     * @author libaojun
     * @date 2024-03-31
     */
    MeshManager::MeshManager()
    {
    }

    /**
     * @brief 析构函数
     * @author libaojun
     * @date 2024-03-31
     */
    MeshManager::~MeshManager()
    {
        _meshDataCreators.clear();
    }

    /**
     * @brief 根据名称获取网格数据对象
     * @param name 网格数据名称
     * @return 网格数据对象指针，如果不存在则创建新的对象
     * @author libaojun
     * @date 2024-03-31
     */
    MeshData* MeshManager::getMeshDataObjectByName(const QString& name)
    {
        // 遍历所有网格数据对象，查找匹配名称的对象
        MeshData* meshData = this->getDataByName(name);
        if (meshData == nullptr)
        {
            // 如果对象不存在，则创建一个新的对象
            meshData = this->createMeshDataByType(name);
            if(meshData == nullptr)
            {
                return nullptr;
            }
            meshData->setDataObjectName(name);
            this->appendDataObj(meshData);
        }
        return meshData;
    }

    /**
     * @brief 注册网格数据创建函数
     * @param type 网格数据类型
     * @param creator 创建函数指针
     * @return 是否注册成功
     * @author libaojun
     * @date 2024-03-31
     */
    bool MeshManager::registerMeshDataCreator(const QString& type, std::function<MeshData*()> creator)
    {
        if (type.isEmpty() || !creator) {
            return false;
        }
        _meshDataCreators[type] = creator;
        return true;
    }

    /**
     * @brief 取消注册网格数据创建函数
     * @param type 网格数据类型
     * @return 是否取消注册成功
     * @author libaojun
     * @date 2024-03-31
     */
    void  MeshManager::unregisterMeshDataCreator(const QString& type)
    {
        if (type.isEmpty()) return ;
        
        _meshDataCreators.remove(type);
        MeshData* meshData = this->getMeshDataObjectByName(type);
        if (meshData != nullptr)
        {
            delete meshData;
        } 
    }

    /**
     * @brief 根据类型创建网格数据对象
     * @param type 网格数据类型
     * @return 网格数据对象指针，如果类型未注册返回nullptr
     * @author libaojun
     * @date 2024-03-31
     */
    MeshData* MeshManager::createMeshDataByType(const QString& type)
    {
        if (type.isEmpty()) {
            return nullptr;
        }
        if (!_meshDataCreators.contains(type)) return nullptr;
        
        // 如果类型已注册，则创建新的对象
        std::function<MeshData*()> creator = _meshDataCreators[type];
        if (creator != nullptr) 
        {
            MeshData* meshData = creator();
            return meshData;
        }
   
        return nullptr;
    }
}


