#include "PluginGeoAbstract.h" 
#include "FITK_Kernel/FITKAppFramework/FITKAppFramework.h"
#include "FITK_Kernel/FITKAppFramework/FITKGlobalData.h"

namespace Plugin
{
    PluginGeoAbstract::PluginGeoAbstract(QLibrary* lib) : PluginAbstract(lib)
    {
        // 几何插件特有的初始化代码
    }

    PluginGeoAbstract::~PluginGeoAbstract()
    {
        // 几何插件特有的清理代码
    }

    void PluginGeoAbstract::unInstall()
    {
        // 清空已注册的操作器列表
        this->clearOperators();
        // 调用基类的卸载函数
        PluginAbstract::unInstall();
        
    }
}
