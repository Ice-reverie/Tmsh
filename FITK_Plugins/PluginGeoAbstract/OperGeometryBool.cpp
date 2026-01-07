#include "OperGeometryBool.h"
#include "FITK_Kernel/FITKAppFramework/FITKGlobalData.h"
#include "FITK_Kernel/FITKAppFramework/FITKAppFramework.h"
#include "OperatorsInterface/GraphEventOperator.h"
#include "GUIWidget/GUIEnumType.h"
#include "GUIFrame/MainWindow.h"
#include "GUIFrame/ControlPanel.h"

#include "FITK_Plugins/PluginGeoAbstract/GUIGeometryBoolDialog.h"
#include <QWidget>

bool ModelOper::OperGeometryBool::execGUI(){
    //获取动作名称
    QString actionName = _emitter->objectName();
    if (actionName == "actionBooleanUnion")
    {
        this->createGeometryUnite();
    }
    else if (actionName == "actionBooleanIntersection")
    {
        this->createGeometryIntersect();
    }
    else if (actionName == "actionBooleanDifference")
    {
        this->createGeometryDifference();
    }
    return false;
}

bool ModelOper::OperGeometryBool::execProfession()
{
    int objID = -1;
    this->argValue("objID", objID);
    //获取三维可视对象数据-用于渲染数据对象
    EventOper::GraphEventOperator* graphOper = FITKOPERREPO->getOperatorT<EventOper::GraphEventOperator>("GraphPreprocess");
    if (graphOper == nullptr)return false;
    graphOper->updateGraphRecursively(objID, 2, false);
    //刷新树
    GUI::MainWindow* mw = GUI::MainWindow::GetMainWindowFromFramework();
    if (mw == nullptr) return false;
    GUI::ControlPanel* cp = mw->getControlPanel();
    if (cp == nullptr) return false;
    GUI::TreeWidgetPage* tw = cp->getTreeWidget(GUI::TreeWidgetType::PageModel);
    if (tw == nullptr) return false;
    tw->updatePage();
    return true;
}
bool ModelOper::OperGeometryBool::createGeometryUnite()
{
    //主界面
    QWidget* mw = FITKAPP->getGlobalData()->getMainWindow();
    if (!mw) return false;
    //创建界面
    GUI::GUIGeometryBoolDialog* dialog = new GUI::GUIGeometryBoolDialog(this, Interface::FITKAbsGeoOperBool::GBTAdd, mw);
    //当界面执行完毕后执行
    connect(dialog, &QDialog::accepted, [this]() { this->execProfession(); });
    dialog->show();
    return true;
}
bool ModelOper::OperGeometryBool::createGeometryIntersect()
{
    //主界面
    QWidget* mw = FITKAPP->getGlobalData()->getMainWindow();
    if (!mw) return false;
    //创建界面
    GUI::GUIGeometryBoolDialog* dialog = new GUI::GUIGeometryBoolDialog(this, Interface::FITKAbsGeoOperBool::GBTCommon, mw);
    //当界面执行完毕后执行
    connect(dialog, &QDialog::accepted, [this]() { this->execProfession(); });
    dialog->show();
    return true;
}
bool ModelOper::OperGeometryBool::createGeometryDifference()
{
    //主界面
    QWidget* mw = FITKAPP->getGlobalData()->getMainWindow();
    if (!mw) return false;
    //创建界面
    GUI::GUIGeometryBoolDialog* dialog = new GUI::GUIGeometryBoolDialog(this, Interface::FITKAbsGeoOperBool::GBTCut, mw);
    //当界面执行完毕后执行
    connect(dialog, &QDialog::accepted, [this]() { this->execProfession(); });
    dialog->show();
    return true;
}
