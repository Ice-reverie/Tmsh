#include "OperWorkingDir.h"
#include "GUIFrame/WorkingDirDlg.h"
#include "FITK_Kernel/FITKAppFramework/FITKAppFramework.h"
#include "FITK_Kernel/FITKAppFramework/FITKGlobalData.h"

bool GUIOper::OperWorkingDir::execGUI()
{
    //主界面
    AppFrame::FITKGlobalData* g = FITKAPP->getGlobalData();
    if (g == nullptr) return false;
    //弹出对话框
    GUI::WorkingDirDlg d(g->getMainWindow());
    return d.exec() == QDialog::Accepted;
}
