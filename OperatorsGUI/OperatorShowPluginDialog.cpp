#include "OperatorShowPluginDialog.h"
#include "GUIFrame/MainWindow.h"
#include "FITK_Component/FITKWidget/FITKPluginManageDialog.h"

#include "FITK_Kernel/FITKAppFramework/FITKAppFramework.h"
#include "FITK_Kernel/FITKAppFramework/FITKGlobalData.h"
#include "FITK_Kernel/FITKAppFramework/FITKAppSettings.h"
#include <QTreeWidget>

namespace Core {

    OperatorShowPluginDialog::OperatorShowPluginDialog() :
        FITKActionOperator()
    {
    }

    OperatorShowPluginDialog::~OperatorShowPluginDialog()
    {
    }
    bool OperatorShowPluginDialog::execGUI()
    {
        // 获取主窗口
        GUI::MainWindow* mainWin = dynamic_cast<GUI::MainWindow*>(FITKAPP->getGlobalData()->getMainWindow());
        if (mainWin == nullptr) {
            return false;
        }

        // 获取安装和卸载路径
        QString pluginPath = QApplication::applicationDirPath() + "/Plugins";

        // 创建并显示插件管理对话框
        Plugins::FITKPluginManageDialog dialog(pluginPath, mainWin);
        dialog.exec();

        return false;
    }

    bool OperatorShowPluginDialog::execProfession()
    {
        return false;
    }

} // namespace Core