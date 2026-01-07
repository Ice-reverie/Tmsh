/**********************************************************************
 * @file   PluginTmshExec.h
 * @brief  Tmsh插件接口声明
 * @date   2025-12-22
 *********************************************************************/
#ifndef __PLUGIN_TMSH_EXEC__H___
#define __PLUGIN_TMSH_EXEC__H___

#include "PluginTmshExecAPI.h"
#include "PluginGridAbstract/PluginGridAbstract.h"

extern "C"
{
    PluginTmshExecAPI QString FITKLibraryRecognizeFun();
    PluginTmshExecAPI AppFrame::FITKAbstractPlugin* FITKLibraryLoadFun(QLibrary*);
}

namespace Tmsh
{
    class FITKMeshGenTmshExecInterface;
}

namespace Plugin
{
    class PluginTmshExec : public Plugin::PluginGridAbstract
    {
    public:
        explicit PluginTmshExec(QLibrary* dylibrary);
        virtual ~PluginTmshExec();

        virtual QString getPluginName() override;

    private:
        virtual void install() override;
        virtual void unInstall() override;
        void initRibbonUI();

    private:
        Tmsh::FITKMeshGenTmshExecInterface* _tMshExecGen{};
    };
}

#endif

