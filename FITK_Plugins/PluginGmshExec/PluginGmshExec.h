/**********************************************************************
 * @file   PluginGmshExec.h
 * @brief  GMesh插件接口声明
 * @author guqingtao (15598887859@163.com)
 * @date   2025-06-05
 *********************************************************************/
#ifndef __PLUGIN_GMSH_EXEC__H___
#define __PLUGIN_GMSH_EXEC__H___
 
#include "PluginGmshExecAPI.h"
#include "PluginGridAbstract/PluginGridAbstract.h"

extern "C"
{
    PluginGmshExecAPI QString FITKLibraryRecognizeFun();
    PluginGmshExecAPI AppFrame::FITKAbstractPlugin* FITKLibraryLoadFun(QLibrary*);
}

namespace AppFrame
{
    class FITKComponentInterface;
}

namespace Gmsh
{
    class FITKMeshGenGmshExecInterface;
}

namespace Plugin
{
    /**
     * @brief  GMesh插件
     * @author guqingtao (15598887859@163.com)
     * @date   2025-06-05
     */
    class PluginGmshExec : public Plugin::PluginGridAbstract
    {
    public:
        /**
         * @brief    Construct a new Plugin FastCAE Grid object
         * @param[i] dylibrary 动态库
         * @return   void
         * @author   guqingtao (15598887859@163.com)
         * @date     2025-06-05
         */
        explicit PluginGmshExec(QLibrary* dylibrary);
        /**
         * @brief    Destroy the Plugin FastCAE Grid object
         * @return   void
         * @author   guqingtao (15598887859@163.com)
         * @date     2025-06-05
         */
        virtual ~PluginGmshExec();
        /**
         * @brief    获取插件名称
         * @return   QString
         * @author   guqingtao (15598887859@163.com)
         * @date     2025-06-05
         */
        virtual QString getPluginName() override;

    private:
        /**
         * @brief    插件加载
         * @return   void
         * @author   guqingtao (15598887859@163.com)
         * @date     2025-06-05
         */
        virtual void install() override;
        /**
         * @brief    插件卸载
         * @return   void
         * @author   guqingtao (15598887859@163.com)
         * @date     2025-06-05
         */
        virtual void unInstall() override;
        /**
         * @brief    初始化RibbonUI
         * @return   void
         * @author   guqingtao (15598887859@163.com)
         * @date     2025-06-05
         */
        void initRibbonUI();
    private:
        /**
         * @brief  GMesh网格组件接口
         * @author guqingtao (15598887859@163.com)
         * @date   2025-06-05
         */
        Gmsh::FITKMeshGenGmshExecInterface* _gMshExecGen{};

    };
}

#endif
