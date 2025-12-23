#ifndef _PLUGIN_GRID_ABSTRACT_H___
#define _PLUGIN_GRID_ABSTRACT_H___

#include "PluginGridAbstractAPI.h"
#include "PluginAbstract/PluginAbstract.h"
#include "FITK_Kernel/FITKCore/FITKAbstractDataObject.h"

namespace ModelData
{
    class MeshManager;
    class MeshData;
}

namespace Plugin
{
    /**
     * @file PluginGridAbstract.h
     * @brief 网格抽象插件类
     * @details 用于管理网格相关的插件功能
     * @author libaojun
     * @date 2024-03-31
     */
    class PluginGridAbstractAPI PluginGridAbstract : public PluginAbstract
    { 
    public:
        /**
         * @brief 构造函数
         * @author libaojun
         * @date 2024-03-31
         */
        explicit PluginGridAbstract(QLibrary* lib);

        /**
         * @brief 析构函数
         * @author libaojun
         * @date 2024-03-31
         */
        virtual ~PluginGridAbstract() = 0 ;

    protected:
        /**
         * @brief 插件卸载函数
         * @details 重写基类的卸载函数，执行插件卸载操作
         * @author libaojun
         * @date 2024-03-31
         */
        virtual void unInstall() override;
        /**
         * @brief 获取网格数据对象 
         * @return 网格数据对象指针，如果转换失败返回nullptr
         * @author libaojun
         * @date 2024-03-31
         */
        ModelData::MeshData* getMeshData();

        /**
         * @brief 获取指定类型的网格数据对象
         * @tparam T 目标类型
         * @param obj 数据对象指针
         * @return 转换后的对象指针，如果转换失败返回nullptr
         * @author libaojun
         * @date 2024-03-31
         */
        template<typename T>
        T* getMeshData()
        {
            ModelData::MeshData* meshData = getMeshData();
            if (meshData == nullptr)
                return nullptr;
            return dynamic_cast<T*>(meshData);
        }

    private: 
    };
}

#endif 