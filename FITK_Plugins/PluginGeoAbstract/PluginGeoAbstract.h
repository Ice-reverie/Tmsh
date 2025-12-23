/**
 * 
 * @file PluginGeoAbstract.h
 * @brief 几何插件抽象类声明
 * @author libaojun
 * @date 2025-03-31
 * 
 */
#ifndef _PLUGIN_GEO_ABSTRACT_H___
#define _PLUGIN_GEO_ABSTRACT_H___

#include "PluginGeoAbstractAPI.h"
#include "FITK_Plugins/PluginAbstract/PluginAbstract.h"
#include <functional>

namespace Plugin
{
    /**
     * @brief 几何插件抽象类
     * @author libaojun
     * @date 2025-03-31
     */
    class PluginGeoAbstractAPI PluginGeoAbstract : public PluginAbstract
    {
    public:
        /**
         * @brief 构造函数
         * @param lib 插件库指针
         * @author libaojun
         * @date 2025-03-31
         */
        explicit PluginGeoAbstract(QLibrary* lib);

        /**
         * @brief 析构函数
         * @author libaojun
         * @date 2025-03-31
         */
        virtual ~PluginGeoAbstract() = 0;


        /**
         * @brief 卸载插件
         * @author libaojun
         * @date 2025-03-31
         */
        virtual void unInstall() override;

        

    protected:
        
    };
}

#endif
