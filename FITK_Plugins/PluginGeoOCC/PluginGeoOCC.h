/**
 * 
 * @file PluginGeoOCC.h
 * @brief OCC插件接口声明 
 * @author LiBaojun (libaojunqd@foxmail.com)
 * @date 2025-03-06
 * 
 */
#ifndef __PLUGIN_GEO_OCC__H___
#define __PLUGIN_GEO_OCC__H___
 
#include "PluginGeoOCCAPI.h"
#include "PluginGeoAbstract/PluginGeoAbstract.h"
extern "C"
{
    PluginGeoOCCAPI QString FITKLibraryRecognizeFun();
    PluginGeoOCCAPI AppFrame::FITKAbstractPlugin* FITKLibraryLoadFun(QLibrary*);
}
namespace OCC
{
    class  FITKGeoCompOCCInterface;
}

/**
 * @brief OCC插件
 * @author LiBaojun (libaojunqd@foxmail.com)
 * @date 2025-03-06
 */
class PluginGeoOCC: public Plugin::PluginGeoAbstract
{
public:
    /**
     * @brief Construct a new Plugin Geo O C C object
     * @param[i]  dylibrary      动态库
     * @author LiBaojun (libaojunqd@foxmail.com)
     * @date 2025-03-06
     */
    explicit PluginGeoOCC(QLibrary* dylibrary);
    /**
     * @brief Destroy the Plugin Geo O C C object
     * @author LiBaojun (libaojunqd@foxmail.com)
     * @date 2025-03-06
     */
    virtual ~PluginGeoOCC();

    /**
        * @brief 获取插件名称，OpenCASCADE
        * @return QString
        * @author libaojun (libaojunqd@foxmail.com)
        * @date 2024-12-04
        */
    virtual QString getPluginName() override;

private:
    /**
     * @brief 插件加载
     * @author libaojun (libaojunqd@foxmail.com)
     * @date 2024-12-04
     */
    virtual void install() override;
    /**
     * @brief 插件卸载
     * @author libaojun (libaojunqd@foxmail.com)
     * @date 2024-12-04
     */
    virtual void unInstall() override;
    /**
     * @brief 初始化RibbonUI
     * @author libaojun (libaojunqd@foxmail.com)
     * @date 2025-03-16
     */
    void initRibbonUI();
private:
    /**
     * @brief OCC组件接口
     * author libaojun
     * @date 2025-03-16
     */
    OCC::FITKGeoCompOCCInterface* _occComp{};

};

#endif
