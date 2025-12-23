#include "PluginGridAbstract.h"
#include "ModelData/MeshManager.h"
#include "ModelData/MeshData.h"
#include "FITK_Kernel/FITKAppFramework/FITKAppFramework.h"
#include "FITK_Kernel/FITKAppFramework/FITKGlobalData.h"

namespace Plugin
{
    /**
     * @brief 构造函数
     * @author libaojun
     * @date 2024-03-31
     */
    PluginGridAbstract::PluginGridAbstract(QLibrary* lib)
        : PluginAbstract (lib)
    {
    }

    /**
     * @brief 析构函数
     * @author libaojun
     * @date 2024-03-31
     */
    PluginGridAbstract::~PluginGridAbstract()
    {
        
    }

    void PluginGridAbstract::unInstall()
    {
        // 调用基类的卸载函数
        PluginAbstract::unInstall();

        //清理网格数据，释放资源等
        QString plugName = this->getPluginName();
        if (plugName.isEmpty())return;

        AppFrame::FITKGlobalData* globalData = FITKAPP->getGlobalData();
        if (globalData == nullptr) return ;
        ModelData::MeshManager* meshManager = globalData->getMeshData<ModelData::MeshManager>();
        if (meshManager == nullptr) return;

        meshManager->unregisterMeshDataCreator(plugName);
        
    }

    /**
     * @brief 获取网格数据对象
     * @param obj 数据对象指针
     * @return 网格数据对象指针，如果转换失败返回nullptr
     * @author libaojun
     * @date 2024-03-31
     */
    ModelData::MeshData* PluginGridAbstract::getMeshData()
    {
        QString plugName = this->getPluginName();
        if (plugName.isEmpty())
            return nullptr;

        AppFrame::FITKGlobalData* globalData  = FITKAPP->getGlobalData();
        if (globalData == nullptr)
            return nullptr;

        ModelData::MeshManager* meshManager = globalData->getMeshData<ModelData::MeshManager>();
        if (meshManager == nullptr) 
            return nullptr;

        return meshManager->getMeshDataObjectByName(plugName);
    }
}         