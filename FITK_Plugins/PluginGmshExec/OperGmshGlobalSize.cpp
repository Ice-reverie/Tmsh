#include "OperGmshGlobalSize.h"
#include "FITK_Kernel/FITKAppFramework/FITKGlobalData.h"
#include "FITK_Kernel/FITKAppFramework/FITKAppFramework.h"
#include "OperatorsInterface/GraphEventOperator.h"

#include "FITK_Component/FITKGmshExeDriver/GUIGmshSettings.h"
#include <QWidget>

bool ModelOper::OperGmshGlobalSize::execGUI(){
    //获取动作名称
    //QString actionName = _emitter->objectName();
    //if (actionName == "actionCreateGlobalMesh")
    //{
    //    this->updateGlobalSize();
    //}
    return false;
}

bool ModelOper::OperGmshGlobalSize::execProfession()
{
    return true;
}
bool ModelOper::OperGmshGlobalSize::updateGlobalSize()
{
    //主界面
    //QWidget* mw = FITKAPP->getGlobalData()->getMainWindow();
    //if (!mw) return false;
    ////创建界面
    //GUI::GUIGmshSettings* dialog = new GUI::GUIGmshSettings( mw);
    //dialog->show();
    return true;
}
